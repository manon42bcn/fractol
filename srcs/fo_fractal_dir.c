/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_fractal_dir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 21:16:39 by mporras-          #+#    #+#             */
/*   Updated: 2024/09/23 00:15:22 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	ft_fractal_dir(char *s)
{
	static char	*fractals[] = {
		"mandelbrot", "julia", "burningship",
		"drop", "bird", "spider",
		"worm", "hiper","mandelbrot3",
        "mandelbrot7", "julia3","julia7",
        "drop7", NULL};
	int i;

    i = 0;
	while (fractals[i])
	{

		if (ft_strict_cmp(fractals[i], s) == 0)
			return (i);
		i++;
	}
	return (NO_FRACTAL);
}

char *ft_fractal_titles(int index)
{
	static char *fractals[] = {
			"fract-ol - mandelbrot set", "fract-ol -  julia set",
			"fract-ol - burningship set", "fract-ol - drop set",
			"fract-ol - bird set", "fract-ol - spider set",
			"fract-ol - worm set", "fract-ol - hiper set",
			"fract-ol - mandelbrot3 set", "fract-ol - mandelbrot7 set",
			"fract-ol - julia3 set", "fract-ol - julia7 set",
			"fract-ol - drop7 set", NULL};

	return (fractals[index]);
}