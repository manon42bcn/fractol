/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_struct_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 13:13:59 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:59:21 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	ft_free_all(t_set *set)
{
	if (set)
	{
		if (set->mlx)
			mlx_destroy_window(set->mlx, set->mlx_win);
		if (set->viewer)
			free(set->viewer);
		set->mlx = NULL;
		set->mlx_win = NULL;
		set->img = NULL;
		set->viewer = NULL;
		free(set);
	}
	exit(0);
	return (1);
}

t_set	*ft_init_set(int select, int color_mode)
{
	t_set	*rst;

	rst = (t_set *)malloc(sizeof(t_set));
	if (rst == NULL)
		return (NULL);
	rst->fractal = select;
	rst->zoom = 1.0;
	rst->color_scheme = 0;
	rst->color_mode = color_mode;
	rst->mlx = mlx_init();
	rst->mlx_win = NULL;
	rst->img = NULL;
	rst->zi = 0.0;
	rst->zr = 0.0;
	rst->max_i = 50;
	rst->esc = 4;
	rst->c_r = -0.8350;
	rst->c_i = -0.2321;
	rst->color = 0;
	mlx_get_screen_size(rst->mlx, &rst->screen_w, &rst->screen_h);
	rst->screen_w = rst->screen_w / FACTOR;
	rst->screen_h = rst->screen_h / FACTOR;
	ft_load_base_color(rst);
	rst->status = 1;
	return (rst);
}

t_viewer	*ft_init_viewer(t_set *set)
{
	t_viewer	*rst;

	rst = (t_viewer *)malloc(sizeof(t_viewer));
	if (rst == NULL)
		ft_alloc_error(set, 3);
	rst->zoom_f = 0.50f;
	rst->x_zero = (long double)set->screen_w / 2.0;
	rst->y_zero = (long double)set->screen_h / 2.0;
	rst->x_width = 4.8f;
	rst->y_height = 3.0f;
	return (rst);
}
