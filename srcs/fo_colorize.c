/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_colorize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 20:49:10 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:53:49 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Compute a fractional escape count for supported quadratic maps.
 * @param[in] set Fractal type and squared escape threshold.
 * @param[in] point Complex coordinate at escape.
 * @param[in] n Integer escape iteration.
 * @return Smoothed count, or the original count when smoothing does not apply.
 */
static double	ft_color_iterations(const t_set *set, t_coor point, int n)
{
	long double	norm;

	if (set->fractal != MALDELBROT && set->fractal != JULIA
		&& set->fractal != TRICORN && set->fractal != BURNINGSHIP)
		return (n);
	norm = point.a * point.a + point.b * point.b;
	if (!isfinite(norm) || norm <= 1 || set->esc <= 1)
		return (n);
	return (fmax(0, n + 1 - log2l(logl(norm) / logl(set->esc))));
}

/**
 * @brief Map a final orbit coordinate to a channel in full color mode.
 * @param[in] coordinate Real or imaginary orbit coordinate.
 * @return Channel intensity in the range 0 to 255.
 */
static int	ft_full_channel(long double coordinate)
{
	if (isnan(coordinate))
		return (0);
	if (fabsl(coordinate) <= 1)
		return (255);
	return ((int)(255 / fabsl(coordinate) / fabsl(coordinate)));
}

/**
 * @brief Shade the monochrome tint by normalized escape time.
 * @param[in] scheme Index of the active color scheme.
 * @param[in] fraction Escape time normalized to the iteration limit.
 * @return Packed monochrome color.
 */
static int	ft_mono_color(int scheme, double fraction)
{
	static const Uint32	tints[COLOR_SCHEME_COUNT] = {
		0x16213d, 0x7a2d3f, 0xa3a09f, 0x87706b, 0x695c52, 0x7b161a
	};
	Uint32				tint;

	tint = tints[scheme];
	fraction = 0.25 + 0.75 * fraction;
	return (create_trgb(0, (int)((tint >> 16 & 255) * fraction),
		(int)((tint >> 8 & 255) * fraction),
		(int)((tint & 255) * fraction)));
}

/**
 * @brief Choose the root hue and dim it according to convergence time.
 * @param[in] root Zero-based index of the converged root.
 * @param[in] iterations Iterations required to converge.
 * @return Packed color for the root and convergence time.
 */
static int	ft_newton_color(int root, int iterations)
{
	int	light;

	light = (int)(255 / (1 + 0.08 * iterations));
	if (root == 0)
		return (create_trgb(0, light, light / 4, light / 3));
	if (root == 1)
		return (create_trgb(0, light / 5, light, light / 2));
	return (create_trgb(0, light / 4, light / 2, light));
}

/**
 * @brief Convert an orbit result to a pixel color using the active color mode.
 * @param[in] set Active fractal and color settings.
 * @param[in] result Final coordinate and escape or convergence data.
 * @return Packed pixel color.
 */
int	ft_colorize_pixel(const t_set *set, t_orbit_result result)
{
	double	iterations;
	double	fraction;
	int		level;

	if (result.basin > 0)
		return (ft_newton_color(result.basin - 1, result.n));
	if (!result.escaped)
		return (0);
	iterations = ft_color_iterations(set, result.point, result.n);
	fraction = fmin(iterations / set->max_i, 1.0);
	level = 32 + (int)(223 * fraction);
	if (set->color_mode == 2)
		return (ft_mono_color(set->color_scheme, fraction));
	if (set->color_mode == 3)
		return (create_trgb(0, level, level, level));
	if (set->color_mode == 4)
		return (create_trgb((int)(224 * fraction),
			ft_full_channel(result.point.a),
			ft_full_channel(result.point.b), level));
	return (ft_scheme_color(set, iterations));
}
