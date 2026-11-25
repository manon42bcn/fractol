/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_mandelbrot_distance.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 22:56:31 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 22:56:31 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Differentiate the quadratic Mandelbrot step with respect to c.
 * @param[in] point Current orbit point.
 * @param[in] derivative Current derivative with respect to c.
 * @return Derivative of the next iterate.
 */
static t_coor	ft_mandelbrot_derivative(t_coor point, t_coor derivative)
{
	return ((t_coor){
		2 * (point.a * derivative.a - point.b * derivative.b) + 1,
		2 * (point.a * derivative.b + point.b * derivative.a)});
}

/**
 * @brief Estimate the distance from an escaped point to the set boundary.
 * @param[in] point Complex iterate at escape.
 * @param[in] derivative Derivative at escape.
 * @return Distance in complex-plane units, or zero when unavailable.
 */
static long double	ft_mandelbrot_distance(t_coor point, t_coor derivative)
{
	long double	magnitude;
	long double	slope;

	magnitude = hypotl(point.a, point.b);
	slope = hypotl(derivative.a, derivative.b);
	if (magnitude <= 1 || slope <= 0
		|| !isfinite(magnitude) || !isfinite(slope))
		return (0);
	return (magnitude * logl(magnitude) / slope);
}

/**
 * @brief Run the Mandelbrot orbit with an exterior distance estimate.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit result with distance when the point escaped.
 */
t_orbit_result	ft_mandelbrot_distance_test(const t_set *set)
{
	t_coor		point;
	t_coor		derivative;
	long double	next_a;
	int			n;

	point = (t_coor){set->zr, set->zi};
	derivative = (t_coor){1, 0};
	n = 1;
	while (n < set->max_i)
	{
		derivative = ft_mandelbrot_derivative(point, derivative);
		next_a = (point.a * point.a - point.b * point.b) + set->zr;
		point.b = 2 * point.a * point.b + set->zi;
		point.a = next_a;
		if (point.a * point.a + point.b * point.b > set->esc)
			return ((t_orbit_result){.point = point, .n = n, .escaped = true,
				.distance = ft_mandelbrot_distance(point, derivative)});
		n++;
	}
	return ((t_orbit_result){.point = point, .n = n});
}

/**
 * @brief Darken colors near the boundary over a fixed pixel distance.
 * @param[in] set Current view and texture dimensions.
 * @param[in] color Unshaded packed RGB color.
 * @param[in] distance Estimated distance in complex-plane units.
 * @return Packed color after adjusting brightness in linear light.
 */
int	ft_shade_distance(const t_set *set, int color, long double distance)
{
	long double	pixel_width;
	long double	factor;

	pixel_width = fmaxl(set->viewer->x_width / set->screen_w,
			set->viewer->y_height / set->screen_h);
	if (!isfinite(distance) || distance <= 0 || pixel_width <= 0
		|| distance >= pixel_width)
		return (color);
	factor = fmaxl(0, 1 + log2l(distance / pixel_width) / 3);
	return (ft_rgb_lerp_linear(0, color, 0.25L + 0.75L * factor));
}
