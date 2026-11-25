/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_fractal_cosine.c                                +:+      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 19:47:42 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 19:47:42 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include <complex.h>

/**
 * @brief Iterate a complex cosine ratio from z = 1.
 * @param[in] set Pixel coordinates and orbit settings.
 * @param[in] c_over_z True for cos(c / z); false for cos(z / c).
 * @return Final orbit point, iteration count, and escape status.
 */
static t_orbit_result	ft_cosine_test(const t_set *set, bool c_over_z)
{
	const long double complex	c = set->zr + set->zi * I;
	long double complex			z;
	int							n;

	z = 1;
	n = 1;
	while (n <= set->max_i)
	{
		if (c_over_z && z != 0)
			z = ccosl(c / z);
		else if (!c_over_z && c != 0)
			z = ccosl(z / c);
		else
			return (ft_orbit_pole(n));
		if (ft_orbit_escaped((t_coor){creall(z), cimagl(z)}, set->esc))
			return ((t_orbit_result){.point = {creall(z), cimagl(z)},
				.n = n, .escaped = true});
		n++;
	}
	return ((t_orbit_result){
		.point = {creall(z), cimagl(z)},
		.n = set->max_i, .escaped = false});
}

/**
 * @brief Iterate z = cos(c / z) from z = 1.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_cos_c_over_z(const t_set *set)
{
	return (ft_cosine_test(set, true));
}

/**
 * @brief Iterate z = cos(z / c) from z = 1.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_cos_z_over_c(const t_set *set)
{
	return (ft_cosine_test(set, false));
}
