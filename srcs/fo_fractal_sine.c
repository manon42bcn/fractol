/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_fractal_sine.c                                  +:+      :+:    :+:   */
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
 * @brief Iterate z = sin(z / sqrt(c)) using the principal square root.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_sin_z_over_sqrt_c(const t_set *set)
{
	const long double complex	root = csqrtl(set->zr + set->zi * I);
	long double complex			z;
	int							n;

	if (root == 0)
		return (ft_orbit_pole(1));
	z = 1;
	n = 1;
	while (n <= set->max_i)
	{
		z = csinl(z / root);
		if (ft_orbit_escaped((t_coor){creall(z), cimagl(z)}, set->esc))
			return ((t_orbit_result){.point = {creall(z), cimagl(z)},
				.n = n, .escaped = true});
		n++;
	}
	return ((t_orbit_result){
		.point = {creall(z), cimagl(z)},
		.n = set->max_i, .escaped = false});
}
