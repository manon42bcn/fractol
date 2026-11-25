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

/**
 * @brief Wait for an event or a Buddhabrot sampling deadline.
 * @param[in,out] set Application state used for sampling and error cleanup.
 * @param[out] event Event received when the function returns true.
 * @return True when an event was received, or false on a sampling timeout.
 */
static bool	ft_wait_event(t_set *set, SDL_Event *event)
{
	if (ft_buddhabrot_pending(set))
		return (SDL_WaitEventTimeout(event, 16));
	if (!SDL_WaitEvent(event))
		ft_alloc_error(set, 1);
	return (true);
}

/**
 * @brief Process the current event and drain the queue before redrawing.
 * @param[in,out] set Application state used for input and rendering.
 * @param[in,out] event First event, reused for the rest of the queue.
 * @return False if a quit event ended the session, or true to continue.
 */
static bool	ft_process_events(t_set *set, SDL_Event *event)
{
	bool	redraw;

	redraw = false;
	while (true)
	{
		if (event->type == SDL_EVENT_QUIT
			|| event->type == SDL_EVENT_WINDOW_CLOSE_REQUESTED
			|| (event->type == SDL_EVENT_KEY_UP
				&& event->key.key == SDLK_ESCAPE))
			return (false);
		ft_handle_event(set, event);
		if (event->type == SDL_EVENT_WINDOW_EXPOSED
			|| event->type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
			redraw = true;
		if (!SDL_PollEvent(event))
			break ;
	}
	if (redraw || set->status || set->buddhabrot_recolor
		|| ft_buddhabrot_pending(set))
		ft_render(set);
	return (true);
}

/**
 * @brief Run the event loop until the window closes or Escape is pressed.
 * @param[in,out] set Application state used for input and rendering.
 */
static void	ft_run(t_set *set)
{
	SDL_Event	event;

	ft_render(set);
	while (true)
	{
		if (!ft_wait_event(set, &event))
			ft_render(set);
		else if (!ft_process_events(set, &event))
			return ;
	}
}

/**
 * @brief Initialize Fractol, run the event loop, and release resources.
 * @param[in] argc Number of command-line arguments.
 * @param[in] argv Command-line argument vector.
 * @return Zero after normal shutdown.
 */
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
