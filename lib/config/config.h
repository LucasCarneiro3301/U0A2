/*
	Lucas Carneiro de Araújo Lima
*/

#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "pico/bootrom.h"
#include "../ssd1306/ssd1306.h"
#include "../ssd1306/font.h"

#define I2C_PORT i2c1   // Define que o barramento I2C usado será o "i2c1"
#define I2C_SDA 14      // Define que o pino GPIO 14 será usado como SDA (linha de dados do I2C)
#define I2C_SCL 15      // Define que o pino GPIO 15 será usado como SCL (linha de clock do I2C)
#define address 0x3C    // Define o endereço I2C do dispositivo (0x3C é o endereço padrão de muitos displays OLED SSD1306)
#define BTNA 5          // Botão A
#define BTNB 6          // Botão B
#define ADC 28          // GPIO para o ohmimetro


// Inicializa e configura os LEDs RGB como saída. Inicializa e configura os botões como entradas.
void io_setup() {
    gpio_init(BTNB);
    gpio_set_dir(BTNB, GPIO_IN);
    gpio_pull_up(BTNB);  
}

// Inicializa e configura a comunicação serial I2C 
void i2c_setup() {
    i2c_init(I2C_PORT, 4e2 * 1e3);              // Inicialização I2C.
  
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);  // Define a função do pino GPIO para I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);  // Define a função do pino GPIO para I2C
    gpio_pull_up(I2C_SDA);                      // Pull up para linha de dados
    gpio_pull_up(I2C_SCL);                      // Pull up para linha de clock
}

// Limpa o display
void clear(ssd1306_t* ssd) {
    ssd1306_fill(ssd, false);
    ssd1306_send_data(ssd);
}

// Inicializa e configura o display
void ssd1306_setup(ssd1306_t* ssd) {
    ssd1306_init(ssd, WIDTH, HEIGHT, false, address, I2C_PORT); 
    ssd1306_config(ssd); 
    ssd1306_send_data(ssd);
    clear(ssd);

  
    ssd1306_draw_char(ssd, '#', 28, 60);  
    ssd1306_send_data(ssd);
}

// Inicializa e configura o pino 28 como periférico ADC
void adc_setup() {
    adc_init();
    adc_gpio_init(ADC);
}

void config(ssd1306_t *ssd) {
    stdio_init_all();
    io_setup();
    i2c_setup();
    ssd1306_setup(ssd);
    adc_setup();
}

#endif