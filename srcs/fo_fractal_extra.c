/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_fractal_extra.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 15:51:20 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:53:43 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Iterate z = z^3 + c for the current pixel.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_mandelbrot_test3(const t_set *set)
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
		t_a = (a * a * a - 3 * a * (b * b)) + set->zr;
		b = (3 * (a * a) * b - b * b * b) + set->zi;
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
 * @brief Iterate the cubic Julia variant using the selected constant components.
 * @param[in] set Pixel coordinates, Julia constant, and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_julia_test3(const t_set *set)
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
		t_a = (a * a * a - 3 * a * (b * b)) + set->c_i;
		b = (3.0 * (a * a) * b - b * b * b) + set->c_r;
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
