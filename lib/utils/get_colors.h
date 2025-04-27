/*
    Lucas Carneiro de Araújo Lima
*/

#ifndef GET_COLORS_H
#define GET_COLORS_H

#include <math.h>

char **get_colors(float E24_value) {
	const char *colors[] = {
		"Black", "Brown", "Red", "Orange", "Yellow",
		"Green", "Blue", "Violet", "Gray", "White",
	};
	const int decades = 8;
	static char *bands[3];
	int multiplier = 0;

	if (E24_value < 10)
	{
		bands[0] = (char *)colors[((int)(E24_value * 10)) / 10];
		bands[1] = (char *)colors[((int)(E24_value * 10)) % 10];
		bands[2] = "Golden";

		return bands;
	}

	int value = (int)E24_value;

	for (int decade = 1; decade < decades; decade++)
	{
		multiplier = (int)pow(10, decade);
		if (E24_value < 10 * multiplier)
		{
			bands[0] = (char *)colors[value / multiplier];
			bands[1] = (char *)colors[(value / (multiplier / 10)) % 10];
			bands[2] = (char *)colors[decade - 1];
			return bands;
		}
	}
}

#endif