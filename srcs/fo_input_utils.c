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

static int	ft_input_reader(char *s)
{
	int test;

	test = ft_fractal_dir(s);
	if (test == NO_FRACTAL)
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

void    ft_checking_input(char *argv[])
{
    int i;

    i = 0;
    while (argv[++i])
    {
        if (ft_strict_cmp(argv[i], "--help") == 0)
        {
            ft_help();
            exit(0);
        }
    }
}

t_set	*ft_init_fractol(int argc, char *argv[])
{
	t_set	*rst;
	int		select;
	int		color_mode;

    ft_checking_input(argv);
	if (argc == 1)
		ft_input_messages(1);
	if (argc > 5)
		ft_input_messages(3);
	select = ft_input_reader(argv[1]);
	if (select != JULIA && argc > 3)
		ft_input_messages(3);
	if (argc == 3)
		color_mode = ft_input_option(argv[2]);
	if (argc == 2)
		color_mode = 3;
	if (select == JULIA && argc == 4)
		color_mode = 3;
	if (select == JULIA && argc == 5)
		color_mode = ft_input_option(argv[2]);
	rst = ft_init_set(select, color_mode);
	if (rst == NULL)
		ft_alloc_error(rst, 0);
	if (select == JULIA)
		ft_julia_const(rst, argc, argv);
	return (rst);
}
