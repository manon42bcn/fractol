/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 15:03:57 by mporras-          #+#    #+#             */
/*   Updated: 2024/09/22 22:44:01 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void    ft_fractol_manager(t_set *set)
{
	t_builders build[] = {&ft_mandelbrot_test, &ft_julia_test,
						  &ft_burningship_test, &ft_drop_test,
						  &ft_bird_test,&ft_spider_test,
						  &ft_worm_test, &ft_hiper_test,
						  &ft_mandelbrot_test3, &ft_mandelbrot_test7,
						  &ft_julia_test3, &ft_julia_test7,
						  &ft_drop_test7, NULL};

    ft_iterator(set, build[set->fractal]);
}

int	ft_render(t_set *set)
{
	if (set->status == 1)
	{
		set->status = 0;
		ft_fractol_manager(set);
	}
	mlx_put_image_to_window(set->mlx, set->mlx_win, set->img->img, 0, 0);
	return (1);
}

int	main(int argc, char *argv[])
{
	t_set	*set;
	t_data	img;

	set = ft_init_fractol(argc, argv);
	if (set->mlx == NULL)
		ft_alloc_error(set, 1);
	set->mlx_win = mlx_new_window(set->mlx, set->screen_w,
			set->screen_h, ft_fractal_titles(set->fractal));
	if (set->mlx_win == NULL)
		ft_alloc_error(set, 1);
	img.img = mlx_new_image(set->mlx, set->screen_w, set->screen_h);
	if (img.img == NULL)
		ft_alloc_error(set, 2);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel,
			&img.line_length, &img.endian);
	set->img = &img;
	set->viewer = ft_init_viewer(set);
	mlx_key_hook(set->mlx_win, ft_keys_handler, set);
	mlx_hook(set->mlx_win, 4, 1L << 2, ft_mouse_handler, set);
	mlx_hook(set->mlx_win, 5, 1L << 3, ft_mouse_handler, set);
	mlx_hook(set->mlx_win, 17, 0L, ft_window_handler, set);
	mlx_loop_hook(set->mlx, ft_render, set);
	mlx_loop(set->mlx);
	return (0);
}
