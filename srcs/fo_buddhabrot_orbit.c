/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_buddhabrot_orbit.c                              +:+      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 19:47:42 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 19:47:42 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Classify an orbit by the iteration at which it escaped.
 * @param[in] escape_step One-based escape iteration.
 * @return Band index, with 0 for long, 1 for medium, and 2 for short orbits.
 */
static int	ft_buddhabrot_band(int escape_step)
{
	if (escape_step <= 8)
		return (2);
	if (escape_step <= 24)
		return (1);
	return (0);
}

/**
 * @brief Add one weighted visit and update the histogram peaks.
 * @param[in,out] set Histogram and peak counters.
 * @param[in] coord Destination pixel column and row.
 * @param[in] band Orbit band index.
 * @param[in] weight Subpixel visit weight.
 */
static void	ft_buddhabrot_add(t_set *set, t_pixel_coord coord, int band,
		Uint64 weight)
{
	t_buddhabrot_pixel	*pixel;
	Uint64				count;
	Uint64				total;

	if (weight == 0 || coord.x < 0 || coord.x >= set->screen_w
		|| coord.y < 0 || coord.y >= set->screen_h)
		return ;
	pixel = &set->buddhabrot_counts[(size_t)coord.y * set->screen_w + coord.x];
	pixel->visits[band] += weight;
	count = pixel->visits[band];
	if (count > set->buddhabrot_color_peaks.visits[band])
		set->buddhabrot_color_peaks.visits[band] = count;
	total = ft_buddhabrot_total(pixel);
	if (total > set->buddhabrot_peak)
		set->buddhabrot_peak = total;
}

/**
 * @brief Apply bilinear visit weights, clipping at the texture edges.
 * @param[in,out] set Histogram and screen dimensions.
 * @param[in] point Continuous pixel coordinate before centering.
 * @param[in] band Orbit band index.
 */
static void	ft_buddhabrot_splat(t_set *set, t_coor point, int band)
{
	t_pixel_coord	base;
	Uint64			right;
	Uint64			bottom;
	Uint64			left;
	Uint64			top;

	point.a -= 0.5L;
	point.b -= 0.5L;
	if (!(point.a > -1 && point.a < set->screen_w
			&& point.b > -1 && point.b < set->screen_h))
		return ;
	base = (t_pixel_coord){(int)floorl(point.a), (int)floorl(point.b)};
	right = (Uint64)((point.a - base.x) * BUDDHABROT_SUBPIXEL_STEPS + 0.5L);
	bottom = (Uint64)((point.b - base.y) * BUDDHABROT_SUBPIXEL_STEPS + 0.5L);
	left = BUDDHABROT_SUBPIXEL_STEPS - right;
	top = BUDDHABROT_SUBPIXEL_STEPS - bottom;
	ft_buddhabrot_add(set, base, band, left * top);
	ft_buddhabrot_add(set, (t_pixel_coord){base.x + 1, base.y},
		band, right * top);
	ft_buddhabrot_add(set, (t_pixel_coord){base.x, base.y + 1},
		band, left * bottom);
	ft_buddhabrot_add(set, (t_pixel_coord){base.x + 1, base.y + 1},
		band, right * bottom);
}

/**
 * @brief Project the stored points of an escaping orbit onto the histogram.
 * @param[in,out] set View and histogram state.
 * @param[in] orbit Stored complex orbit points.
 * @param[in] n Number of stored points preceding escape.
 */
static void	ft_buddhabrot_record(t_set *set, const t_coor *orbit, int n)
{
	long double	x_scale;
	long double	y_scale;
	int			band;
	int			i;

	x_scale = set->screen_w / set->viewer->x_width;
	y_scale = set->screen_h / set->viewer->y_height;
	band = ft_buddhabrot_band(n + 1);
	i = 0;
	while (i < n)
	{
		ft_buddhabrot_splat(set, (t_coor){
			orbit[i].a * x_scale + set->viewer->x_zero,
			orbit[i].b * y_scale + set->viewer->y_zero}, band);
		i++;
	}
}

/**
 * @brief Sample the fixed Mandelbrot region and record escaping orbits.
 * @param[in,out] set Sampling settings, random state, and histogram.
 * @param[out] orbit Scratch buffer with room for the configured iteration limit.
 */
void	ft_buddhabrot_sample(t_set *set, t_coor *orbit)
{
	double	c[2];
	double	a;
	double	b;
	double	next_a;
	int		n;

	c[0] = 3.4 * SDL_randf_r(&set->buddhabrot_rng) - 2.2;
	c[1] = 3.0 * SDL_randf_r(&set->buddhabrot_rng) - 1.5;
	a = 0;
	b = 0;
	n = 0;
	while (n < set->max_i)
	{
		next_a = a * a - b * b + c[0];
		b = 2 * a * b + c[1];
		a = next_a;
		if (a * a + b * b > set->esc)
			break ;
		orbit[n++] = (t_coor){a, b};
	}
	if (n < set->max_i)
		ft_buddhabrot_record(set, orbit, n);
}
