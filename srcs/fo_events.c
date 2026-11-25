/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_events.c                                        +:+      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 19:47:42 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 19:47:42 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Change fractal in the direction selected by the Shift modifier.
 * @param[in,out] set Application state receiving the next fractal.
 * @param[in] modifiers Modifier flags captured with the Tab press.
 */
static void	ft_handle_tab(t_set *set, SDL_Keymod modifiers)
{
	if (modifiers & SDL_KMOD_SHIFT)
		ft_cycle_fractal(set, -1);
	else
		ft_cycle_fractal(set, 1);
}

/**
 * @brief Normalize the scroll direction and apply zoom at the cursor.
 * @param[in,out] set Application state containing the view.
 * @param[in] wheel SDL mouse wheel event.
 */
static void	ft_handle_wheel(t_set *set, const SDL_MouseWheelEvent *wheel)
{
	float	scroll;

	scroll = wheel->y;
	if (wheel->direction == SDL_MOUSEWHEEL_FLIPPED)
		scroll = -scroll;
	ft_mouse_zoom(set, scroll, (int)wheel->mouse_x, (int)wheel->mouse_y);
}

/**
 * @brief Apply keyboard, mouse, or renderer-reset events to the active view.
 * @param[in,out] set Application and render state to update.
 * @param[in] event SDL event to dispatch.
 */
void	ft_handle_event(t_set *set, const SDL_Event *event)
{
	if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_TAB
		&& !event->key.repeat)
		ft_handle_tab(set, event->key.mod);
	else if (event->type == SDL_EVENT_KEY_UP)
		ft_keys_handler(event->key.key, set);
	else if (event->type == SDL_EVENT_MOUSE_WHEEL)
		ft_handle_wheel(set, &event->wheel);
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
