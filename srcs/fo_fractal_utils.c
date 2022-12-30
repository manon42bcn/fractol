/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_fractal_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 21:16:39 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:53:38 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int	ft_load_base_color(t_set *set)
{
	static int	base = 0;

	if (base == 5)
		base = 0;
	set->color_scheme = base;
	base++;
	return (0);
}
