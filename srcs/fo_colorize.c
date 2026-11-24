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

static int	ft_blend_channel(Uint32 start, Uint32 end, double fraction)
{
	return ((int)(start + ((double)end - start) * fraction + 0.5));
}

static int	ft_palette_blend(t_palette_interpolation interpolation,
		Uint32 start, Uint32 end, double fraction)
{
	if (interpolation == PALETTE_OKLAB)
		return (ft_rgb_lerp_oklab(start, end, fraction));
	if (interpolation == PALETTE_LINEAR)
		return (ft_rgb_lerp_linear(start, end, fraction));
	return (create_trgb(0,
			ft_blend_channel(start >> 16 & 255, end >> 16 & 255, fraction),
			ft_blend_channel(start >> 8 & 255, end >> 8 & 255, fraction),
			ft_blend_channel(start & 255, end & 255, fraction)));
}

static int	ft_single_color_gradient(const t_set *set, Uint32 reference,
		double position)
{
	double	fraction;

	/* Reflect the black-to-white gradient so its repeat has no hard seam. */
	fraction = fmod(position, 4.0);
	if (fraction > 2)
		fraction = 4 - fraction;
	if (fraction <= 1)
		return (ft_palette_blend(set->palette_interpolation, 0,
				reference, fraction));
	return (ft_palette_blend(set->palette_interpolation, reference,
			0xffffff, fraction - 1));
}

static int	ft_scheme_color(const t_set *set, double iterations)
{
	static const Uint32	colors[COLOR_SCHEME_COUNT][PALETTE_COLOR_COUNT] = {
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
	const Uint32	*palette;
	double			position;
	int				index;

	palette = colors[set->color_scheme];
	position = iterations / set->palette_span;
	if (set->palette_single_color)
		return (ft_single_color_gradient(set, palette[set->palette_anchor],
				position));
	position = fmod(position, PALETTE_COLOR_COUNT);
	index = (int)position;
	return (ft_palette_blend(set->palette_interpolation, palette[index],
			palette[(index + 1) % PALETTE_COLOR_COUNT], position - index));
}

static double	ft_color_iterations(const t_set *set, long double a,
		long double b, int n)
{
	long double	norm;

	if (set->fractal != MALDELBROT && set->fractal != JULIA
		&& set->fractal != TRICORN && set->fractal != BURNINGSHIP)
		return (n);
	norm = a * a + b * b;
	if (!isfinite(norm) || norm <= 1 || set->esc <= 1)
		return (n);
	/* Fractional iteration for orbits with quadratic growth. */
	return (fmax(0, n + 1 - log2l(logl(norm) / logl(set->esc))));
}

static int	ft_full_channel(long double coordinate)
{
	if (isnan(coordinate))
		return (0);
	if (fabsl(coordinate) <= 1)
		return (255);
	return ((int)(255 / fabsl(coordinate) / fabsl(coordinate)));
}

int	ft_colorize_pixel(t_set *set, long double a, long double b, int n,
		bool escaped)
{
	static const Uint32	tints[COLOR_SCHEME_COUNT] = {
		0x16213d, 0x7a2d3f, 0xa3a09f, 0x87706b, 0x695c52, 0x7b161a
	};
	double	iterations;
	double	fraction;
	int		level;
	Uint32	tint;

	set->color = 0;
	if (!escaped)
		return (1);
	iterations = ft_color_iterations(set, a, b, n);
	fraction = fmin(iterations / set->max_i, 1.0);
	level = 32 + (int)(223 * fraction);
	if (set->color_mode == 2)
	{
		tint = tints[set->color_scheme];
		fraction = 0.25 + 0.75 * fraction;
		set->color = create_trgb(0, (int)((tint >> 16 & 255) * fraction),
				(int)((tint >> 8 & 255) * fraction), (int)((tint & 255) * fraction));
	}
	else if (set->color_mode == 3)
		set->color = create_trgb(0, level, level, level);
	else if (set->color_mode == 4)
		set->color = create_trgb((int)(224 * fraction), ft_full_channel(a),
				ft_full_channel(b), level);
	else
		set->color = ft_scheme_color(set, iterations);
	return (1);
}
