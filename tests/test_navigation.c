/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_navigation.c                                  +:+      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 19:47:42 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 19:47:42 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include <SDL3/SDL_main.h>
#include <assert.h>
#include <stdio.h>

#define TEST_WIDTH 160
#define TEST_HEIGHT 100

/**
 * @brief Verify that each supported startup mode selects Mandelbrot.
 * @return Initialized Mandelbrot state in color mode for the render test.
 */
static t_set	*test_startup(void)
{
	static char *const	modes[] = {"color", "mono", "bw", "full"};
	char				*args[3];
	t_set				*set;
	int					i;

	args[0] = "fractol";
	args[1] = NULL;
	args[2] = NULL;
	set = ft_init_fractol(1, args);
	assert(set->fractal == MALDELBROT && set->color_mode == 3);
	ft_free_all(set);
	i = 0;
	while (i < 4)
	{
		args[1] = modes[i];
		set = ft_init_fractol(2, args);
		assert(set->fractal == MALDELBROT && set->color_mode == i + 1);
		ft_free_all(set);
		i++;
	}
	args[1] = modes[0];
	return (ft_init_fractol(2, args));
}

/**
 * @brief Dispatch a Tab press through the application's event handler.
 * @param[in,out] set Application state receiving the event.
 * @param[in] modifiers SDL keyboard modifier flags.
 * @param[in] repeat Whether the key press is an auto-repeat.
 */
static void	send_tab(t_set *set, SDL_Keymod modifiers, bool repeat)
{
	SDL_Event	event;

	event = (SDL_Event){.type = SDL_EVENT_KEY_DOWN};
	event.key.key = SDLK_TAB;
	event.key.mod = modifiers;
	event.key.repeat = repeat;
	ft_handle_event(set, &event);
}

/**
 * @brief Check that Q restores the small-offset framing and visible detail.
 * @param[in,out] set State currently showing the 0.001 rational variant.
 */
static void	test_small_offset_view(t_set *set)
{
	Uint32		pixels[TEST_WIDTH * TEST_HEIGHT];
	t_data		image;
	SDL_Event	event;
	int			interior;
	int			i;

	assert(fabsl(set->viewer->x_width - 0.0048L) < 1e-9L);
	set->viewer->x_width = DEFAULT_VIEW_WIDTH;
	set->viewer->y_height = DEFAULT_VIEW_HEIGHT;
	event = (SDL_Event){.type = SDL_EVENT_KEY_UP};
	event.key.key = SDLK_Q;
	ft_handle_event(set, &event);
	assert(fabsl(set->viewer->x_width - 0.0048L) < 1e-9L);
	assert(fabsl(set->viewer->y_height - 0.003L) < 1e-9L);
	image = (t_data){.addr = pixels, .pitch = TEST_WIDTH * sizeof(*pixels)};
	ft_iterator(set, ft_rational_offset_0001, &image);
	interior = 0;
	i = 0;
	while (i < TEST_WIDTH * TEST_HEIGHT)
	{
		interior += pixels[i] == 0xff000000u;
		i++;
	}
	assert(interior > TEST_WIDTH * TEST_HEIGHT / 100);
	assert(interior < TEST_WIDTH * TEST_HEIGHT);
}

/**
 * @brief Render every fractal and verify circular navigation in both directions.
 * @param[in,out] set Initialized application and SDL renderer.
 */
static void	test_navigation(t_set *set)
{
	const char	*title;
	int			step;

	step = 1;
	while (step <= FRACTAL_COUNT)
	{
		send_tab(set, SDL_KMOD_NONE, false);
		assert(set->fractal == step % FRACTAL_COUNT);
		if (set->fractal == RATIONAL_OFFSET_0001)
			test_small_offset_view(set);
		ft_render(set);
		title = ft_fractal_titles(set->fractal);
		assert(SDL_strncmp(SDL_GetWindowTitle(set->window), title,
				SDL_strlen(title)) == 0);
		step++;
	}
	step = FRACTAL_COUNT;
	while (step > 0)
	{
		send_tab(set, SDL_KMOD_SHIFT, false);
		assert(set->fractal == --step);
	}
	send_tab(set, SDL_KMOD_NONE, true);
	assert(set->fractal == MALDELBROT);
}

/**
 * @brief Run the render and input tests with a small hidden SDL window.
 * @param[in] argc Argument count forwarded by SDL.
 * @param[in] argv Argument vector forwarded by SDL.
 * @return Zero if all assertions pass.
 */
int	main(int argc, char *argv[])
{
	t_set	*set;

	(void)argc;
	(void)argv;
	set = test_startup();
	assert(SDL_Init(SDL_INIT_VIDEO));
	set->screen_w = TEST_WIDTH;
	set->screen_h = TEST_HEIGHT;
	assert(SDL_CreateWindowAndRenderer("Fractol tests", TEST_WIDTH, TEST_HEIGHT,
			SDL_WINDOW_HIDDEN, &set->window, &set->renderer));
	assert(SDL_SetRenderDrawColor(set->renderer, 0, 0, 0, 255));
	ft_init_texture(set);
	set->viewer = ft_init_viewer(set);
	test_navigation(set);
	ft_free_all(set);
	puts("Navigation and render tests passed.");
	return (0);
}
