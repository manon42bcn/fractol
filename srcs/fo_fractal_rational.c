/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_fractal_rational.c                              +:+      :+:    :+:   */
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
 * @brief Iterate z = z^2 + c^2 / (z^2 + c) + c from z = 0.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_rational_quadratic(const t_set *set)
{
	const long double complex	c = set->zr + set->zi * I;
	const long double complex	c_squared = c * c;
	long double complex			z;
	long double complex			denominator;
	int							n;

	z = 0;
	n = 1;
	while (n <= set->max_i)
	{
		denominator = z * z + c;
		if (denominator == 0)
			return (ft_orbit_pole(n));
		z = denominator + c_squared / denominator;
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
 * @brief Iterate z = (1 + z) / z^2 + c from z = -2.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_rational_reciprocal(const t_set *set)
{
	const long double complex	c = set->zr + set->zi * I;
	long double complex			z;
	long double complex			denominator;
	int							n;

	z = -2;
	n = 1;
	while (n <= set->max_i)
	{
		denominator = z * z;
		if (denominator == 0)
			return (ft_orbit_pole(n));
		z = (1 + z) / denominator + c;
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
 * @brief Iterate an inverse power of z plus c from z = 1.
 * @param[in] set Pixel coordinates and orbit settings.
 * @param[in] exponent Magnitude of the negative exponent, either 2 or 5.
 * @return Final orbit point, iteration count, and escape status.
 */
static t_orbit_result	ft_inverse_power(const t_set *set, int exponent)
{
	const long double complex	c = set->zr + set->zi * I;
	long double complex			z;
	long double complex			inverse;
	long double complex			power;
	int							n;

	z = 1;
	n = 1;
	while (n <= set->max_i)
	{
		if (z == 0)
			return (ft_orbit_pole(n));
		inverse = 1 / z;
		power = inverse * inverse;
		if (exponent == 5)
			power = power * power * inverse;
		z = power + c;
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
 * @brief Iterate z = z^-2 + c from z = 1.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_inverse_square(const t_set *set)
{
	return (ft_inverse_power(set, 2));
}

/**
 * @brief Iterate z = z^-5 + c from z = 1.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_inverse_fifth(const t_set *set)
{
	return (ft_inverse_power(set, 5));
}
