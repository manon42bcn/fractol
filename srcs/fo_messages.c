/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_messages.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 12:17:07 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:53:16 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	ft_alloc_error(t_set *set, int error)
{
	if (error == 0)
		ft_putendl_fd("\x1b[31m\e[1mError: allocating set.\e[0m", 1);
	else if (error == 1)
		ft_putendl_fd("\x1b[31m\e[1mError: MLX error.\e[0m", 1);
	else if (error == 2)
		ft_putendl_fd("\x1b[31m\e[1mError: MLX image error.\e[0m", 1);
	else if (error == 3)
		ft_putendl_fd("\x1b[31m\e[1mError: allocating viewer.\e[0m", 1);
	ft_free_all(set);
	exit(0);
}

void	ft_input_messages(int select)
{
	ft_putendl_fd("\x1b[31m\e[1mError: \e[0m", 1);
	if (select == 1)
		ft_putendl_fd("Missing argument.\x1b[0m", 1);
	else if (select == 2)
		ft_putendl_fd("Wrong argument!.\x1b[0m", 1);
	else if (select == 3)
		ft_putendl_fd("Too many arguments!.\x1b[0m", 1);
	ft_putendl_fd("\e[32m\e[1mIntructions:\e[0m", 1);
	ft_putstr_fd("Run: ./fractol [fractal] [options]", 1);
	ft_putendl_fd(" [params] or --help to get more details.\x1b[0m", 1);
	exit(0);
}

void	ft_help(void)
{
	ft_putendl_fd("\x1b[35m------------------------------------------", 1);
	ft_putendl_fd("\e[1m\e[34mHELP - OPTIONS\x1b[0m", 1);
	ft_putstr_fd("\e[44m[fractal]\e[0m\e[49m: mandelbrot, julia, ", 1);
	ft_putendl_fd(" burningship, drop, bird, worm, spider, hiper.", 1);
	ft_putendl_fd("\e[44m[options]\e[0m\e[49m: color, mono, bw, full.", 1);
	ft_putendl_fd("\e[44m[params] (julia set only)\e[0m\e[49m: ", 1);
	ft_putendl_fd("write two constant values for julia set.	", 1);
	ft_putendl_fd("format: rC iC (default: -0.8350 -0.2321)", 1);
	ft_putendl_fd("\x1b[35m------------------------------------------", 1);
	ft_putendl_fd("\e[34mUSE:\x1b[0m", 1);
	ft_putstr_fd("Zoom in: mouse scroll up | ", 1);
	ft_putendl_fd(" Zoom out: mouse scroll down. | Q: reset zoom.", 1);
	ft_putendl_fd("Up - down - left - right: move current view.", 1);
	ft_putendl_fd("Mouse second button: change Julia set values.", 1);
	ft_putstr_fd("(+) and (-): increases and reduces max iteration.", 1);
	ft_putstr_fd("I and ): increases and reduces escape point.", 1);
	ft_putstr_fd(" More iterations means more details and lag.", 1);
	ft_putendl_fd("C: swich to color mode.", 1);
	ft_putendl_fd("M: swich to mono mode.", 1);
	ft_putendl_fd("B: swich to black and white.", 1);
	ft_putendl_fd("F: swich to full mode (laggy, but interesting).", 1);
	ft_putendl_fd("SPACE BAR: change color scheme.", 1);
	ft_putendl_fd("ESC: exit fract-ol.", 1);
	exit(0);
}
