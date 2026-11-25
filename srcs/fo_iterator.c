/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_iterator.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/13 15:34:21 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:53:23 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Iterate the Hiper map using exponential and trigonometric terms.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_hiper_test(const t_set *set)
{
	int			n;
	long double	a;
	long double	b;
	long double	t_a;
	t_coor		c;

	n = 1;
	a = set->zr / (set->zr * set->zr + set->zi * set->zi);
	b = -set->zi / (set->zr * set->zr + set->zi * set->zi);
	c = (t_coor){(double)a, (double)b};
	while (n < set->max_i)
	{
		t_a = cos(b) * ((exp(a) - exp(-a)) / 2) + c.a;
		b = sin(b) * ((exp(a) + exp(-a)) / 2) + c.b;
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
 * @brief Iterate the Worm map using the complex exponential.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_worm_test(const t_set *set)
{
	int			n;
	long double	a;
	long double	b;
	long double	t_a;
	t_coor		c;

	n = 1;
	a = exp(set->zr) * cos(set->zi);
	b = exp(set->zr) * sin(set->zi);
	c = (t_coor){(double)a, (double)b};
	while (n < set->max_i)
	{
		t_a = exp(a) * cos(b) + c.a;
		b = exp(a) * sin(b) + c.b;
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
 * @brief Iterate the Spider map using trigonometric and hyperbolic terms.
 * @param[in] set Pixel coordinates and orbit settings.
 * @return Final orbit point, iteration count, and escape status.
 */
t_orbit_result	ft_spider_test(const t_set *set)
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
		t_a = cos(a) * ((exp(b) + exp(-b)) / 2) + c.a;
		b = -sin(a) * ((exp(b) - exp(-b)) / 2) + c.b;
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
 * @brief Map each pixel to the complex plane and render its orbit.
 * @param[in,out] set Fractal settings and per-pixel coordinates.
 * @param[in] orbit Orbit callback for the active fractal.
 * @param[out] img Destination pixel buffer.
 */
void	ft_iterator(t_set *set, t_builders orbit, t_data *img)
{
	int	x;
	int	y;

	x = 0;
	while (x < set->screen_w)
	{
		y = 0;
		while (y < set->screen_h)
		{
			set->zr = (x - set->viewer->x_zero)
				* (set->viewer->x_width / set->screen_w);
			set->zi = (y - set->viewer->y_zero)
				* (set->viewer->y_height / set->screen_h);
			ft_put_pixel(img, x, y, ft_colorize_pixel(set, orbit(set)));
			y++;
		}
		x++;
	}
}
