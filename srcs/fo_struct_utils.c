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

/**
 * @brief Set the initial parameters for a selected fractal and request redraw.
 * @param[in,out] set Application state to configure.
 * @param[in] fractal Fractal index to select.
 */
static void	ft_fractal_defaults(t_set *set, int fractal)
{
	set->fractal = fractal;
	set->zoom = 1.0;
	set->max_i = 50;
	set->esc = 4;
	set->c_r = -0.8350;
	set->c_i = -0.2321;
	if (fractal == PHOENIX)
	{
		set->c_r = 0.5667L;
		set->c_i = 0;
	}
	set->tricorn_julia = false;
	set->color_scheme = 0;
	set->buddhabrot_recolor = false;
	set->status = 1;
}

/**
 * @brief Allocate the state and initialize fractal and palette defaults.
 * @param[in] select Initial fractal index.
 * @param[in] color_mode Initial color mode identifier.
 * @return Allocated state, or NULL if allocation fails.
 */
t_set	*ft_init_set(int select, int color_mode)
{
	t_set	*rst;

	rst = calloc(1, sizeof(t_set));
	if (rst == NULL)
		return (NULL);
	ft_fractal_defaults(rst, select);
	rst->palette_span = DEFAULT_PALETTE_SPAN;
	rst->palette_anchor = 1;
	rst->color_mode = color_mode;
	return (rst);
}

/**
 * @brief Center and size a view for the currently selected fractal.
 * @param[out] viewer View geometry to initialize.
 * @param[in] set Screen dimensions and selected fractal.
 */
void	ft_viewer_defaults(t_viewer *viewer, const t_set *set)
{
	viewer->zoom_f = 0.50f;
	viewer->x_zero = (long double)set->screen_w / 2.0;
	viewer->y_zero = (long double)set->screen_h / 2.0;
	viewer->x_width = DEFAULT_VIEW_WIDTH;
	viewer->y_height = DEFAULT_VIEW_HEIGHT;
	if (set->fractal == RATIONAL_OFFSET_0001)
	{
		viewer->x_width *= RATIONAL_SMALL_OFFSET;
		viewer->y_height *= RATIONAL_SMALL_OFFSET;
	}
}

/**
 * @brief Allocate a view and initialize it for the active fractal.
 * @param[in] set Screen dimensions and fractal used for initialization.
 * @return Allocated view on success.
 */
t_viewer	*ft_init_viewer(t_set *set)
{
	t_viewer	*rst;

	rst = (t_viewer *)malloc(sizeof(t_viewer));
	if (rst == NULL)
		ft_alloc_error(set, 3);
	ft_viewer_defaults(rst, set);
	return (rst);
}

/**
 * @brief Advance or reverse the circular fractal selection and reset its view.
 * @param[in,out] set Active fractal, view geometry, and redraw state.
 * @param[in] direction Direction of travel, either 1 or -1.
 */
void	ft_cycle_fractal(t_set *set, int direction)
{
	ft_fractal_defaults(set,
		(set->fractal + direction + FRACTAL_COUNT) % FRACTAL_COUNT);
	ft_viewer_defaults(set->viewer, set);
}
