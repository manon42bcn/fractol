/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_fractal_polynomial.c                            +:+      :+:    :+:   */
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
 * @brief Iterate z = z^4 + c from z = 0.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_mandelbrot_test4(const t_set *set)
{
	const long double complex	c = set->zr + set->zi * I;
	long double complex			z;
	long double complex			square;
	t_coor						point;
	int							n;

	z = 0;
	n = 1;
	while (n <= set->max_i)
	{
		square = z * z;
		z = square * square + c;
		point = (t_coor){creall(z), cimagl(z)};
		if (point.a * point.a + point.b * point.b > set->esc)
			return ((t_orbit_result){
				.point = point, .n = n, .escaped = true});
		n++;
	}
	return ((t_orbit_result){
		.point = {creall(z), cimagl(z)},
		.n = set->max_i, .escaped = false});
}

/**
 * @brief Iterate z = z^3 - z + c from z = 0.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_cubic_minus_linear(const t_set *set)
{
	const long double complex	c = set->zr + set->zi * I;
	long double complex			z;
	t_coor						point;
	int							n;

	z = 0;
	n = 1;
	while (n <= set->max_i)
	{
		z = z * z * z - z + c;
		point = (t_coor){creall(z), cimagl(z)};
		if (point.a * point.a + point.b * point.b > set->esc)
			return ((t_orbit_result){
				.point = point, .n = n, .escaped = true});
		n++;
	}
	return ((t_orbit_result){
		.point = {creall(z), cimagl(z)},
		.n = set->max_i, .escaped = false});
}

/**
 * @brief Iterate z = z^2 + c + c^2 with the constant precomputed.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_quadratic_c_plus_c_squared(const t_set *set)
{
	const long double complex	c = set->zr + set->zi * I;
	const long double complex	parameter = c + c * c;
	long double complex			z;
	t_coor						point;
	int							n;

	z = 0;
	n = 1;
	while (n <= set->max_i)
	{
		z = z * z + parameter;
		point = (t_coor){creall(z), cimagl(z)};
		if (point.a * point.a + point.b * point.b > set->esc)
			return ((t_orbit_result){
				.point = point, .n = n, .escaped = true});
		n++;
	}
	return ((t_orbit_result){
		.point = {creall(z), cimagl(z)},
		.n = set->max_i, .escaped = false});
}
