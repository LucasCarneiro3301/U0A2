/*
    Lucas Carneiro de Araújo Lima
*/

#ifndef GET_COLORS_H
#define GET_COLORS_H

#include <math.h>

// Função para retornar as cores das faixas de um resistor com base no valor E24
char **get_colors(float E24_value) {
	const char *colors[] = {
		"Black", "Brown", "Red", "Orange", "Yellow",
		"Green", "Blue", "Violet", "Gray", "White",
	};
	const int decades = 8;  // Número de décadas (faixas de potências de 10)
	static char *bands[3];  // Array para armazenar as três faixas de cores (1ª, 2ª e multiplicador)
	int multiplier = 0;

	if (E24_value < 10)     // Se o valor E24 for menor que 10, o resistor tem a cor "Golden" na 3ª faixa
	{
		bands[0] = (char *)colors[((int)(E24_value * 10)) / 10];    // 1ª faixa
		bands[1] = (char *)colors[((int)(E24_value * 10)) % 10];    // 2ª faixa
		bands[2] = "Golden";                                        // 3ª faixa

		return bands;
	}

	int value = (int)E24_value;

	for (int decade = 1; decade < decades; decade++)    // Itera as "décadas" (potências de 10) para obter as faixas de cores
	{
		multiplier = (int)pow(10, decade);
		if (E24_value < 10 * multiplier)
		{
			bands[0] = (char *)colors[value / multiplier];                  // Obtém o 1° dígito => cor da 1º faixa
			bands[1] = (char *)colors[(value / (multiplier / 10)) % 10];    // Obtém o 3° dígito => cor da 2º faixa
			bands[2] = (char *)colors[decade - 1];                          // Obtém a década => cor da 3º faixa
			return bands;
		}
	}
}

#endif