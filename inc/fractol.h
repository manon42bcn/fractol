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
# include <SDL3/SDL.h>
# include "libft.h"
# include "rgb.h"

# define MAX_ITERATIONS 1000
# define COLOR_SCHEME_COUNT 6
# define PALETTE_COLOR_COUNT 10
# define DEFAULT_PALETTE_SPAN 8
# define MIN_PALETTE_SPAN 2
# define MAX_PALETTE_SPAN 32
# define DEFAULT_VIEW_WIDTH 4.8f
# define DEFAULT_VIEW_HEIGHT 3.0f
# define RATIONAL_SMALL_OFFSET 0.001L
# define BUDDHABROT_SUBPIXEL_STEPS 256
# define BUDDHABROT_VISIT_WEIGHT 65536

typedef enum e_fractal_dir
{
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
	DROP7 = 12,
	TRICORN = 13,
	NEWTON,
	PHOENIX,
	BUDDHABROT,
	COS_C_OVER_Z,
	COS_Z_OVER_C,
	SIN_Z_OVER_SQRT_C,
	RATIONAL_QUADRATIC,
	QUADRATIC_LINEAR,
	RATIONAL_RECIPROCAL,
	INVERSE_SQUARE,
	INVERSE_FIFTH,
	RATIONAL_OFFSET_01,
	RATIONAL_OFFSET_0001,
	RATIONAL_SQUARE,
	MANDELBROT4,
	CUBIC_MINUS_LINEAR,
	QUADRATIC_C_PLUS_C_SQUARED,
	FRACTAL_COUNT
}	t_fractal_dir;

typedef struct s_data
{
	void	*addr;
	int		pitch;
}	t_data;

typedef struct s_viewer
{
	long double	zoom_f;
	long double	x_zero;
	long double	y_zero;
	long double	x_width;
	long double	y_height;
}	t_viewer;

typedef struct s_buddhabrot_pixel
{
	Uint64	visits[3];
}	t_buddhabrot_pixel;

typedef struct s_pixel_coord
{
	int	x;
	int	y;
}	t_pixel_coord;

typedef struct s_buddhabrot_tones
{
	float	scale[3];
	Uint32	gradients[3][256];
}	t_buddhabrot_tones;

typedef struct s_coor
{
	long double	a;
	long double	b;
}	t_coor;

/**
 * @brief Final iterate and termination data for a single orbit.
 * @details Newton basins are one-based. Zero means no basin was found.
 */
typedef struct s_orbit_result
{
	t_coor		point;
	int			n;
	bool		escaped;
	int			basin;
	long double	distance;
}	t_orbit_result;

typedef enum e_palette_interpolation
{
	PALETTE_SRGB,
	PALETTE_LINEAR,
	PALETTE_OKLAB,
	PALETTE_INTERPOLATION_COUNT
}	t_palette_interpolation;

typedef struct s_set
{
	int							fractal;
	long double					zoom;
	int							color_scheme;
	int							color_mode;
	t_palette_interpolation		palette_interpolation;
	int							palette_span;
	int							palette_anchor;
	bool						palette_single_color;
	bool						distance_shading;
	bool						tricorn_julia;
	t_viewer					*viewer;
	int							screen_w;
	int							screen_h;
	SDL_Window					*window;
	SDL_Renderer				*renderer;
	SDL_Texture					*texture;
	double						zi;
	double						zr;
	int							max_i;
	long double					esc;
	long double					c_r;
	long double					c_i;
	int							status;
	t_buddhabrot_pixel			*buddhabrot_counts;
	t_buddhabrot_pixel			buddhabrot_color_peaks;
	Uint64						buddhabrot_peak;
	bool						buddhabrot_recolor;
	bool						buddhabrot_original_exposure;
	unsigned int				buddhabrot_samples;
	unsigned int				buddhabrot_sample_limit;
	Uint64						buddhabrot_rng;
}	t_set;

typedef t_orbit_result	(*t_builders)(const t_set *set);

