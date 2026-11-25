/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_fractal_phoenix.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 15:51:20 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:53:43 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Compute the next Phoenix iterate using two successive points.
 * @param[in] point Current orbit point.
 * @param[in] previous Previous orbit point.
 * @param[in] set Selected Phoenix constant.
 * @return Next complex iterate.
 */
static t_coor	ft_phoenix_step(t_coor point, t_coor previous, const t_set *set)
{
	t_coor	next;

	next.a = point.a * point.a - point.b * point.b
		+ set->c_r - 0.5L * previous.a;
	next.b = 2 * point.a * point.b + set->c_i - 0.5L * previous.b;
	return (next);
}

/**
 * @brief Check the escape bound for a growing Phoenix orbit.
 * @param[in] point Current orbit point.
 * @param[in] previous Previous orbit point.
 * @param[in] constant_bound Sum of the absolute constant components.
 * @param[in] escape Squared escape threshold.
 * @return True when the orbit meets the growth and magnitude criteria.
 */
static bool	ft_phoenix_escaped(t_coor point, t_coor previous,
		long double constant_bound, long double escape)
{
	long double	norm;

	norm = point.a * point.a + point.b * point.b;
	return (norm > escape && norm >= previous.a * previous.a
		+ previous.b * previous.b && (constant_bound <= 1 || isinf(norm)
			|| norm - 1.5L * sqrtl(norm) > constant_bound));
}

/**
 * @brief Iterate the Phoenix map using two successive orbit points.
 * @param[in] set Pixel coordinates, Phoenix constant, and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_phoenix_test(const t_set *set)
{
	t_coor		point;
	t_coor		previous;
	t_coor		next;
	long double	constant_bound;
	int			n;

	point = (t_coor){set->zr, set->zi};
	previous = (t_coor){0, 0};
	constant_bound = fabsl(set->c_r) + fabsl(set->c_i);
	n = 0;
	while (n < set->max_i)
	{
		next = ft_phoenix_step(point, previous, set);
		previous = point;
		point = next;
		n++;
		if (ft_phoenix_escaped(point, previous, constant_bound, set->esc))
			return ((t_orbit_result){
				.point = point, .n = n, .escaped = true});
	}
	return ((t_orbit_result){
		.point = point, .n = n, .escaped = false});
}
