/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_handlers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 09:25:35 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:54:31 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Pan the view by one keyboard step.
 * @param[in] key SDL arrow key code.
 * @param[in,out] set State containing the view origin.
 */
inline static void	ft_keys_panning_mode(SDL_Keycode key, t_set *set)
{
	if (key == SDLK_UP)
		set->viewer->y_zero += 50;
	else if (key == SDLK_DOWN)
		set->viewer->y_zero -= 50;
	else if (key == SDLK_LEFT)
		set->viewer->x_zero += 50;
	else if (key == SDLK_RIGHT)
		set->viewer->x_zero -= 50;
}

/**
 * @brief Adjust the iteration limit within its supported range.
 * @param[in] key SDL key code for increasing or decreasing the limit.
 * @param[in,out] set State containing the iteration limit.
 */
inline static void	ft_keys_iteration(SDL_Keycode key, t_set *set)
{
	if (key == SDLK_EQUALS || key == SDLK_PLUS || key == SDLK_KP_PLUS)
	{
		if (set->max_i + 10 <= MAX_ITERATIONS)
			set->max_i += 10;
	}
	else if (key == SDLK_MINUS || key == SDLK_KP_MINUS)
	{
		if (set->max_i - 10 >= 5)
			set->max_i -= 10;
	}
}

/**
 * @brief Dispatch a released key to the applicable view or color control.
 * @param[in] key SDL key code from the keyboard event.
 * @param[in,out] set Fractal, view, color, and redraw settings.
 */
void	ft_keys_handler(SDL_Keycode key, t_set *set)
{
	if (ft_keys_special(key, set))
		return ;
	if (key == SDLK_UP || key == SDLK_DOWN || key == SDLK_LEFT
		|| key == SDLK_RIGHT)
		ft_keys_panning_mode(key, set);
	else if (key == SDLK_SPACE)
		ft_load_base_color(set);
	else if (key == SDLK_Q)
		ft_reset_viewer(set);
	else if (key == SDLK_C || key == SDLK_M || key == SDLK_B || key == SDLK_F)
		ft_keys_color_mode(key, set);
	else if (key == SDLK_EQUALS || key == SDLK_PLUS || key == SDLK_KP_PLUS
		|| key == SDLK_MINUS || key == SDLK_KP_MINUS)
		ft_keys_iteration(key, set);
	else if (key == SDLK_I)
		set->esc += 5;
	else if (key == SDLK_O)
	{
		if (set->esc - 5 > 1)
			set->esc -= 5;
	}
	else
		return ;
	set->status = 1;
}
