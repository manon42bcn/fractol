/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_colors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 22:58:01 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 22:58:01 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include <assert.h>
#include <stdio.h>

/**
 * @brief Compute relative luminance from a packed sRGB color.
 * @param[in] color Packed RGB color.
 * @return Relative luminance in linear light.
 */
static double	luminance(Uint32 color)
{
	return (0.2126 * ft_rgb_decode((color >> 16) & 255)
		+ 0.7152 * ft_rgb_decode((color >> 8) & 255)
		+ 0.0722 * ft_rgb_decode(color & 255));
}

/**
 * @brief Check the lightness ramp and closure of a revised classic palette.
 * @param[in] scheme Zero-based palette index.
 */
static void	test_palette_cycle(int scheme)
{
	t_set	set;
	double	previous;
	double	current;
	int		i;
	int		period;

	set = (t_set){.color_scheme = scheme, .palette_span = 8,
		.palette_interpolation = PALETTE_OKLAB};
	period = PALETTE_COLOR_COUNT * set.palette_span;
	assert(ft_scheme_color(&set, period) == ft_scheme_color(&set, 0));
	previous = luminance(ft_scheme_color(&set, 0));
	i = 1;
	while (i <= period)
	{
		current = luminance(ft_scheme_color(&set, i));
		if (i <= period / 2)
			assert(current > previous);
		else
			assert(current < previous);
		previous = current;
		i++;
	}
}

/**
 * @brief Check escape-time smoothing for several degrees and its fallback.
 */
static void	test_smoothing(void)
{
	t_set	set;
	t_coor	point;

	set = (t_set){.fractal = MALDELBROT, .esc = 4};
	point = (t_coor){16, 0};
	assert(fabs(ft_smooth_iterations(&set, point, 8) - 7) < 1e-12);
	set.fractal = MANDELBROT4;
	assert(fabs(ft_smooth_iterations(&set, point, 8) - 8) < 1e-12);
	set.fractal = JULIA3;
	assert(fabs(ft_smooth_iterations(&set, (t_coor){8, 0}, 8) - 8) < 1e-12);
	set.fractal = MANDELBROT7;
	assert(fabs(ft_smooth_iterations(&set, (t_coor){128, 0}, 8) - 8) < 1e-12);
	assert(ft_smooth_iterations(&set, (t_coor){INFINITY, 0}, 8) == 8);
	set.fractal = COS_Z_OVER_C;
	assert(ft_smooth_iterations(&set, point, 8) == 8);
}

/**
 * @brief Verify the exterior estimate and the resulting near-edge shading.
 */
static void	test_distance_shading(void)
{
	t_set			set;
	t_viewer		viewer;
	t_orbit_result	plain;
	t_orbit_result	shaded;
	int				color;

	viewer = (t_viewer){.x_width = 1, .y_height = 1};
	set = (t_set){.fractal = MALDELBROT, .zr = 1, .esc = 4, .max_i = 50,
		.color_mode = 1, .palette_span = 8, .screen_w = 100, .screen_h = 100,
		.viewer = &viewer};
	plain = ft_mandelbrot_test(&set);
	shaded = ft_mandelbrot_distance_test(&set);
	assert(shaded.escaped && plain.escaped && shaded.n == plain.n);
	assert(shaded.point.a == plain.point.a && shaded.point.b == plain.point.b);
	assert(fabsl(shaded.distance - 5 * logl(5) / 13) < 1e-12L);
	color = ft_colorize_pixel(&set, plain);
	assert(ft_colorize_pixel(&set, shaded) == color);
	shaded.distance = 0.001L;
	assert(luminance(ft_colorize_pixel(&set, shaded)) < luminance(color));
	set.zr = 0;
	shaded = ft_mandelbrot_distance_test(&set);
	assert(!shaded.escaped && shaded.distance == 0);
}

/**
 * @brief Run color regression tests for palette cycles and orbit shading.
 * @return Zero if all assertions pass.
 */
int	main(void)
{
	test_palette_cycle(2);
	test_palette_cycle(5);
	test_smoothing();
	test_distance_shading();
	puts("Color tests passed.");
	return (0);
}
