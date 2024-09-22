/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/23 13:03:57 by mporras-          #+#    #+#             */
/*   Updated: 2024/09/22 21:49:32 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

# include <unistd.h>
# include <stdlib.h>
# include <math.h>
# include "mlx.h"
# include "libft.h"

# define K_SPACE		49
# define K_ESC			53
# define K_Q			12
# define K_C			8
# define K_M 			46
# define K_B			11
# define K_F 			3
# define K_PLUS			24
# define K_MINUS		27
# define K_PAD_PLUS		69
# define K_PAD_MINUS	78
# define K_UP			126
# define K_DOWN			125
# define K_LEFT			123
# define K_RIGHT		124
# define K_I 			34
# define K_O 			31
# define NO_FRACTAL     -1

enum e_fractal_dir {
    MALDELBROT = 0,
    JULIA = 1,
    BURNINGSHIP = 2,
    DROP = 3,
    BIRD = 4,
    SPIDER = 5,
    WORM = 6,
    HIPER = 7,
    MANDELBROT3 = 8,
    MANDELBROT7 = 9,
    JULIA3 = 10,
    JULIA7 = 11,
    DROP7 = 12
};

typedef struct s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct s_viewer
{
	long double	zoom_f;
	long double	x_zero;
	long double	y_zero;
	long double	x_width;
	long double	y_height;
}	t_viewer;

typedef struct s_set
{
	int				fractal;
	long double		zoom;
	int				color_scheme;
	int				color_mode;
	t_viewer		*viewer;
	int				screen_w;
	int				screen_h;
	void			*mlx;
	void			*mlx_win;
	t_data			*img;
	double			zi;
	double			zr;
	int				max_i;
	long double		esc;
	long double		c_r;
	long double		c_i;
	int				color;
	int				status;
}	t_set;

typedef int (*t_builders)(t_set *set);

int         ft_fractal_dir(char *s);
char        *ft_fractal_titles(int index);
int			ft_mandelbrot_test(t_set *set);
int			ft_julia_test(t_set *set);
int			ft_burningship_test(t_set *set);
int			ft_drop_test(t_set *set);
int			ft_bird_test(t_set *set);
int			ft_hiper_test(t_set *set);
int			ft_spider_test(t_set *set);
int			ft_worm_test(t_set *set);
int	        ft_mandelbrot_test3(t_set *set);
int	        ft_mandelbrot_test7(t_set *set);
int	        ft_julia_test3(t_set *set);
int	        ft_julia_test7(t_set *set);
int	        ft_drop_test7(t_set *set);
void		ft_iterator(t_set *set, int (f)(t_set *));
int			ft_colorize_pixel(t_set *set, long double a, long double b, int n);
int			ft_load_base_color(t_set *set);
int			ft_keys_handler(int key, t_set *set);
int			ft_window_handler(t_set *set);
t_set		*ft_init_fractol(int argc, char *argv[]);
void		ft_alloc_error(t_set *set, int error);
void		ft_input_messages(int select);
void		ft_help(void);
int			create_trgb(int t, int r, int g, int b);
void		my_mlx_pixel_put(t_data *data, int x, int y, int color);
int			ft_mouse_handler(int mouse_code, int x, int y, t_set *set);
long double	ft_strfloat(char *str);
int			ft_free_all(t_set *set);
void		ft_reset_viewer(t_set *set);
t_set		*ft_init_set(int select, int color_mode);
t_viewer	*ft_init_viewer(t_set *set);
int			ft_render(t_set *set);

#endif
