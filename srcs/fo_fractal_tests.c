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

int	ft_mandelbrot_test(t_set *set)
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

int	ft_tricorn_test(t_set *set)
{
	int				n;
	long double	a;
	long double	b;
	long double	next_a;
	long double	c_r;
	long double	c_i;
	long double	escape;

	n = 1;
	a = 0;
	b = 0;
	c_r = set->zr;
	c_i = set->zi;
	escape = set->esc;
	if (set->tricorn_julia)
	{
		a = set->zr;
		b = set->zi;
		c_r = set->c_r;
		c_i = set->c_i;
		/* A large selected constant can require a larger escape radius. */
		escape = fmaxl(escape, c_r * c_r + c_i * c_i);
	}
	while (n < set->max_i)
	{
		next_a = a * a - b * b + c_r;
		b = -2 * a * b + c_i;
		a = next_a;
		if (a * a + b * b > escape)
			return (ft_colorize_pixel(set, a, b, n, true));
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n, false));
}

int	ft_julia_test(t_set *set)
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
			return (ft_colorize_pixel(set, a, b, n, true));
		if (t_a == b)
			n = set->max_i - 1;
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n, false));
}

int	ft_burningship_test(t_set *set)
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
			return (ft_colorize_pixel(set, a, b, n, true));
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n, false));
}

int	ft_drop_test(t_set *set)
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
		t_a = (a * a - b * b) + set->zr;
		b = 2.0 * a * b + set->zi;
		a = t_a;
		if (a * a + b * b > set->esc)
			return (ft_colorize_pixel(set, a, b, n, true));
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n, false));
}

int	ft_bird_test(t_set *set)
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
			return (ft_colorize_pixel(set, a, b, n, true));
		n++;
	}
	return (ft_colorize_pixel(set, a, b, n, false));
}
