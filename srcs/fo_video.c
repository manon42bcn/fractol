/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_video.c                                         +:+      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 19:47:42 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 19:47:42 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Select the active orbit callback and render its pixels.
 * @param[in,out] set Fractal settings and per-pixel state.
 * @param[out] img Destination pixel buffer.
 */
static void	ft_fractol_manager(t_set *set, t_data *img)
{
	static const t_builders	build[FRACTAL_COUNT] = {
		ft_mandelbrot_test, ft_julia_test, ft_burningship_test, ft_drop_test,
		ft_bird_test, ft_spider_test, ft_worm_test, ft_hiper_test,
		ft_mandelbrot_test3, ft_mandelbrot_test7, ft_julia_test3,
		ft_julia_test7, ft_drop_test7, ft_tricorn_test, ft_newton_test,
		ft_phoenix_test, NULL, ft_cos_c_over_z, ft_cos_z_over_c,
		ft_sin_z_over_sqrt_c, ft_rational_quadratic, ft_quadratic_linear,
		ft_rational_reciprocal, ft_inverse_square, ft_inverse_fifth,
		ft_rational_offset_01, ft_rational_offset_0001, ft_rational_square,
		ft_mandelbrot_test4, ft_cubic_minus_linear,
		ft_quadratic_c_plus_c_squared
	};

	if (set->fractal == MALDELBROT && set->color_mode == 1
		&& set->distance_shading)
		ft_iterator(set, ft_mandelbrot_distance_test, img);
	else
		ft_iterator(set, build[set->fractal], img);
}

/**
 * @brief Synchronize the window title with the fractal and color settings.
 * @param[in] set Active fractal and palette settings, with the window to update.
 */
static void	ft_update_palette_title(const t_set *set)
{
	static const char	*interpolation[] = {"sRGB", "Linear light", "Oklab"};
	const char			*name;
	char				title[160];

	name = ft_fractal_titles(set->fractal);
	if (set->color_mode != 1 || set->fractal == NEWTON)
		SDL_snprintf(title, sizeof(title), "%s", name);
	else if (set->fractal == BUDDHABROT)
		SDL_snprintf(title, sizeof(title), "%s | Palette %d | %s", name,
			set->color_scheme + 1, interpolation[set->palette_interpolation]);
	else if (set->palette_single_color)
		SDL_snprintf(title, sizeof(title),
			"%s | Palette %d, color %d | %d iterations/step | %s", name,
			set->color_scheme + 1, set->palette_anchor + 1, set->palette_span,
			interpolation[set->palette_interpolation]);
	else
		SDL_snprintf(title, sizeof(title),
			"%s | Palette %d | %d iterations/step | %s", name,
			set->color_scheme + 1, set->palette_span,
			interpolation[set->palette_interpolation]);
	if (set->distance_shading && set->fractal == MALDELBROT
		&& set->color_mode == 1)
		SDL_strlcat(title, " | Distance shading", sizeof(title));
	if (SDL_strcmp(title, SDL_GetWindowTitle(set->window)) != 0)
		SDL_SetWindowTitle(set->window, title);
}

/**
 * @brief Refresh the texture when needed and present it to the window.
 * @param[in,out] set Render resources, fractal state, and redraw flags.
 */
void	ft_render(t_set *set)
{
	t_data	img;

	if (set->status || set->buddhabrot_recolor)
		ft_update_palette_title(set);
	if (set->status || set->buddhabrot_recolor || ft_buddhabrot_pending(set))
	{
		if (!SDL_LockTexture(set->texture, NULL, &img.addr, &img.pitch))
			ft_alloc_error(set, 2);
		if (set->fractal == BUDDHABROT)
			ft_buddhabrot_render(set, &img);
		else
			ft_fractol_manager(set, &img);
		SDL_UnlockTexture(set->texture);
		set->status = 0;
	}
	if (!SDL_RenderClear(set->renderer)
		|| !SDL_RenderTexture(set->renderer, set->texture, NULL, NULL)
		|| !SDL_RenderPresent(set->renderer))
		ft_alloc_error(set, 1);
}
