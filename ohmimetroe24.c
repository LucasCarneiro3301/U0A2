/*
	Lucas Carneiro de Araújo Lima
*/

#include "lib/config/config.h"
#include "lib/utils/get_E24_value.h"
#include "lib/utils/get_colors.h"
#include "lib/ws2812/ws2812.h"

const int standard_resistor = 10e3;	// Resistor de 10k ohm
const int adc_resolution = 4095; 	// Resolução do ADC (12 bits)

bool reset = false;

static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

void gpio_irq_handler(uint gpio, uint32_t events);
void draw_band_colors(char *color, Led_Matrix *frame, int column, bool clear, bool print);

int main() {
	ssd1306_t ssd;		// Inicializa a estrutura do display
	Led_Matrix frame;	// Frame atual da matriz de led
	char str_a[5], str_r[5], str_e[5]; // Buffer para armazenar a string
	bool cor = true;

	config(&ssd);	// Inicializa e configura todos os componentes

	ws2812_clean(&frame);	// Limpa a matriz 5x5

	gpio_set_irq_enabled_with_callback(BTNB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);	// Callback para BTN B

	while (true) {
		if(!reset) {
			adc_select_input(2); // Seleciona o ADC do pino 28.

			float sum = 0.0f;	// Registra a soma de todos os níveis ADC calculados em 500 iterações
			for (int i = 0; i < 500; i++) {
				sum += adc_read();
				sleep_ms(1);
			}

			float avg = sum / 500.0f; // Média aritmética para 500 iterações

			printf("Valor media: %f\n\n", avg);

			float rx = (standard_resistor * avg) / (adc_resolution - avg);	// Calcula o valor do resistor desconhecido

			float rc = get_E24_value(rx);									// Obtém o valor da série E24 mais próximo

			float error = (fabsf(rx - rc) / rx) * 100;						// Calcula o erro relativo

			char **colors = (rx >= 1.5e5) ? (char*[]){"--", "--", "--"} : get_colors(rc);	// Obtém as cores correspondentes

			printf("Valor calculado: %1.0f ||| Valor E24: %1.0f || Erro: %.2f\n\n", rx, rc, error);
			printf("Faixa 1: %s  ", colors[0]);
			printf("Faixa 2: %s  ", colors[1]);
			printf("Multiplicador: %s\n\n", colors[2]);

			sprintf(str_a, "%1.0f", avg);						   							// Converte em string
			sprintf(str_r, (rx >= 1.5e5) ? "--" : (rc < 10) ? "%.1f" : "%1.0f", rc);		// Converte em string
			sprintf(str_e, (rx >= 1.5e5) ? "--" : (error < 10) ? "%.2f" : "%.1f", error);	// Converte em string

		
			ssd1306_fill(&ssd, !cor);						// Limpa o display
			ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);	// Desenha um retângulo
			ssd1306_line(&ssd, 3, 37, 123, 37, cor);		// Desenha uma linha
			ssd1306_draw_string(&ssd, "Erro", 8, 6);		// Desenha uma string
			ssd1306_draw_string(&ssd, "(%)", 12, 16);		// Desenha uma string
			ssd1306_draw_string(&ssd, str_e, 8, 26);		// Desenha uma string
			ssd1306_draw_string(&ssd, "1:", 50, 6);			// Desenha uma string
			ssd1306_draw_string(&ssd, "2:", 50, 16);		// Desenha uma string
			ssd1306_draw_string(&ssd, "3:", 50, 26);		// Desenha uma string
			ssd1306_draw_string(&ssd, colors[0], 70, 6);	// Desenha uma string
			ssd1306_draw_string(&ssd, colors[1], 70, 16);	// Desenha uma string
			ssd1306_draw_string(&ssd, colors[2], 70, 26);	// Desenha uma string
			ssd1306_draw_string(&ssd, "ADC", 13, 41);		// Desenha uma string
			ssd1306_draw_string(&ssd, "Resisten.", 50, 41); // Desenha uma string
			ssd1306_line(&ssd, 44, 3, 44, 61, cor);			// Desenha uma linha vertical
			ssd1306_draw_string(&ssd, str_a, 8, 52);		// Desenha uma string
			ssd1306_draw_string(&ssd, str_r, 59, 52);		// Desenha uma string
			ssd1306_send_data(&ssd);						// Atualiza o display

			draw_band_colors(colors[0],&frame,1,true,false);	// Desenha a faixa 1 na coluna 1
			draw_band_colors(colors[1],&frame,2,false,false);	// Desenha a faixa 2 na coluna 2
			draw_band_colors(colors[2],&frame,3,false,true);	// Desenha a faixa 3 (multiplicador) na coluna 3

			sleep_ms(700);
		} else {
            printf("Saindo para o modo de gravação...\n\n");

            ssd1306_fill(&ssd, false);  					// Limpa o display
            ssd1306_draw_string(&ssd, "MODO DE", 28, 28);	// Desenha uma string 
            ssd1306_draw_string(&ssd, "GRAVACAO", 24, 40);	// Desenha uma string
            ssd1306_send_data(&ssd);    					// Atualiza o display

			ws2812_clean(&frame);

            reset_usb_boot(0,0);    // Sai para o modo de gravação
        }
	}
}

// Handler de interrupção
void gpio_irq_handler(uint gpio, uint32_t events) {
	uint32_t current_time = to_us_since_boot(get_absolute_time());	// Obtém o tempo atual em microssegundos
  
    if (current_time - last_time > 2e5) {	// 200 ms de debouncing
		last_time = current_time;
		reset = true;
	}
}

//	Desenha uma coluna na matriz de leds conforme a cor obtida.
void draw_band_colors(char *color, Led_Matrix *frame, int column, bool clear, bool print) {
    if(strcmp(color, "Black") == 0 || strcmp(color, "--") == 0) 
        ws2812_draw_column(frame, 0, 0, 0, column, clear, print);
    else if(strcmp(color, "Brown") == 0) 
        ws2812_draw_column(frame, 0.35/8, 0.05/8, 0, column, clear, print);
    else if(strcmp(color, "Red") == 0) 
        ws2812_draw_column(frame, 1.0/7, 0, 0, column, clear, print);
    else if(strcmp(color, "Orange") == 0) 
        ws2812_draw_column(frame, 0.75/8, 0.15/8, 0, column, clear, print);
    else if(strcmp(color, "Yellow") == 0) 
        ws2812_draw_column(frame, 1.0/8, 1.0/8, 0, column, clear, print);
    else if(strcmp(color, "Green") == 0) 
        ws2812_draw_column(frame, 0, 1.0/8, 0, column, clear, print);
    else if(strcmp(color, "Blue") == 0) 
        ws2812_draw_column(frame, 0, 0, 1.0/8, column, clear, print);
    else if(strcmp(color, "Violet") == 0) 
        ws2812_draw_column(frame, 0.5/8, 0, 1.0/8, column, clear, print);
    else if(strcmp(color, "Gray") == 0) 
        ws2812_draw_column(frame, 0.5/8, 0.5/8, 0.5/8, column, clear, print);
    else if(strcmp(color, "White") == 0) 
        ws2812_draw_column(frame, 1.0/8, 1.0/8, 1.0/8, column, clear, print);
}