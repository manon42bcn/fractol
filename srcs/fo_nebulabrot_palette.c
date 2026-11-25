/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_nebulabrot_palette.c                            +:+      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 19:47:42 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 19:47:42 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

#define NEBULABROT_PALETTE_COUNT 4

/**
 * @brief Advance to the next Nebulabrot palette and request recoloring.
 * @param[in,out] set State containing the active palette and recolor flag.
 */
void	ft_nebulabrot_next_palette(t_set *set)
{
	set->color_scheme = (set->color_scheme + 1) % NEBULABROT_PALETTE_COUNT;
	set->buddhabrot_recolor = true;
}

/**
 * @brief Interpolate between two colors in the selected color space.
 * @param[in] stops Packed RGB starting and ending colors.
 * @param[in] step Step within the gradient segment.
 * @param[in] span Number of steps in the segment.
 * @param[in] interpolation Interpolation mode.
 * @return Packed RGB color at the requested step.
 */
static Uint32	ft_gradient_rgb(const Uint32 stops[2], int step, int span,
		t_palette_interpolation interpolation)
{
	Uint32	color;
	int		shift;
	int		channel;

	if (interpolation == PALETTE_OKLAB)
		return (ft_rgb_lerp_oklab(stops[0], stops[1], (double)step / span));
	if (interpolation == PALETTE_LINEAR)
		return (ft_rgb_lerp_linear(stops[0], stops[1], (double)step / span));
	color = 0;
	shift = 0;
	while (shift <= 16)
	{
		channel = (((stops[0] >> shift) & 255) * (span - step)
				+ ((stops[1] >> shift) & 255) * step + span / 2) / span;
		color |= (Uint32)channel << shift;
		shift += 8;
	}
	return (color);
}

/**
 * @brief Build a gradient from black through the midpoint to the endpoint.
 * @param[in] stops Packed RGB midpoint and endpoint colors.
 * @param[in] interpolation Interpolation mode.
 * @param[out] gradient One array of 256 packed RGB colors.
 */
static void	ft_nebulabrot_band_gradient(const Uint32 stops[2],
		t_palette_interpolation interpolation, Uint32 gradient[256])
{
	int	level;

	level = 0;
	while (level <= 128)
	{
		gradient[level] = ft_gradient_rgb((Uint32[2]){0, stops[0]},
				level, 128, interpolation);
		level++;
	}
	while (level <= 255)
	{
		gradient[level] = ft_gradient_rgb(stops, level - 128, 127,
				interpolation);
		level++;
	}
}

/**
 * @brief Build an intensity-to-color lookup table for each orbit band.
 * @param[in] set Active Nebulabrot palette and interpolation mode.
 * @param[out] gradients Three arrays of 256 packed RGB colors.
 */
void	ft_nebulabrot_gradients(const t_set *set, Uint32 gradients[3][256])
{
	static const Uint32	palettes[NEBULABROT_PALETTE_COUNT][3][2] = {
	{{0x800000, 0xff0000}, {0x008000, 0x00ff00}, {0x000080, 0x0000ff}},
	{{0x982e12, 0xffba65}, {0x087063, 0x87ffd4}, {0x121d74, 0x598cfa}},
	{{0x6d154c, 0xff9ac2}, {0x4a2493, 0xd2a6ff}, {0x124d6b, 0x77e9ff}},
	{{0xa34206, 0xffdf9c}, {0x0a4b8c, 0x88d9ff}, {0x241449, 0x9672dc}},
	};
	int					band;

	band = 0;
	while (band < 3)
	{
		ft_nebulabrot_band_gradient(palettes[set->color_scheme][band],
			set->palette_interpolation, gradients[band]);
		band++;
	}
}
