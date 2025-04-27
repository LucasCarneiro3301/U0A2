/*
    Lucas Carneiro de Araújo Lima
*/

#ifndef GET_24_VALUE_H
#define GET_24_VALUE_H

#include <math.h>

// Função para calcular o valor E24 mais próximo de uma resistência fornecida.
float get_E24_value(float rx) {
	const float E24_factors[] = {
		1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0,
		2.2, 2.4, 2.7, 3.0, 3.3, 3.6, 3.9, 4.3,
		4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1
	};
	const int decades = 8;
	float last_diff = 1e9, cur_diff = 0.0, cur_value = 0.0, multiplier = 0.0;
	float E24_value = 0.0;

	for (int decade = 0; decade < decades; decade++)    // Percorre as décadas de resistores (1, 10, 100, 1000, etc...).
	{
		multiplier = powf(10, decade);
		for (int i = 0; i < 24; i++)                    // Percorre os fatores da série E24
		{
			cur_value = E24_factors[i] * multiplier;    // Multiplica o fator da série E24 pelo multiplicador da década.
			cur_diff = fabsf(cur_value - rx);           // Calcula a diferença absoluta entre o valor atual e o valor fornecido (rx).

			if (cur_diff < last_diff)                   // Se a diferença atual for menor que a diferença anterior, atualiza o valor E24 mais próximo.
			{
				last_diff = cur_diff;
				E24_value = cur_value;
			}
		}
	}

	return E24_value;
}

#endif