const char		*ft_fractal_titles(int index);
t_orbit_result	ft_mandelbrot_test(const t_set *set);
t_orbit_result	ft_mandelbrot_distance_test(const t_set *set);
t_orbit_result	ft_tricorn_test(const t_set *set);
t_orbit_result	ft_newton_test(const t_set *set);
t_orbit_result	ft_phoenix_test(const t_set *set);
t_orbit_result	ft_cos_c_over_z(const t_set *set);
t_orbit_result	ft_cos_z_over_c(const t_set *set);
t_orbit_result	ft_sin_z_over_sqrt_c(const t_set *set);
t_orbit_result	ft_rational_quadratic(const t_set *set);
t_orbit_result	ft_quadratic_linear(const t_set *set);
t_orbit_result	ft_rational_reciprocal(const t_set *set);
t_orbit_result	ft_inverse_square(const t_set *set);
t_orbit_result	ft_inverse_fifth(const t_set *set);
t_orbit_result	ft_rational_offset_01(const t_set *set);
t_orbit_result	ft_rational_offset_0001(const t_set *set);
t_orbit_result	ft_rational_square(const t_set *set);
t_orbit_result	ft_mandelbrot_test4(const t_set *set);
t_orbit_result	ft_cubic_minus_linear(const t_set *set);
t_orbit_result	ft_quadratic_c_plus_c_squared(const t_set *set);
bool			ft_buddhabrot_pending(const t_set *set);
void			ft_buddhabrot_render(t_set *set, t_data *img);
Uint64			ft_buddhabrot_total(const t_buddhabrot_pixel *pixel);
void			ft_buddhabrot_sample(t_set *set, t_coor *orbit);
void			ft_buddhabrot_draw(const t_set *set, t_data *img);
void			ft_nebulabrot_gradients(const t_set *set,
					Uint32 gradients[3][256]);
void			ft_nebulabrot_next_palette(t_set *set);
t_orbit_result	ft_julia_test(const t_set *set);
t_orbit_result	ft_burningship_test(const t_set *set);
t_orbit_result	ft_drop_test(const t_set *set);
t_orbit_result	ft_bird_test(const t_set *set);
t_orbit_result	ft_hiper_test(const t_set *set);
t_orbit_result	ft_spider_test(const t_set *set);
t_orbit_result	ft_worm_test(const t_set *set);
t_orbit_result	ft_mandelbrot_test3(const t_set *set);
t_orbit_result	ft_mandelbrot_test7(const t_set *set);
t_orbit_result	ft_julia_test3(const t_set *set);
t_orbit_result	ft_julia_test7(const t_set *set);
t_orbit_result	ft_drop_test7(const t_set *set);
void			ft_iterator(t_set *set, t_builders orbit, t_data *img);
int				ft_colorize_pixel(const t_set *set, t_orbit_result result);
double			ft_smooth_iterations(const t_set *set, t_coor point, int n);
int				ft_shade_distance(const t_set *set, int color,
					long double distance);
bool			ft_orbit_escaped(t_coor point, long double escape);
t_orbit_result	ft_orbit_pole(int n);
int				ft_scheme_color(const t_set *set, double iterations);
int				ft_load_base_color(t_set *set);
void			ft_keys_handler(SDL_Keycode key, t_set *set);
void			ft_keys_color_mode(SDL_Keycode key, t_set *set);
bool			ft_keys_special(SDL_Keycode key, t_set *set);
void			ft_handle_event(t_set *set, const SDL_Event *event);
void			ft_cycle_fractal(t_set *set, int direction);
t_set			*ft_init_fractol(int argc, char *argv[]);
void			ft_alloc_error(t_set *set, int error);
void			ft_input_messages(void);
void			ft_help(void);
int				create_trgb(int t, int r, int g, int b);
void			ft_put_pixel(t_data *data, int x, int y, int color);
void			ft_mouse_zoom(t_set *set, float scroll, int x, int y);
void			ft_mouse_select(t_set *set, int x, int y);
void			ft_free_all(t_set *set);
void			ft_init_video(t_set *set);
void			ft_init_texture(t_set *set);
void			ft_render(t_set *set);
void			ft_reset_viewer(t_set *set);
void			ft_viewer_defaults(t_viewer *viewer, const t_set *set);
t_set			*ft_init_set(int select, int color_mode);
t_viewer		*ft_init_viewer(t_set *set);

#endif
