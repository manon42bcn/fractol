/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_fractal_degree7.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 15:51:20 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:53:43 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Compute one seventh-power Mandelbrot step.
 * @param[in] point Current complex iterate.
 * @param[in] set Pixel coordinate used as the complex constant.
 * @return Next complex iterate.
 */
static t_coor	ft_mandelbrot_step7(t_coor point, const t_set *set)
{
	long double	a2;
	long double	b2;
	t_coor		next;

	a2 = point.a * point.a;
	b2 = point.b * point.b;
	next.a = point.a * (((a2 - 21 * b2) * a2 + 35 * b2 * b2) * a2
			- 7 * b2 * b2 * b2) + set->zr;
	next.b = point.b * (((7 * a2 - 35 * b2) * a2 + 21 * b2 * b2) * a2
			- b2 * b2 * b2) + set->zi;
	return (next);
}

/**
 * @brief Iterate z = z^7 + c from z = 0.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_mandelbrot_test7(const t_set *set)
{
	t_coor	point;
	int		n;

	point = (t_coor){0, 0};
	n = 1;
	while (n <= set->max_i)
	{
		point = ft_mandelbrot_step7(point, set);
		if (point.a * point.a + point.b * point.b > set->esc)
			return ((t_orbit_result){
				.point = point, .n = n, .escaped = true});
		n++;
	}
	return ((t_orbit_result){
		.point = point, .n = set->max_i, .escaped = false});
}

/**
 * @brief Compute one seventh-power Julia step with double orbit components.
 * @param[in] a Real component of the current iterate.
 * @param[in] b Imaginary component of the current iterate.
 * @param[in] set Selected Julia constant.
 * @return Next complex iterate, rounded to double precision per component.
 */
static t_coor	ft_julia_step7(double a, double b, const t_set *set)
{
	double	next_a;
	double	next_b;

	next_a = (a * a * a * a * a * a * a)
		- (21 * ((a * a * a * a * a) * (b * b)))
		+ (35 * ((a * a * a) * (b * b * b * b)))
		- (7 * a * (b * b * b * b * b * b)) + set->c_i;
	next_b = 7 * b * (a * a * a * a * a * a)
		- (35 * ((a * a * a * a) * (b * b * b)))
		+ (21 * ((a * a) * (b * b * b * b * b)))
		- (b * b * b * b * b * b * b) + set->c_r;
	return ((t_coor){next_a, next_b});
}

/**
 * @brief Iterate the seventh-power Julia map with the selected constant.
 * @param[in] set Pixel coordinates, Julia constant, and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_julia_test7(const t_set *set)
{
	int		n;
	double	a;
	double	b;
	t_coor	next;

	n = 0;
	a = set->zr;
	b = set->zi;
	while (n < set->max_i)
	{
		next = ft_julia_step7(a, b, set);
		a = next.a;
		b = next.b;
		if (a * a + b * b > set->esc)
			return ((t_orbit_result){
				.point = next, .n = n, .escaped = true});
		if (a == b)
			n = set->max_i - 1;
		n++;
	}
	return ((t_orbit_result){
		.point = {a, b}, .n = n, .escaped = false});
}
