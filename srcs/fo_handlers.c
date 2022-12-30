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

inline static void	ft_keys_color_mode(int key, t_set *set)
{
	if (key == K_C)
		set->color_mode = 1;
	if (key == K_M)
		set->color_mode = 2;
	if (key == K_B)
		set->color_mode = 3;
	if (key == K_F)
		set->color_mode = 4;
}

inline static void	ft_keys_panning_mode(int key, t_set *set)
{
	if (key == K_UP)
		set->viewer->y_zero += 50;
	else if (key == K_DOWN)
		set->viewer->y_zero -= 50;
	else if (key == K_LEFT)
		set->viewer->x_zero += 50;
	else if (key == K_RIGHT)
		set->viewer->x_zero -= 50;
}

inline static void	ft_keys_iteration(int key, t_set *set)
{
	if (key == K_PLUS || key == K_PAD_PLUS)
	{
		if (set->max_i + 10 <= 1000)
			set->max_i += 10;
	}
	else if (key == K_MINUS || key == K_PAD_MINUS)
	{
		if (set->max_i - 10 >= 5)
			set->max_i -= 10;
	}
}

int	ft_keys_handler(int key, t_set *set)
{
	if (key == K_ESC)
		ft_free_all(set);
	else if (key == K_UP || key == K_DOWN || key == K_LEFT || key == K_RIGHT)
		ft_keys_panning_mode(key, set);
	else if (key == K_SPACE)
		ft_load_base_color(set);
	else if (key == K_Q)
		ft_reset_viewer(set);
	else if (key == K_C || key == K_M || key == K_B || key == K_F)
		ft_keys_color_mode(key, set);
	else if (key == K_PLUS || key == K_PAD_PLUS
		|| key == K_MINUS || key == K_PAD_MINUS)
		ft_keys_iteration(key, set);
	else if (key == K_I)
		set->esc += 5;
	else if (key == K_O)
	{
		if (set->esc - 5 > 1)
			set->esc -= 5;
	}
	set->status = 1;
	ft_render(set);
	return (0);
}

int	ft_window_handler(t_set *set)
{
	ft_free_all(set);
	return (0);
}
