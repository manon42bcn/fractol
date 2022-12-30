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

	y_pointer -= 30;
	zoom_x = (x_pointer - set->viewer->x_zero)
		* (set->viewer->zoom_f / (set->screen_w / set->screen_h));
	zoom_y = (y_pointer - set->viewer->y_zero)
		* (set->viewer->zoom_f / (set->screen_w / set->screen_h));
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

	y_pointer -= 30;
	zoom_x = (x_pointer - set->viewer->x_zero)
		* (set->viewer->zoom_f / (set->screen_w / set->screen_h));
	zoom_y = (y_pointer - set->viewer->y_zero)
		* (set->viewer->zoom_f / (set->screen_w / set->screen_h));
	set->zoom -= set->viewer->zoom_f;
	set->viewer->x_width /= (set->viewer->zoom_f);
	set->viewer->y_height /= (set->viewer->zoom_f);
	set->viewer->x_zero -= (zoom_x * -1);
	set->viewer->y_zero -= (zoom_y * -1);
}

int	ft_mouse_handler(int mouse_code, int x, int y, t_set *set)
{
	int	x_pointer;
	int	y_pointer;

	mlx_mouse_get_pos(set->mlx_win, &x_pointer, &y_pointer);
	if (mouse_code == 4 && x)
		ft_zoom_in(set, x_pointer, y_pointer);
	if (mouse_code == 5 && y)
		ft_zoom_out(set, x_pointer, y_pointer);
	if (mouse_code == 2 && (set->fractal == 2 ||
		set->fractal == 12 || set->fractal == 13))
	{
		set->c_r = (x_pointer - set->viewer->x_zero)
			* (set->viewer->x_width / set->screen_w);
		set->c_i = (y_pointer - set->viewer->y_zero)
			* (set->viewer->y_height / set->screen_h);
	}
	set->status = 1;
	ft_render(set);
	return (1);
}
