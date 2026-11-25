/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_fractal_rational_offset.c                       +:+      :+:    :+:   */
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
 * @brief Iterate z = z^2 + c / (z^2 + offset) from z = 0.
 * @param[in] set Pixel coordinates and orbit settings.
 * @param[in] offset Positive constant added to the denominator.
 * @return Final orbit point, iteration count, and escape status.
 */
static t_orbit_result	ft_rational_offset(const t_set *set, long double offset)
{
	const long double complex	c = set->zr + set->zi * I;
	long double complex			z;
	long double complex			z_squared;
	long double complex			denominator;
	int							n;

	z = 0;
	n = 1;
	while (n <= set->max_i)
	{
		z_squared = z * z;
		denominator = z_squared + offset;
		if (denominator == 0)
			return (ft_orbit_pole(n));
		z = z_squared + c / denominator;
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
 * @brief Iterate the rational offset map with denominator constant 0.1.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_rational_offset_01(const t_set *set)
{
	return (ft_rational_offset(set, 0.1L));
}

/**
 * @brief Iterate the rational offset map with denominator constant 0.001.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_rational_offset_0001(const t_set *set)
{
	return (ft_rational_offset(set, RATIONAL_SMALL_OFFSET));
}
