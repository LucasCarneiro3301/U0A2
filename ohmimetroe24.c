/*
	Lucas Carneiro de Araújo Lima
*/

#include "lib/config/config.h"
#include "lib/utils/get_E24_value.h"
#include "lib/utils/get_colors.h"

const int standard_resistor = 10e3;	// Resistor de 10k ohm
const int adc_resolution = 4095; 	// Resolução do ADC (12 bits)

bool reset = false;

static volatile uint32_t last_time = 0; // Armazena o tempo do último evento (em microssegundos)

void gpio_irq_handler(uint gpio, uint32_t events);

int main() {
	ssd1306_t ssd;	// Inicializa a estrutura do display
	char strx[5], str_y[5], str_e[5]; // Buffer para armazenar a string
	bool cor = true;

	config(&ssd);	// Inicializa e configura todos os componentes

	gpio_set_irq_enabled_with_callback(BTNB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);	// Callback para BTN B

	while (true) {
		if(!reset) {
			adc_select_input(2); // Seleciona o ADC para eixo X. O pino 28 como entrada analógica

			float sum = 0.0f;	// Registra a soma de todos os níveis ADC calculados em 500 iterações
			for (int i = 0; i < 500; i++) {
				sum += adc_read();
				sleep_ms(1);
			}

			float avg = sum / 500.0f; // Média aritmética para 500 iterações

			float rx = (standard_resistor * avg) / (adc_resolution - avg); // Calcula o valor do resistor desconhecido

			float rc = get_E24_value(rx);

			float error = (fabsf(rx - rc) / rx) * 100;

			char **cores = get_colors(rc);

			printf("Valor calculado: %1.0f ||| Valor E24: %1.0f || Erro: %.2f\n\n", rx, rc, error);
			printf("Faixa 1: %s  ", cores[0]);
			printf("Faixa 2: %s  ", cores[1]);
			printf("Multiplicador: %s\n\n", cores[2]);

			sprintf(strx, "%1.0f", avg);						   // Converte em string
			sprintf(str_y, (rc < 10) ? "%.1f" : "%1.0f", rc);	   // Converte em string
			sprintf(str_e, (error < 10) ? "%.2f" : "%.1f", error); // Converte em string

			ssd1306_fill(&ssd, !cor);						// Limpa o display
			ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);	// Desenha um retângulo
			ssd1306_line(&ssd, 3, 37, 123, 37, cor);		// Desenha uma linha
			ssd1306_draw_string(&ssd, "Erro", 8, 6);		// Desenha uma string
			ssd1306_draw_string(&ssd, "(%)", 12, 16);		// Desenha uma string
			ssd1306_draw_string(&ssd, str_e, 8, 26);		// Desenha uma string
			ssd1306_draw_string(&ssd, "1:", 50, 6);			// Desenha uma string
			ssd1306_draw_string(&ssd, "2:", 50, 16);		// Desenha uma string
			ssd1306_draw_string(&ssd, "3:", 50, 26);		// Desenha uma string
			ssd1306_draw_string(&ssd, cores[0], 70, 6);		// Desenha uma string
			ssd1306_draw_string(&ssd, cores[1], 70, 16);	// Desenha uma string
			ssd1306_draw_string(&ssd, cores[2], 70, 26);	// Desenha uma string
			ssd1306_draw_string(&ssd, "ADC", 13, 41);		// Desenha uma string
			ssd1306_draw_string(&ssd, "Resisten.", 50, 41); // Desenha uma string
			ssd1306_line(&ssd, 44, 3, 44, 61, cor);			// Desenha uma linha vertical
			ssd1306_draw_string(&ssd, strx, 8, 52);			// Desenha uma string
			ssd1306_draw_string(&ssd, str_y, 59, 52);		// Desenha uma string
			ssd1306_send_data(&ssd);						// Atualiza o display
			sleep_ms(700);
		} else {
            printf("Saindo para o modo de gravação...\n\n");

            ssd1306_fill(&ssd, false);  					// Limpa o display
            ssd1306_draw_string(&ssd, "MODO DE", 28, 28);	// Desenha uma string 
            ssd1306_draw_string(&ssd, "GRAVACAO", 24, 40);	// Desenha uma string
            ssd1306_send_data(&ssd);    					// Atualiza o display

            reset_usb_boot(0,0);    // Sai para o modo de gravação
        }
	}
}

void gpio_irq_handler(uint gpio, uint32_t events) {
	uint32_t current_time = to_us_since_boot(get_absolute_time()); // Obtém o tempo atual em microssegundos
  
    if (current_time - last_time > 2e5) { // 200 ms de debouncing
		last_time = current_time;
		reset = true;
	}
}