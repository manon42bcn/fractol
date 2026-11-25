/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_fractal_tests.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 15:51:20 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:53:43 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Iterate z = z^2 + z + c from z = 0.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_quadratic_linear(const t_set *set)
{
	long double	a;
	long double	b;
	long double	next_a;
	int			n;

	a = 0;
	b = 0;
	n = 1;
	while (n <= set->max_i)
	{
		next_a = a * a - b * b + a + set->zr;
		b = 2 * a * b + b + set->zi;
		a = next_a;
		if (a * a + b * b > set->esc)
			return ((t_orbit_result){
				.point = {a, b}, .n = n, .escaped = true});
		n++;
	}
	return ((t_orbit_result){
		.point = {a, b}, .n = set->max_i, .escaped = false});
}

/**
 * @brief Iterate the quadratic Mandelbrot map for the current pixel.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_mandelbrot_test(const t_set *set)
{
	int			n;
	long double	a;
	long double	b;
	long double	t_a;

	n = 1;
	a = set->zr;
	b = set->zi;
	while (n < set->max_i)
	{
		t_a = (a * a - b * b) + set->zr;
		b = 2 * a * b + set->zi;
		a = t_a;
		if (a * a + b * b > set->esc)
			return ((t_orbit_result){
				.point = {a, b}, .n = n, .escaped = true});
		n++;
	}
	return ((t_orbit_result){
		.point = {a, b}, .n = n, .escaped = false});
}

/**
 * @brief Iterate the quadratic Julia map using the selected complex constant.
 * @param[in] set Pixel coordinates, Julia constant, and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_julia_test(const t_set *set)
{
	int		n;
	double	a;
	double	b;
	double	t_a;

	n = 0;
	a = set->zr;
	b = set->zi;
	while (n < set->max_i)
	{
		t_a = (a * a - b * b) + set->c_i;
		b = 2.0 * a * b + set->c_r;
		a = t_a;
		if (a * a + b * b > set->esc)
			return ((t_orbit_result){
				.point = {a, b}, .n = n, .escaped = true});
		if (t_a == b)
			n = set->max_i - 1;
		n++;
	}
	return ((t_orbit_result){
		.point = {a, b}, .n = n, .escaped = false});
}

/**
 * @brief Iterate the Burning Ship map using absolute coordinate components.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_burningship_test(const t_set *set)
{
	int			n;
	long double	a;
	long double	b;
	long double	t_a;

	n = 1;
	a = set->zr;
	b = set->zi;
	while (n < set->max_i)
	{
		t_a = (a * a - b * b) + set->zr;
		if (2.0 * a * b < 0)
			b = (2.0 * a * b) * -1;
		else
			b = 2.0 * a * b;
		b += set->zi;
		a = t_a;
		if (a * a + b * b > set->esc)
			return ((t_orbit_result){
				.point = {a, b}, .n = n, .escaped = true});
		n++;
	}
	return ((t_orbit_result){
		.point = {a, b}, .n = n, .escaped = false});
}
