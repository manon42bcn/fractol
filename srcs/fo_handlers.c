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

inline static void	ft_keys_color_mode(SDL_Keycode key, t_set *set)
{
	if (key == SDLK_C)
		set->color_mode = 1;
	if (key == SDLK_M)
		set->color_mode = 2;
	if (key == SDLK_B)
		set->color_mode = 3;
	if (key == SDLK_F)
		set->color_mode = 4;
}

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

static bool	ft_keys_palette(SDL_Keycode key, t_set *set)
{
	if (set->color_mode != 1 || set->fractal == NEWTON
		|| set->fractal == BUDDHABROT)
		return (false);
	if (key == SDLK_J)
		set->palette_span = SDL_max(MIN_PALETTE_SPAN, set->palette_span - 1);
	else if (key == SDLK_K)
		set->palette_span = SDL_min(MAX_PALETTE_SPAN, set->palette_span + 1);
	else if (key == SDLK_P)
		set->palette_single_color = !set->palette_single_color;
	else if (key == SDLK_N && set->palette_single_color)
		set->palette_anchor = (set->palette_anchor + 1) % PALETTE_COLOR_COUNT;
	else
		return (false);
	return (true);
}

void	ft_keys_handler(SDL_Keycode key, t_set *set)
{
	if (key == SDLK_L && set->fractal != NEWTON)
	{
		set->palette_interpolation = (set->palette_interpolation + 1)
			% PALETTE_INTERPOLATION_COUNT;
		if (set->color_mode == 1)
		{
			if (set->fractal == BUDDHABROT)
				set->buddhabrot_recolor = true;
			else
				set->status = 1;
		}
		return;
	}
	if (set->fractal == BUDDHABROT && key == SDLK_S)
	{
		set->buddhabrot_original_exposure = !set->buddhabrot_original_exposure;
		set->buddhabrot_recolor = true;
		return;
	}
	if (set->fractal == BUDDHABROT && key == SDLK_SPACE)
	{
		if (set->color_mode == 1)
			ft_nebulabrot_next_palette(set);
		return;
	}
	if (set->fractal == BUDDHABROT && (key == SDLK_C || key == SDLK_B))
	{
		ft_keys_color_mode(key, set);
		set->buddhabrot_recolor = true;
		return;
	}
	if ((set->fractal == NEWTON || set->fractal == BUDDHABROT)
		&& (key == SDLK_SPACE || key == SDLK_C || key == SDLK_M
			|| key == SDLK_B || key == SDLK_F))
		return;
	if (set->fractal == NEWTON && (key == SDLK_I || key == SDLK_O))
		return;
	if (ft_keys_palette(key, set))
	{
		set->status = 1;
		return;
	}
	if (key == SDLK_UP || key == SDLK_DOWN || key == SDLK_LEFT || key == SDLK_RIGHT)
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
		return;
	set->status = 1;
}
