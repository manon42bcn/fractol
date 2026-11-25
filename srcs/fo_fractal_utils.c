/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_fractal_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 21:16:39 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:53:38 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Advance to the next classic color scheme and wrap at the end.
 * @param[in,out] set State containing the active color scheme.
 * @return Always 0.
 */
int	ft_load_base_color(t_set *set)
{
	set->color_scheme = (set->color_scheme + 1) % COLOR_SCHEME_COUNT;
	return (0);
}

/**
 * @brief Check whether an orbit point is nonfinite or beyond the threshold.
 * @param[in] point Current complex iterate.
 * @param[in] escape Squared escape threshold.
 * @return True if the point meets an escape condition.
 */
bool	ft_orbit_escaped(t_coor point, long double escape)
{
	return (!isfinite(point.a) || !isfinite(point.b)
		|| point.a * point.a + point.b * point.b > escape);
}

/**
 * @brief Represent a division by zero as an escape at infinity.
 * @param[in] n Iteration that reached the pole.
 * @return Escaped orbit result with infinite coordinates.
 */
t_orbit_result	ft_orbit_pole(int n)
{
	return ((t_orbit_result){
		.point = {INFINITY, INFINITY}, .n = n, .escaped = true});
}
