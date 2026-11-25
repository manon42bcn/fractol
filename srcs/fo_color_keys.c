/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_color_keys.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 09:25:35 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:54:31 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Select a display mode from a color shortcut.
 * @param[in] key SDL key code for the shortcut.
 * @param[in,out] set State containing the active color mode.
 */
void	ft_keys_color_mode(SDL_Keycode key, t_set *set)
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

/**
 * @brief Apply a classic palette shortcut when the current mode supports it.
 * @param[in] key SDL key code for the shortcut.
 * @param[in,out] set Palette selection and gradient settings.
 * @return True if the shortcut was handled.
 */
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

/**
 * @brief Advance the interpolation mode and request a color update.
 * @param[in,out] set State containing the active interpolation mode.
 */
static void	ft_keys_interpolation(t_set *set)
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
}

/**
 * @brief Handle Buddhabrot shortcuts that preserve the orbit histogram.
 * @param[in] key SDL key code from the keyboard event.
 * @param[in,out] set Buddhabrot exposure and palette state.
 * @return True if the key was handled or suppressed for Buddhabrot.
 */
static bool	ft_keys_buddhabrot(SDL_Keycode key, t_set *set)
{
	if (key == SDLK_S)
	{
		set->buddhabrot_original_exposure = !set->buddhabrot_original_exposure;
		set->buddhabrot_recolor = true;
	}
	else if (key == SDLK_SPACE)
	{
		if (set->color_mode == 1)
			ft_nebulabrot_next_palette(set);
	}
	else if (key == SDLK_C || key == SDLK_B)
	{
		ft_keys_color_mode(key, set);
		set->buddhabrot_recolor = true;
	}
	else if (key != SDLK_M && key != SDLK_F)
		return (false);
	return (true);
}

/**
 * @brief Handle palette shortcuts and keys restricted by the active fractal.
 * @param[in] key SDL key code from the keyboard event.
 * @param[in,out] set Fractal, palette, and redraw state.
 * @return True if the key was handled or suppressed.
 */
bool	ft_keys_special(SDL_Keycode key, t_set *set)
{
	if (key == SDLK_D && set->fractal == MALDELBROT && set->color_mode == 1)
	{
		set->distance_shading = !set->distance_shading;
		set->status = 1;
		return (true);
	}
	if (key == SDLK_L && set->fractal != NEWTON)
	{
		ft_keys_interpolation(set);
		return (true);
	}
	if (set->fractal == BUDDHABROT)
		return (ft_keys_buddhabrot(key, set));
	if (set->fractal == NEWTON && (key == SDLK_SPACE || key == SDLK_C
			|| key == SDLK_M || key == SDLK_B || key == SDLK_F
			|| key == SDLK_I || key == SDLK_O))
		return (true);
	if (!ft_keys_palette(key, set))
		return (false);
	set->status = 1;
	return (true);
}
