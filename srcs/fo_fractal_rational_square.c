/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_fractal_rational_square.c                       +:+      :+:    :+:   */
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
 * @brief Iterate the squared rational quotient from z = 0.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_rational_square(const t_set *set)
{
	const long double complex	c = set->zr + set->zi * I;
	long double complex			z;
	long double complex			denominator;
	long double complex			quotient;
	int							n;

	z = 0;
	n = 1;
	while (n <= set->max_i)
	{
		denominator = 2 * z + c - 2;
		if (denominator == 0)
			return (ft_orbit_pole(n));
		quotient = (z * z + c - 1) / denominator;
		z = quotient * quotient;
		if (ft_orbit_escaped((t_coor){creall(z), cimagl(z)}, set->esc))
			return ((t_orbit_result){.point = {creall(z), cimagl(z)},
				.n = n, .escaped = true});
		n++;
	}
	return ((t_orbit_result){
		.point = {creall(z), cimagl(z)},
		.n = set->max_i, .escaped = false});
}
