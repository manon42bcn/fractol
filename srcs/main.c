/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:03:57 by mporras-          #+#    #+#             */
/*   Updated: 2024/09/22 22:44:01 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include <SDL3/SDL_main.h>

static void	ft_fractol_manager(t_set *set, t_data *img)
{
	t_builders build[] = {&ft_mandelbrot_test, &ft_julia_test,
						  &ft_burningship_test, &ft_drop_test,
						  &ft_bird_test,&ft_spider_test,
						  &ft_worm_test, &ft_hiper_test,
						  &ft_mandelbrot_test3, &ft_mandelbrot_test7,
						  &ft_julia_test3, &ft_julia_test7,
						  &ft_drop_test7, NULL};

    ft_iterator(set, build[set->fractal], img);
}

static void	ft_render(t_set *set)
{
	t_data	img;

	if (set->status)
	{
		if (!SDL_LockTexture(set->texture, NULL, &img.addr, &img.pitch))
			ft_alloc_error(set, 2);
		ft_fractol_manager(set, &img);
		SDL_UnlockTexture(set->texture);
		set->status = 0;
	}
	if (!SDL_RenderClear(set->renderer)
		|| !SDL_RenderTexture(set->renderer, set->texture, NULL, NULL)
		|| !SDL_RenderPresent(set->renderer))
		ft_alloc_error(set, 1);
}

static void	ft_init_texture(t_set *set)
{
	set->texture = SDL_CreateTexture(set->renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, set->screen_w, set->screen_h);
	if (!set->texture || !SDL_SetTextureBlendMode(set->texture, SDL_BLENDMODE_BLEND))
		ft_alloc_error(set, 2);
	set->status = 1;
}

static void	ft_init_video(t_set *set)
{
	SDL_Rect	display;

	if (!SDL_Init(SDL_INIT_VIDEO)
		|| !SDL_GetDisplayBounds(SDL_GetPrimaryDisplay(), &display))
		ft_alloc_error(set, 1);
	set->screen_w = display.w / FACTOR;
	set->screen_h = display.h / FACTOR;
	/* Use the same logical grid for the window, texture and cursor. */
	if (!SDL_CreateWindowAndRenderer(ft_fractal_titles(set->fractal),
			set->screen_w, set->screen_h, 0, &set->window, &set->renderer)
		|| !SDL_SetRenderDrawColor(set->renderer, 0, 0, 0, 255))
		ft_alloc_error(set, 1);
	ft_init_texture(set);
}

static void	ft_handle_event(t_set *set, const SDL_Event *event)
{
	float	scroll;

	if (event->type == SDL_EVENT_KEY_UP)
		ft_keys_handler(event->key.key, set);
	else if (event->type == SDL_EVENT_MOUSE_WHEEL)
	{
		scroll = event->wheel.y;
		if (event->wheel.direction == SDL_MOUSEWHEEL_FLIPPED)
			scroll = -scroll;
		ft_mouse_zoom(set, scroll, (int)event->wheel.mouse_x,
			(int)event->wheel.mouse_y);
	}
	else if ((event->type == SDL_EVENT_MOUSE_BUTTON_DOWN
			|| event->type == SDL_EVENT_MOUSE_BUTTON_UP)
		&& event->button.button == SDL_BUTTON_RIGHT)
		ft_mouse_select(set, (int)event->button.x, (int)event->button.y);
	else if (event->type == SDL_EVENT_RENDER_DEVICE_RESET)
	{
		SDL_DestroyTexture(set->texture);
		ft_init_texture(set);
	}
}

static void	ft_run(t_set *set)
{
	SDL_Event	event;
	bool		redraw;

	ft_render(set);
	while (SDL_WaitEvent(&event))
	{
		redraw = false;
		do
		{
			if (event.type == SDL_EVENT_QUIT
				|| event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED
				|| (event.type == SDL_EVENT_KEY_UP && event.key.key == SDLK_ESCAPE))
				return;
			ft_handle_event(set, &event);
			if (event.type == SDL_EVENT_WINDOW_EXPOSED
				|| event.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
				redraw = true;
		} while (SDL_PollEvent(&event));
		if (redraw || set->status)
			ft_render(set);
	}
	ft_alloc_error(set, 1);
}

int	main(int argc, char *argv[])
{
	t_set	*set;

	set = ft_init_fractol(argc, argv);
	ft_init_video(set);
	set->viewer = ft_init_viewer(set);
	ft_run(set);
	ft_free_all(set);
	return (0);
}
