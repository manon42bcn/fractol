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

void	ft_free_all(t_set *set)
{
	if (set)
	{
		if (set->texture)
			SDL_DestroyTexture(set->texture);
		if (set->renderer)
			SDL_DestroyRenderer(set->renderer);
		if (set->window)
			SDL_DestroyWindow(set->window);
		free(set->buddhabrot_counts);
		free(set->viewer);
		free(set);
	}
	SDL_Quit();
}

t_set	*ft_init_set(int select, int color_mode)
{
	t_set	*rst;

	rst = calloc(1, sizeof(t_set));
	if (rst == NULL)
		return (NULL);
	rst->fractal = select;
	rst->zoom = 1.0;
	rst->color_scheme = 0;
	rst->palette_span = DEFAULT_PALETTE_SPAN;
	rst->palette_anchor = 1;
	rst->color_mode = color_mode;
	rst->zi = 0.0;
	rst->zr = 0.0;
	rst->max_i = 50;
	rst->esc = 4;
	rst->c_r = -0.8350;
	rst->c_i = -0.2321;
	if (select == PHOENIX)
	{
		rst->c_r = 0.5667L;
		rst->c_i = 0;
	}
	rst->color = 0;
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
	rst->x_width = DEFAULT_VIEW_WIDTH;
	rst->y_height = DEFAULT_VIEW_HEIGHT;
	return (rst);
}
