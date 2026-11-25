/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_smooth_color.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 22:56:31 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 22:56:31 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Return the degree used for escape-time smoothing.
 * @param[in] fractal Active fractal index.
 * @return Polynomial degree, or zero for maps without smoothing.
 */
static int	ft_smoothing_degree(int fractal)
{
	if (fractal == MANDELBROT7 || fractal == JULIA7 || fractal == DROP7)
		return (7);
	if (fractal == MANDELBROT4)
		return (4);
	if (fractal == MANDELBROT3 || fractal == JULIA3
		|| fractal == CUBIC_MINUS_LINEAR)
		return (3);
	if (fractal == MALDELBROT || fractal == JULIA || fractal == TRICORN
		|| fractal == BURNINGSHIP || fractal == DROP
		|| fractal == QUADRATIC_LINEAR || fractal == QUADRATIC_C_PLUS_C_SQUARED)
		return (2);
	return (0);
}

/**
 * @brief Compute a fractional escape count for a supported polynomial map.
 * @param[in] set Fractal type and squared escape threshold.
 * @param[in] point Complex coordinate at escape.
 * @param[in] n Integer escape iteration.
 * @return Smoothed count, or the original count when smoothing cannot apply.
 */
double	ft_smooth_iterations(const t_set *set, t_coor point, int n)
{
	long double	norm;
	int			degree;

	degree = ft_smoothing_degree(set->fractal);
	norm = point.a * point.a + point.b * point.b;
	if (!degree || !isfinite(norm) || norm <= 1 || set->esc <= 1
		|| !isfinite(set->esc))
		return (n);
	return (fmax(0, n + 1
			- log2l(logl(norm) / logl(set->esc)) / log2l(degree)));
}
