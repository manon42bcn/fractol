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

/**
 * @brief Clamp a color component to the unsigned byte range.
 * @param[in] value Color component before clamping.
 * @return Clamped component in the range 0 to 255.
 */
static Uint32	ft_color_byte(int value)
{
	if (value < 0)
		return (0);
	if (value > 255)
		return (255);
	return ((Uint32)value);
}

/**
 * @brief Pack four clamped components into a single pixel value.
 * @param[in] t Transparency component.
 * @param[in] r Red component.
 * @param[in] g Green component.
 * @param[in] b Blue component.
 * @return Packed transparency, red, green, and blue value.
 */
int	create_trgb(int t, int r, int g, int b)
{
	return (ft_color_byte(t) << 24 | ft_color_byte(r) << 16
		| ft_color_byte(g) << 8 | ft_color_byte(b));
}

/**
 * @brief Write a pixel, flipping the transparency byte for SDL opacity.
 * @param[out] data Destination image buffer and row pitch.
 * @param[in] x Pixel column.
 * @param[in] y Pixel row.
 * @param[in] color Packed transparency and RGB value.
 */
void	ft_put_pixel(t_data *data, int x, int y, int color)
{
	Uint32	*row;

	row = (Uint32 *)((char *)data->addr + y * data->pitch);
	row[x] = (Uint32)color ^ 0xFF000000u;
}
