/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_colorize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 20:49:10 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:53:49 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void	ft_scheme_mode(t_set *set, int n)
{
	static int	colors[6][10] = {
	{0x0016213d, 0x007a2d3f, 0x00a3a09f, 0x0087706b, 0x00695c52,
		0x007b161a, 0x00878962, 0x0013121f, 0x002c434b, 0x00e0dedf},
	{0x00603b2b, 0x00987259, 0x006b5149, 0x008b6b56, 0x00a27f63,
		0x00907f89, 0x0096aab4, 0x00884439, 0x00857267, 0x009b0929},
	{0x000D0D0D, 0x00B0B0B0, 0x00E6E6E6, 0x009C9C9C, 0x00414141,
		0x000A0A0A, 0x002E2E2E, 0x006E6E6E, 0x00838383, 0x00DFDFDF},
	{0x001C0D06, 0x00221B10, 0x00403F4F, 0x003F4D96, 0x0085AFA1,
		0x0043301B, 0x0027281A, 0x00967658, 0x00321F0F, 0x00423724},
	{0x00AF3433, 0x00AEB5BF, 0x00878684, 0x0063646F, 0x00FCF2ED,
		0x00C8522F, 0x006A798B, 0x00B5AFB1, 0x003C3C3E, 0x00B1583E},
	{0x00030500, 0x00897131, 0x005B601F, 0x009B5C0C, 0x00D8CD4A,
		0x00E0F89E, 0x00BC3B00, 0x005B1900, 0x006F5B0B, 0x00E3FFB3}};

	if (n == set->max_i)
		set->color = colors[set->color_scheme][0];
	else
	{
		while (n % 7 > 9)
		n /= 2;
		set->color = colors[set->color_scheme][n % 7];
	}
}

static void	ft_full_mode(t_set *set, double a, double b, int n)
{
	if (n == set->max_i)
		set->color = 0x00000000;
	else
		set->color = create_trgb((n * 255) / set->max_i,
				(b * b * 255) / (b * b * a * a),
				(a * a * 255) / (b * b * a * a),
				(n * 255) / set->max_i);
}

static void	ft_mono_mode(t_set *set, int n)
{
	static int	colors[10] = {0x0016213d, 0x007a2d3f, 0x00a3a09f,
		0x0087706b, 0x00695c52, 0x007b161a,
		0x00878962, 0x0013121f, 0x002c434b, 0x00e0dedf};
	int			transparency;
	int			red;
	int			green;
	int			black;

	if (n == set->max_i && set->fractal != 2)
		set->color = colors[set->color_scheme];
	else
	{
		transparency = (colors[set->color_scheme] >> 24 & 0xFF);
		red = ((colors[set->color_scheme] >> 16 & 0xFF) * n) / set->max_i;
		green = ((colors[set->color_scheme] >> 8 & 0xFF) * n) / set->max_i;
		black = ((colors[set->color_scheme] & 0xFF) * n) / set->max_i;
		set->color = create_trgb(transparency, red, green, black);
	}
}

int	ft_colorize_pixel(t_set *set, long double a, long double b, int n)
{	
	if (set->fractal == 2)
		n *= 2;
	n = (n * 255) / set->max_i;
	if (set->color_mode == 2)
		ft_mono_mode(set, n);
	else if (set->color_mode == 3)
		set->color = create_trgb(0, n, n, n);
	else if (set->color_mode == 4)
		ft_full_mode(set, a, b, n);
	else
		ft_scheme_mode(set, n);
	return (1);
}
