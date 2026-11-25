/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_fractal_shapes.c                                +:+      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 19:47:42 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 19:47:42 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Iterate the quadratic Drop map with an inverted pixel parameter.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_drop_test(const t_set *set)
{
	int			n;
	long double	a;
	long double	b;
	long double	t_a;
	t_coor		c;

	n = 1;
	a = set->zr / (set->zr * set->zr + set->zi * set->zi);
	b = set->zi / (set->zr * set->zr + set->zi * set->zi);
	c = (t_coor){(double)a, (double)b};
	while (n < set->max_i)
	{
		t_a = (a * a - b * b) + c.a;
		b = 2.0 * a * b + c.b;
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
 * @brief Iterate the Bird map with component-wise reciprocal initialization.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_bird_test(const t_set *set)
{
	int			n;
	long double	a;
	long double	b;
	long double	t_a;

	n = 1;
	a = 1.0f / set->zr;
	b = 1.0f / set->zi;
	while (n < set->max_i)
	{
		t_a = 1.0f / (a * a - b * b) + set->zr;
		b = 1.0f / 2.0 * a * b + set->zi;
		a = t_a;
		if (a * a - b * b > set->esc)
			return ((t_orbit_result){
				.point = {a, b}, .n = n, .escaped = true});
		n++;
	}
	return ((t_orbit_result){
		.point = {a, b}, .n = n, .escaped = false});
}

/**
 * @brief Compute the seventh-power step for the Drop orbit.
 * @param[in] point Current complex iterate.
 * @param[in] c Inverted pixel coordinate used as the constant.
 * @return Next complex iterate.
 */
static t_coor	ft_drop_step7(t_coor point, t_coor c)
{
	long double	a;
	long double	b;
	t_coor		next;

	a = point.a;
	b = point.b;
	next.a = (a * a * a * a * a * a * a)
		- (21 * ((a * a * a * a * a) * (b * b)))
		+ (35 * ((a * a * a) * (b * b * b * b)))
		- (7 * a * (b * b * b * b * b * b)) + c.a;
	next.b = 7 * b * (a * a * a * a * a * a)
		- (35 * ((a * a * a * a) * (b * b * b)))
		+ (21 * ((a * a) * (b * b * b * b * b)))
		- (b * b * b * b * b * b * b) + c.b;
	return (next);
}

/**
 * @brief Iterate the seventh-power Drop map with an inverted pixel parameter.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_drop_test7(const t_set *set)
{
	int		n;
	t_coor	point;
	t_coor	c;

	n = 1;
	point.a = set->zr / (set->zr * set->zr + set->zi * set->zi);
	point.b = set->zi / (set->zr * set->zr + set->zi * set->zi);
	c = (t_coor){(double)point.a, (double)point.b};
	while (n < set->max_i)
	{
		point = ft_drop_step7(point, c);
		if (point.a * point.a + point.b * point.b > set->esc)
			return ((t_orbit_result){
				.point = point, .n = n, .escaped = true});
		n++;
	}
	return ((t_orbit_result){
		.point = point, .n = n, .escaped = false});
}
