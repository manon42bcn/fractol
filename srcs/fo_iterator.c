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

int	ft_hiper_test(t_set *set)
{
	int			n;
	long double	a;
	long double	b;
	long double	t_a;

	n = 1;
	a = set->zr / (set->zr * set->zr + set->zi * set->zi);
	b = -set->zi / (set->zr * set->zr + set->zi * set->zi);
	set->zr = a;
	set->zi = b;
	while (n < set->max_i)
	{
		t_a = cos(b) * ((exp(a) - exp(-a)) / 2) + set->zr;
		b = sin(b) * ((exp(a) + exp(-a)) / 2) + set->zi;
		a = t_a;
		if (a * a - b * b > set->esc)
			return (ft_colorize_pixel(set, a, b, n));
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n));
}

int	ft_worm_test(t_set *set)
{
	int			n;
	long double	a;
	long double	b;
	long double	t_a;

	n = 1;
	a = exp(set->zr) * cos(set->zi);
	b = exp(set->zr) * sin(set->zi);
	set->zr = a;
	set->zi = b;
	while (n < set->max_i)
	{
		t_a = exp(a) * cos(b) + set->zr;
		b = exp(a) * sin(b) + set->zi;
		a = t_a;
		if (a * a - b * b > set->esc)
			return (ft_colorize_pixel(set, a, b, n));
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n));
}

int	ft_spider_test(t_set *set)
{
	int			n;
	long double	a;
	long double	b;
	long double	t_a;

	n = 1;
	a = set->zr / (set->zr * set->zr + set->zi * set->zi);
	b = set->zi / (set->zr * set->zr + set->zi * set->zi);
	set->zr = a;
	set->zi = b;
	while (n < set->max_i)
	{
		t_a = cos(a) * ((exp(b) + exp(-b)) / 2) + set->zr;
		b = -sin(a) * ((exp(b) - exp(-b)) / 2) + set->zi;
		a = t_a;
		if (a * a - b * b > set->esc)
			return (ft_colorize_pixel(set, a, b, n));
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n));
}

void	ft_iterator(t_set *set, int (f)(t_set *))
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
			f(set);
			my_mlx_pixel_put(set->img, x, y, set->color);
			y++;
		}
		x++;
	}
}
