/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_buddhabrot.c                                    +:+      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 19:47:42 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 19:47:42 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

#define BUDDHABROT_BATCH 4096
#define BUDDHABROT_BASE_SAMPLES 1048576u
#define BUDDHABROT_MAX_SAMPLE_MULTIPLIER 16

/**
 * @brief Check whether the current Buddhabrot view needs more samples.
 * @param[in] set Current fractal state.
 * @return True while the sample count is below the view's limit.
 */
bool	ft_buddhabrot_pending(const t_set *set)
{
	return (set->fractal == BUDDHABROT
		&& set->buddhabrot_samples < set->buddhabrot_sample_limit);
}

/**
 * @brief Scale the sample budget with the visible area up to the configured cap.
 * @param[in] viewer Dimensions of the current view.
 * @return Maximum number of samples for this view.
 */
static unsigned int	ft_buddhabrot_sample_budget(const t_viewer *viewer)
{
	long double	area_ratio;

	area_ratio = (DEFAULT_VIEW_WIDTH / viewer->x_width)
		* (DEFAULT_VIEW_HEIGHT / viewer->y_height);
	area_ratio = fminl(fmaxl(area_ratio, 1), BUDDHABROT_MAX_SAMPLE_MULTIPLIER);
	return ((unsigned int)ceill(BUDDHABROT_BASE_SAMPLES * area_ratio));
}

/**
 * @brief Clear the histogram and restart sampling for the current view.
 * @param[in,out] set Histogram, sample counters, and random seed to reset.
 */
static void	ft_buddhabrot_reset(t_set *set)
{
	size_t	pixels;

	pixels = (size_t)set->screen_w * set->screen_h;
	if (set->buddhabrot_counts == NULL)
		set->buddhabrot_counts
			= calloc(pixels, sizeof(*set->buddhabrot_counts));
	else
		SDL_memset(set->buddhabrot_counts, 0,
			pixels * sizeof(*set->buddhabrot_counts));
	if (set->buddhabrot_counts == NULL)
		ft_alloc_error(set, 4);
	set->buddhabrot_samples = 0;
	set->buddhabrot_sample_limit = ft_buddhabrot_sample_budget(set->viewer);
	set->buddhabrot_peak = 0;
	set->buddhabrot_color_peaks = (t_buddhabrot_pixel){0};
	set->buddhabrot_rng = 1;
}

/**
 * @brief Sum the accumulated visits across the three orbit bands.
 * @param[in] pixel Pixel histogram entry.
 * @return Total weighted visits.
 */
Uint64	ft_buddhabrot_total(const t_buddhabrot_pixel *pixel)
{
	return (pixel->visits[0] + pixel->visits[1] + pixel->visits[2]);
}

/**
 * @brief Advance the orbit sampler and draw the current histogram.
 * @param[in,out] set Sampling state and redraw flags.
 * @param[out] img Destination pixel buffer.
 */
void	ft_buddhabrot_render(t_set *set, t_data *img)
{
	t_coor			orbit[MAX_ITERATIONS];
	unsigned int	end;

	if (set->status)
		ft_buddhabrot_reset(set);
	end = set->buddhabrot_samples + BUDDHABROT_BATCH;
	while (set->buddhabrot_samples < end && ft_buddhabrot_pending(set))
	{
		ft_buddhabrot_sample(set, orbit);
		set->buddhabrot_samples++;
	}
	ft_buddhabrot_draw(set, img);
	set->buddhabrot_recolor = false;
}
