/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_input_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/30 09:49:16 by mporras-          #+#    #+#             */
/*   Updated: 2022/10/23 01:53:27 by mporras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static int	ft_input_reader(char *s)
{
	int test;

	test = ft_fractal_dir(s);
	if (test == 0)
		ft_input_messages(2);
	return (test);
}

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
	ft_input_messages(2);
	return (0);
}

static int	ft_julia_const(t_set *set, int argc, char *argv[])
{
	if (argc == 4)
	{
		set->c_r = ft_strfloat(argv[2]);
		set->c_i = ft_strfloat(argv[3]);
	}
	if (argc == 5)
	{
		set->c_r = ft_strfloat(argv[3]);
		set->c_i = ft_strfloat(argv[4]);
	}
	return (1);
}

t_set	*ft_init_fractol(int argc, char *argv[])
{
	t_set	*rst;
	int		select;
	int		color_mode;

	if (argc == 1)
		ft_input_messages(1);
	if (argc > 5)
		ft_input_messages(3);
	select = ft_input_reader(argv[1]);
	if (select != 2 && argc > 3)
		ft_input_messages(3);
	if (argc == 3)
		color_mode = ft_input_option(argv[2]);
	if (argc == 2)
		color_mode = 3;
	if (select == 2 && argc == 4)
		color_mode = 3;
	if (select == 2 && argc == 5)
		color_mode = ft_input_option(argv[2]);
	rst = ft_init_set(select, color_mode);
	if (rst == NULL)
		ft_alloc_error(rst, 0);
	if (select == 2)
		ft_julia_const(rst, argc, argv);
	return (rst);
}
