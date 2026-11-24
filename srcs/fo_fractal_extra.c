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
			return (ft_colorize_pixel(set, a, b, n, true));
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n, false));
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
			return (ft_colorize_pixel(set, a, b, n, true));
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n, false));
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
			return (ft_colorize_pixel(set, a, b, n, true));
		if (t_a == b)
			n = set->max_i - 1;
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n, false));
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
			return (ft_colorize_pixel(set, a, b, n, true));
		if (t_a == b)
			n = set->max_i - 1;
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n, false));
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
			return (ft_colorize_pixel(set, a, b, n, true));
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n, false));
}

int	ft_phoenix_test(t_set *set)
{
	long double	a;
	long double	b;
	long double	prev_a;
	long double	prev_b;
	long double	next_a;
	long double	next_b;
	int			n;

	a = set->zr;
	b = set->zi;
	prev_a = 0;
	prev_b = 0;
	n = 0;
	while (n < set->max_i)
	{
		next_a = a * a - b * b + 0.5667L - 0.5L * prev_a;
		next_b = 2 * a * b - 0.5L * prev_b;
		prev_a = a;
		prev_b = b;
		a = next_a;
		b = next_b;
		n++;
		/* Above radius 2, growth relative to the previous step guarantees escape. */
		if (a * a + b * b > set->esc
			&& a * a + b * b >= prev_a * prev_a + prev_b * prev_b)
			return (ft_colorize_pixel(set, a, b, n, true));
	}
	return (ft_colorize_pixel(set, a, b, n, false));
}
