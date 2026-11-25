/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_fractal_tricorn.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 15:51:20 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:53:43 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Iterate the conjugate-square map and color the pixel.
 * @param[in] set Orbit iteration limit.
 * @param[in] point Initial complex iterate.
 * @param[in] c Constant added each iteration.
 * @param[in] escape Squared escape threshold.
 * @return Final orbit point, iteration count, and escape status.
 */
static t_orbit_result	ft_tricorn_orbit(const t_set *set, t_coor point,
		t_coor c, long double escape)
{
	long double	next_a;
	int			n;

	n = 1;
	while (n < set->max_i)
	{
		next_a = point.a * point.a - point.b * point.b + c.a;
		point.b = -2 * point.a * point.b + c.b;
		point.a = next_a;
		if (point.a * point.a + point.b * point.b > escape)
			return ((t_orbit_result){
				.point = point, .n = n, .escaped = true});
		n++;
	}
	return ((t_orbit_result){
		.point = point, .n = n, .escaped = false});
}

/**
 * @brief Select the Tricorn or anti-Julia orbit for the current pixel.
 * @param[in] set Pixel coordinates, selected constant, and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_tricorn_test(const t_set *set)
{
	t_coor		point;
	t_coor		c;
	long double	escape;

	point = (t_coor){0, 0};
	c = (t_coor){set->zr, set->zi};
	escape = set->esc;
	if (set->tricorn_julia)
	{
		point = (t_coor){set->zr, set->zi};
		c = (t_coor){set->c_r, set->c_i};
		escape = fmaxl(escape, c.a * c.a + c.b * c.b);
	}
	return (ft_tricorn_orbit(set, point, c, escape));
}
