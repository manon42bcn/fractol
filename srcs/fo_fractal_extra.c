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

int	ft_mandelbrot_test3(t_set *set)
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
			return (ft_colorize_pixel(set, a, b, n));
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n));
}

int	ft_mandelbrot_test7(t_set *set)
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
			return (ft_colorize_pixel(set, a, b, n));
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n));
}

int	ft_julia_test3(t_set *set)
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
			return (ft_colorize_pixel(set, a, b, n));
		if (t_a == b)
			n = set->max_i - 1;
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n));
}

int	ft_julia_test7(t_set *set)
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
		t_a = (a * a * a * a * a * a * a) - (21 * ((a * a * a * a * a)
					* (b * b))) + (35 * ((a * a * a) * (b * b * b * b)))
			- (7 * a * (b * b * b * b * b * b)) + set->c_i;
		b = 7 * b * (a * a * a * a * a * a) - (35 * ((a * a * a * a)
					* (b * b * b))) + (21 * ((a * a) * (b * b * b * b * b)))
			- (b * b * b * b * b * b * b) + set->c_r;
		a = t_a;
		if (a * a + b * b > set->esc)
			return (ft_colorize_pixel(set, a, b, n));
		if (t_a == b)
			n = set->max_i - 1;
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n));
}

int	ft_drop_test7(t_set *set)
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
		t_a = (a * a * a * a * a * a * a) - (21 * ((a * a * a * a * a)
					* (b * b))) + (35 * ((a * a * a) * (b * b * b * b)))
			- (7 * a * (b * b * b * b * b * b)) + set->zr;
		b = 7 * b * (a * a * a * a * a * a) - (35 * ((a * a * a * a)
					* (b * b * b))) + (21 * ((a * a) * (b * b * b * b * b)))
			- (b * b * b * b * b * b * b) + set->zi;
		a = t_a;
		if (a * a + b * b > set->esc)
			return (ft_colorize_pixel(set, a, b, n));
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n));
}
