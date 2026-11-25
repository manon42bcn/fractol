/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_messages.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 12:17:07 by mporras-          #+#    #+#             */
/*   Updated: 2024/09/22 21:39:35 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include <stdio.h>

/**
 * @brief Report an SDL or allocation error, release resources, and exit.
 * @param[in,out] set Partially initialized state, which may be NULL.
 * @param[in] error Application error code.
 */
void	ft_alloc_error(t_set *set, int error)
{
	if (error == 0)
		ft_putendl_fd("\x1b[31m\e[1mError: allocating set.\e[0m", 1);
	else if (error == 1 || error == 2)
		fprintf(stderr, "SDL3: %s\n", SDL_GetError());
	else if (error == 3)
		ft_putendl_fd("\x1b[31m\e[1mError: allocating viewer.\e[0m", 1);
	else if (error == 4)
		ft_putendl_fd("Error: allocating Buddhabrot histogram.", 2);
	ft_free_all(set);
	exit(EXIT_FAILURE);
}

/**
 * @brief Print valid command-line usage and exit with failure.
 */
void	ft_input_messages(void)
{
	ft_putendl_fd("Usage: ./fractol [color|mono|bw|full]"
		" or ./fractol --help", 2);
	exit(EXIT_FAILURE);
}

/**
 * @brief Print the color modes and palette shortcuts.
 */
static void	ft_help_colors(void)
{
	ft_putendl_fd("C: swich to color mode.", 1);
	ft_putendl_fd("M: swich to mono mode.", 1);
	ft_putendl_fd("B: swich to black and white.", 1);
	ft_putendl_fd("F: swich to full mode (laggy, but interesting).", 1);
	ft_putendl_fd("SPACE BAR: change color scheme.", 1);
	ft_putendl_fd("Buddhabrot: C shows orbit colors, B shows"
		" density in grayscale.", 1);
	ft_putendl_fd("Nebulabrot: SPACE BAR cycles palettes without"
		" restarting sampling.", 1);
	ft_putendl_fd("Buddhabrot: S toggles shadow suppression.", 1);
	ft_putendl_fd("L: cycle sRGB, linear-light and Oklab"
		" interpolation in color mode.", 1);
	ft_putendl_fd("Classic color palettes: J shortens"
		" transitions, K lengthens them.", 1);
	ft_putendl_fd("P switches between the full palette and one"
		" color. N picks the next color.", 1);
}

/**
 * @brief Print usage and controls, then exit successfully.
 */
void	ft_help(void)
{
	ft_putendl_fd("Usage: ./fractol [color|mono|bw|full]", 1);
	ft_putendl_fd("Fractol starts in Mandelbrot. Default color mode: bw.", 1);
	ft_putendl_fd("\x1b[35m------------------------------------------", 1);
	ft_putendl_fd("\e[34mUSE:\x1b[0m", 1);
	ft_putstr_fd("Zoom in: mouse scroll up | ", 1);
	ft_putendl_fd(" Zoom out: mouse scroll down. | Q: reset zoom.", 1);
	ft_putendl_fd("Up - down - left - right: move current view.", 1);
	ft_putendl_fd("Tab: next fractal. Shift+Tab: previous fractal.", 1);
	ft_putendl_fd("Right mouse button: change the Julia or Phoenix constant.",
		1);
	ft_putendl_fd("Tricorn: right-click to explore anti-Julia."
		" Q returns to the Tricorn.", 1);
	ft_putstr_fd("(+) and (-): increases and reduces max iteration.", 1);
	ft_putstr_fd("I and ): increases and reduces escape point.", 1);
	ft_putstr_fd(" More iterations means more details and lag.", 1);
	ft_help_colors();
	ft_putendl_fd("ESC: exit fract-ol.", 1);
	exit(0);
}
