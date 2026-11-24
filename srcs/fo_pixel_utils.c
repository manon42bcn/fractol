/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_pixel_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 16:39:27 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:53:02 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static Uint32	ft_color_byte(int value)
{
	if (value < 0)
		return (0);
	if (value > 255)
		return (255);
	return ((Uint32)value);
}

int	create_trgb(int t, int r, int g, int b)
{
	return (ft_color_byte(t) << 24 | ft_color_byte(r) << 16
		| ft_color_byte(g) << 8 | ft_color_byte(b));
}

void	ft_put_pixel(t_data *data, int x, int y, int color)
{
	Uint32	*row;

	row = (Uint32 *)((char *)data->addr + y * data->pitch);
	/* The existing high byte is transparency. SDL expects opacity. */
	row[x] = (Uint32)color ^ 0xFF000000u;
}
