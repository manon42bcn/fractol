/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_mouse_zoom.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 09:25:35 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:52:57 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	ft_reset_viewer(t_set *set)
{
	set->zoom = 1.0f;
	set->viewer->zoom_f = 0.50f;
	set->viewer->x_zero = set->screen_w / 2.0;
	set->viewer->y_zero = set->screen_h / 2.0;
	set->viewer->x_width = 5.1;
	set->viewer->y_height = 3;
}

inline static void	ft_zoom_in(t_set *set, int x_pointer, int y_pointer)
{
	long double	zoom_x;
	long double	zoom_y;
	int				aspect;

	aspect = set->screen_w / set->screen_h;
	if (aspect < 1)
		aspect = 1;
	zoom_x = (x_pointer - set->viewer->x_zero)
		* (set->viewer->zoom_f / aspect);
	zoom_y = (y_pointer - set->viewer->y_zero)
		* (set->viewer->zoom_f / aspect);
	set->zoom += set->viewer->zoom_f;
	set->viewer->x_width /= (1 + set->viewer->zoom_f);
	set->viewer->y_height /= (1 + set->viewer->zoom_f);
	set->viewer->x_zero += (zoom_x * -1);
	set->viewer->y_zero += (zoom_y * -1);
}

inline static void	ft_zoom_out(t_set *set, int x_pointer, int y_pointer)
{
	long double	zoom_x;
	long double	zoom_y;
	int				aspect;

	aspect = set->screen_w / set->screen_h;
	if (aspect < 1)
		aspect = 1;
	zoom_x = (x_pointer - set->viewer->x_zero)
		* (set->viewer->zoom_f / aspect);
	zoom_y = (y_pointer - set->viewer->y_zero)
		* (set->viewer->zoom_f / aspect);
	set->zoom -= set->viewer->zoom_f;
	set->viewer->x_width /= (set->viewer->zoom_f);
	set->viewer->y_height /= (set->viewer->zoom_f);
	set->viewer->x_zero -= (zoom_x * -1);
	set->viewer->y_zero -= (zoom_y * -1);
}

void	ft_mouse_zoom(t_set *set, float scroll, int x, int y)
{
	if (scroll > 0)
		ft_zoom_in(set, x, y);
	else if (scroll < 0)
		ft_zoom_out(set, x, y);
	else
		return;
	set->status = 1;
}

void	ft_mouse_select(t_set *set, int x, int y)
{
	if (set->fractal != JULIA && set->fractal != JULIA3
		&& set->fractal != JULIA7)
		return;
	set->c_r = (x - set->viewer->x_zero)
		* (set->viewer->x_width / set->screen_w);
	set->c_i = (y - set->viewer->y_zero)
		* (set->viewer->y_height / set->screen_h);
	set->status = 1;
}
