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

/**
 * @brief Look up the window title for a fractal.
 * @param[in] index Valid index in the fractal enumeration.
 * @return Pointer to the stored window title.
 */
const char	*ft_fractal_titles(int index)
{
	static const char	*fractals[] = {
		"fract-ol - mandelbrot set", "fract-ol -  julia set",
		"fract-ol - burningship set", "fract-ol - drop set",
		"fract-ol - bird set", "fract-ol - spider set",
		"fract-ol - worm set", "fract-ol - hiper set",
		"fract-ol - mandelbrot3 set", "fract-ol - mandelbrot7 set",
		"fract-ol - julia3 set", "fract-ol - julia7 set",
		"fract-ol - drop7 set", "fract-ol - tricorn set",
		"fract-ol - newton set", "fract-ol - phoenix set",
		"fract-ol - buddhabrot", "fract-ol - cos(c/z)",
		"fract-ol - cos(z/c)", "fract-ol - sin(z/sqrt(c))",
		"fract-ol - rational quadratic", "fract-ol - z^2 + z + c",
		"fract-ol - (1 + z) / z^2 + c", "fract-ol - z^-2 + c",
		"fract-ol - z^-5 + c", "fract-ol - z^2 + c / (z^2 + 0.1)",
		"fract-ol - z^2 + c / (z^2 + 0.001)",
		"fract-ol - ((z^2 + c - 1) / (2z + c - 2))^2",
		"fract-ol - z^4 + c", "fract-ol - z^3 - z + c",
		"fract-ol - z^2 + c + c^2", NULL
	};

	return (fractals[index]);
}
