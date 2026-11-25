/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_video_init.c                                    +:+      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 19:47:42 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 19:47:42 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Create the streaming texture and request a redraw.
 * @param[in,out] set Renderer, dimensions, and destination texture.
 */
void	ft_init_texture(t_set *set)
{
	set->texture = SDL_CreateTexture(set->renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, set->screen_w, set->screen_h);
	if (!set->texture
		|| !SDL_SetTextureBlendMode(set->texture, SDL_BLENDMODE_BLEND))
		ft_alloc_error(set, 2);
	set->status = 1;
}

/**
 * @brief Open the SDL window and initialize its matching render texture.
 * @param[in,out] set State receiving the render resources and dimensions.
 */
void	ft_init_video(t_set *set)
{
	SDL_Rect	display;

	if (!SDL_Init(SDL_INIT_VIDEO)
		|| !SDL_GetDisplayBounds(SDL_GetPrimaryDisplay(), &display))
		ft_alloc_error(set, 1);
	set->screen_w = display.w / FACTOR;
	set->screen_h = display.h / FACTOR;
	if (!SDL_CreateWindowAndRenderer(ft_fractal_titles(set->fractal),
			set->screen_w, set->screen_h, 0, &set->window, &set->renderer)
		|| !SDL_SetRenderDrawColor(set->renderer, 0, 0, 0, 255))
		ft_alloc_error(set, 1);
	ft_init_texture(set);
}

/**
 * @brief Release fractal resources and shut down SDL.
 * @param[in,out] set State to destroy, which may be NULL.
 */
void	ft_free_all(t_set *set)
{
	if (set)
	{
		if (set->texture)
			SDL_DestroyTexture(set->texture);
		if (set->renderer)
			SDL_DestroyRenderer(set->renderer);
		if (set->window)
			SDL_DestroyWindow(set->window);
		free(set->buddhabrot_counts);
		free(set->viewer);
		free(set);
	}
	SDL_Quit();
}
