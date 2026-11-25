/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_palette.c                                       +:+      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 19:47:42 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 19:47:42 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Interpolate one 8-bit channel in sRGB and round to the nearest integer.
 * @param[in] start Starting channel value.
 * @param[in] end Ending channel value.
 * @param[in] fraction Interpolation position between 0 and 1.
 * @return Interpolated channel value.
 */
static int	ft_blend_channel(Uint32 start, Uint32 end, double fraction)
{
	return ((int)(start + ((double)end - start) * fraction + 0.5));
}

/**
 * @brief Interpolate between two palette colors in the selected color space.
 * @param[in] interpolation Interpolation mode.
 * @param[in] start Packed RGB starting color.
 * @param[in] end Packed RGB ending color.
 * @param[in] fraction Interpolation position between 0 and 1.
 * @return Packed interpolated color.
 */
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

/**
 * @brief Evaluate a reflected black-to-white gradient through one color.
 * @param[in] set Selected interpolation mode.
 * @param[in] reference Packed RGB reference color.
 * @param[in] position Position measured in gradient segments.
 * @return Packed gradient color at the requested position.
 */
static int	ft_single_color_gradient(const t_set *set, Uint32 reference,
		double position)
{
	double	fraction;

	fraction = fmod(position, 4.0);
	if (fraction > 2)
		fraction = 4 - fraction;
	if (fraction <= 1)
		return (ft_palette_blend(set->palette_interpolation, 0,
				reference, fraction));
	return (ft_palette_blend(set->palette_interpolation, reference,
			0xffffff, fraction - 1));
}

/**
 * @brief Look up the reference colors in the selected classic palette.
 * @param[in] scheme Index of the active color scheme.
 * @return Array of packed RGB stops in palette order.
 */
static const Uint32	*ft_palette_colors(int scheme)
{
	static const Uint32	colors[COLOR_SCHEME_COUNT][PALETTE_COLOR_COUNT] = {
	{0x0016213d, 0x007a2d3f, 0x00a3a09f, 0x0087706b, 0x00695c52,
		0x007b161a, 0x00878962, 0x0013121f, 0x002c434b, 0x00e0dedf},
	{0x0010233d, 0x0014435f, 0x00136e80, 0x0028a3a6, 0x0081d1c5,
		0x00fff4d6, 0x00b7dcd2, 0x0067afb4, 0x0036758e, 0x001f4867},
	{0x000D0D0D, 0x00B0B0B0, 0x00E6E6E6, 0x009C9C9C, 0x00414141,
		0x000A0A0A, 0x002E2E2E, 0x006E6E6E, 0x00838383, 0x00DFDFDF},
	{0x001C0D06, 0x00221B10, 0x00403F4F, 0x003F4D96, 0x0085AFA1,
		0x0043301B, 0x0027281A, 0x00967658, 0x00321F0F, 0x00423724},
	{0x00AF3433, 0x00AEB5BF, 0x00878684, 0x0063646F, 0x00FCF2ED,
		0x00C8522F, 0x006A798B, 0x00B5AFB1, 0x003C3C3E, 0x00B1583E},
	{0x00030500, 0x00897131, 0x005B601F, 0x009B5C0C, 0x00D8CD4A,
		0x00E0F89E, 0x00BC3B00, 0x005B1900, 0x006F5B0B, 0x00E3FFB3}
	};

	return (colors[scheme]);
}

/**
 * @brief Map an escape count to a classic palette color.
 * @param[in] set Palette, transition length, and interpolation settings.
 * @param[in] iterations Escape count, which may be fractional.
 * @return Packed color from the active gradient.
 */
int	ft_scheme_color(const t_set *set, double iterations)
{
	const Uint32	*palette;
	double			position;
	int				index;

	palette = ft_palette_colors(set->color_scheme);
	position = iterations / set->palette_span;
	if (set->palette_single_color)
		return (ft_single_color_gradient(set, palette[set->palette_anchor],
				position));
	position = fmod(position, PALETTE_COLOR_COUNT);
	index = (int)position;
	return (ft_palette_blend(set->palette_interpolation, palette[index],
			palette[(index + 1) % PALETTE_COLOR_COUNT], position - index));
}
