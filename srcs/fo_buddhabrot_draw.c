/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_buddhabrot_draw.c                               +:+      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 19:47:42 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 19:47:42 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Compute the scale that maps the peak count to full brightness.
 * @param[in] peak Peak weighted visit count.
 * @return Logarithmic scaling factor, or zero for an empty histogram.
 */
static float	ft_density_scale(Uint64 peak)
{
	if (peak == 0)
		return (0);
	return (255.0f / log1pf((float)peak / BUDDHABROT_VISIT_WEIGHT));
}

/**
 * @brief Map a weighted visit count to an 8-bit display intensity.
 * @param[in] count Weighted visit count.
 * @param[in] scale Logarithmic scaling factor for the band.
 * @param[in] original_exposure True to use the unadjusted logarithmic scale.
 * @return Intensity in the range 0 to 255.
 */
static int	ft_density_level(Uint64 count, float scale, bool original_exposure)
{
	float	level;

	if (count == 0)
		return (0);
	level = fminf(255, log1pf((float)count / BUDDHABROT_VISIT_WEIGHT) * scale);
	if (!original_exposure)
		level *= sqrtf(level / 255.0f);
	return ((int)level);
}

/**
 * @brief Combine the three orbit bands without clipping additive sums.
 * @param[in] first RGB color for the first band.
 * @param[in] second RGB color for the second band.
 * @param[in] third RGB color for the third band.
 * @return Packed RGB color of the combined bands.
 */
static int	ft_nebulabrot_mix(Uint32 first, Uint32 second, Uint32 third)
{
	Uint32	color;
	Uint32	remainder;
	int		shift;

	color = 0;
	shift = 0;
	while (shift <= 16)
	{
		remainder = (255 - ((first >> shift) & 255))
			* (255 - ((second >> shift) & 255))
			* (255 - ((third >> shift) & 255));
		color |= (255 - (remainder + 32512) / 65025) << shift;
		shift += 8;
	}
	return ((int)color);
}

/**
 * @brief Determine a pixel color from its histogram entry.
 * @param[in] set Display mode and exposure settings.
 * @param[in] pixel Histogram entry for the destination pixel.
 * @param[in] tones Density scales and orbit-band gradients.
 * @return Packed color for the histogram entry.
 */
static int	ft_buddhabrot_pixel_color(const t_set *set,
		const t_buddhabrot_pixel *pixel, const t_buddhabrot_tones *tones)
{
	int		level;
	int		band;
	Uint32	colors[3];

	if (set->color_mode != 1)
	{
		level = ft_density_level(ft_buddhabrot_total(pixel), tones->scale[0],
				set->buddhabrot_original_exposure);
		return (create_trgb(0, level, level, level));
	}
	band = 0;
	while (band < 3)
	{
		level = ft_density_level(pixel->visits[band], tones->scale[band],
				set->buddhabrot_original_exposure);
		colors[band] = tones->gradients[band][level];
		band++;
	}
	return (ft_nebulabrot_mix(colors[0], colors[1], colors[2]));
}

/**
 * @brief Convert the current histogram to grayscale or Nebulabrot pixels.
 * @param[in] set Histogram, exposure, and palette settings.
 * @param[out] img Destination pixel buffer.
 */
void	ft_buddhabrot_draw(const t_set *set, t_data *img)
{
	t_buddhabrot_tones	tones;
	t_pixel_coord		coord;
	size_t				index;
	const Uint64		*peaks;

	if (set->color_mode == 1)
	{
		ft_nebulabrot_gradients(set, tones.gradients);
		peaks = set->buddhabrot_color_peaks.visits;
		tones.scale[0] = ft_density_scale(peaks[0]);
		tones.scale[1] = ft_density_scale(peaks[1]);
		tones.scale[2] = ft_density_scale(peaks[2]);
	}
	else
		tones.scale[0] = ft_density_scale(set->buddhabrot_peak);
	coord.y = -1;
	index = 0;
	while (++coord.y < set->screen_h)
	{
		coord.x = -1;
		while (++coord.x < set->screen_w)
			ft_put_pixel(img, coord.x, coord.y,
				ft_buddhabrot_pixel_color(set,
					&set->buddhabrot_counts[index++], &tones));
	}
}
