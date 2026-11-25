/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_input_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:49:16 by mporras-          #+#    #+#             */
/*   Updated: 2024/09/22 22:27:08 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Parse the initial color mode or report invalid usage and exit.
 * @param[in] s Command-line mode name.
 * @return Numeric identifier of the selected color mode.
 */
static int	ft_input_option(char *s)
{
	if (ft_strncmp("color", s, 5) == 0)
		return (1);
	if (ft_strncmp("mono", s, 5) == 0)
		return (2);
	if (ft_strncmp("bw", s, 5) == 0)
		return (3);
	if (ft_strncmp("full", s, 5) == 0)
		return (4);
	ft_input_messages();
	return (0);
}

/**
 * @brief Parse startup options and allocate the initial Mandelbrot state.
 * @param[in] argc Number of command-line arguments.
 * @param[in] argv Command-line argument vector.
 * @return Allocated application state on successful startup.
 */
t_set	*ft_init_fractol(int argc, char *argv[])
{
	t_set	*rst;
	int		color_mode;

	if (argc > 2)
		ft_input_messages();
	color_mode = 3;
	if (argc == 2)
	{
		if (ft_strncmp("--help", argv[1], sizeof("--help")) == 0)
			ft_help();
		color_mode = ft_input_option(argv[1]);
	}
	rst = ft_init_set(MALDELBROT, color_mode);
	if (rst == NULL)
		ft_alloc_error(rst, 0);
	return (rst);
}
