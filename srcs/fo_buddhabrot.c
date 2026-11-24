#include "fractol.h"

#define BUDDHABROT_BATCH 4096
#define BUDDHABROT_BASE_SAMPLES 1048576u
#define BUDDHABROT_MAX_SAMPLE_MULTIPLIER 16
#define BUDDHABROT_SUBPIXEL_STEPS 256
#define BUDDHABROT_VISIT_WEIGHT (BUDDHABROT_SUBPIXEL_STEPS * BUDDHABROT_SUBPIXEL_STEPS)

typedef struct s_orbit_point
{
	double	a;
	double	b;
}	t_orbit_point;

/* Each row has midpoint and endpoint colors for long, medium and short orbits. */
static const Uint32 g_nebulabrot_palettes[][3][2] = {
	{{0x800000, 0xff0000}, {0x008000, 0x00ff00}, {0x000080, 0x0000ff}},
	{{0x982e12, 0xffba65}, {0x087063, 0x87ffd4}, {0x121d74, 0x598cfa}},
	{{0x6d154c, 0xff9ac2}, {0x4a2493, 0xd2a6ff}, {0x124d6b, 0x77e9ff}},
	{{0xa34206, 0xffdf9c}, {0x0a4b8c, 0x88d9ff}, {0x241449, 0x9672dc}},
};

void	ft_nebulabrot_next_palette(t_set *set)
{
	set->color_scheme = (set->color_scheme + 1)
		% (sizeof(g_nebulabrot_palettes) / sizeof(g_nebulabrot_palettes[0]));
	set->buddhabrot_recolor = true;
}

bool	ft_buddhabrot_pending(const t_set *set)
{
	return (set->fractal == BUDDHABROT
		&& set->buddhabrot_samples < set->buddhabrot_sample_limit);
}

static unsigned int	ft_buddhabrot_sample_budget(const t_viewer *viewer)
{
	long double	area_ratio;

	area_ratio = (DEFAULT_VIEW_WIDTH / viewer->x_width)
		* (DEFAULT_VIEW_HEIGHT / viewer->y_height);
	area_ratio = fminl(fmaxl(area_ratio, 1), BUDDHABROT_MAX_SAMPLE_MULTIPLIER);
	return ((unsigned int)ceill(BUDDHABROT_BASE_SAMPLES * area_ratio));
}

static void	ft_buddhabrot_reset(t_set *set)
{
	size_t	pixels;

	pixels = (size_t)set->screen_w * set->screen_h;
	if (set->buddhabrot_counts == NULL)
		set->buddhabrot_counts = calloc(pixels, sizeof(*set->buddhabrot_counts));
	else
		SDL_memset(set->buddhabrot_counts, 0, pixels * sizeof(*set->buddhabrot_counts));
	if (set->buddhabrot_counts == NULL)
		ft_alloc_error(set, 4);
	set->buddhabrot_samples = 0;
	set->buddhabrot_sample_limit = ft_buddhabrot_sample_budget(set->viewer);
	set->buddhabrot_peak = 0;
	set->buddhabrot_color_peaks = (t_buddhabrot_pixel){0};
	set->buddhabrot_rng = 1;
}

static Uint64	ft_buddhabrot_total(const t_buddhabrot_pixel *pixel)
{
	return (pixel->visits[0] + pixel->visits[1] + pixel->visits[2]);
}

static int	ft_buddhabrot_band(int escape_step)
{
	if (escape_step <= 8)
		return (2);
	if (escape_step <= 24)
		return (1);
	return (0);
}

static void	ft_buddhabrot_add(t_set *set, int x, int y, int band, Uint64 weight)
{
	t_buddhabrot_pixel	*pixel;
	Uint64				count;
	Uint64				total;

	if (weight == 0 || x < 0 || x >= set->screen_w || y < 0 || y >= set->screen_h)
		return;
	pixel = &set->buddhabrot_counts[(size_t)y * set->screen_w + x];
	pixel->visits[band] += weight;
	count = pixel->visits[band];
	if (count > set->buddhabrot_color_peaks.visits[band])
		set->buddhabrot_color_peaks.visits[band] = count;
	total = ft_buddhabrot_total(pixel);
	if (total > set->buddhabrot_peak)
		set->buddhabrot_peak = total;
}

static void	ft_buddhabrot_splat(t_set *set, long double x, long double y, int band)
{
	int		left;
	int		top;
	Uint64	right_weight;
	Uint64	bottom_weight;
	Uint64	left_weight;
	Uint64	top_weight;

	/* Move the pixel centers to integer coordinates before weighting them. */
	x -= 0.5L;
	y -= 0.5L;
	if (!(x > -1 && x < set->screen_w && y > -1 && y < set->screen_h))
		return;
	left = (int)floorl(x);
	top = (int)floorl(y);
	right_weight = (Uint64)((x - left) * BUDDHABROT_SUBPIXEL_STEPS + 0.5L);
	bottom_weight = (Uint64)((y - top) * BUDDHABROT_SUBPIXEL_STEPS + 0.5L);
	left_weight = BUDDHABROT_SUBPIXEL_STEPS - right_weight;
	top_weight = BUDDHABROT_SUBPIXEL_STEPS - bottom_weight;
	/* Contributions outside the texture are clipped at its edges. */
	ft_buddhabrot_add(set, left, top, band, left_weight * top_weight);
	ft_buddhabrot_add(set, left + 1, top, band, right_weight * top_weight);
	ft_buddhabrot_add(set, left, top + 1, band, left_weight * bottom_weight);
	ft_buddhabrot_add(set, left + 1, top + 1, band, right_weight * bottom_weight);
}

static void	ft_buddhabrot_record(t_set *set, const t_orbit_point *orbit, int n)
{
	long double	x_scale;
	long double	y_scale;
	int			band;
	int			i;

	x_scale = set->screen_w / set->viewer->x_width;
	y_scale = set->screen_h / set->viewer->y_height;
	band = ft_buddhabrot_band(n + 1);
	i = 0;
	while (i < n)
	{
		ft_buddhabrot_splat(set,
			orbit[i].a * x_scale + set->viewer->x_zero,
			orbit[i].b * y_scale + set->viewer->y_zero, band);
		i++;
	}
}

static void	ft_buddhabrot_sample(t_set *set, t_orbit_point *orbit)
{
	double	c_r;
	double	c_i;
	double	a;
	double	b;
	double	next_a;
	int		n;

	/* Sample around Mandelbrot regardless of where the viewer is pointed. */
	c_r = 3.4 * SDL_randf_r(&set->buddhabrot_rng) - 2.2;
	c_i = 3.0 * SDL_randf_r(&set->buddhabrot_rng) - 1.5;
	a = 0;
	b = 0;
	n = 0;
	while (n < set->max_i)
	{
		next_a = a * a - b * b + c_r;
		b = 2 * a * b + c_i;
		a = next_a;
		if (a * a + b * b > set->esc)
		{
			ft_buddhabrot_record(set, orbit, n);
			return;
		}
		orbit[n].a = a;
		orbit[n].b = b;
		n++;
	}
}

static float	ft_density_scale(Uint64 peak)
{
	if (peak == 0)
		return (0);
	return (255.0f / log1pf((float)peak / BUDDHABROT_VISIT_WEIGHT));
}

static int	ft_density_level(Uint64 count, float scale, bool original_exposure)
{
	float	level;

	if (count == 0)
		return (0);
	level = fminf(255, log1pf((float)count / BUDDHABROT_VISIT_WEIGHT) * scale);
	if (!original_exposure)
		level *= sqrtf(level / 255.0f);
	return ((int)level);
}

static Uint32	ft_gradient_rgb(Uint32 start, Uint32 end, int step, int span)
{
	Uint32	color;
	int		shift;
	int		channel;

	color = 0;
	shift = 0;
	while (shift <= 16)
	{
		channel = (((start >> shift) & 255) * (span - step)
				+ ((end >> shift) & 255) * step + span / 2) / span;
		color |= (Uint32)channel << shift;
		shift += 8;
	}
	return (color);
}

static void	ft_nebulabrot_gradients(int scheme, Uint32 gradients[3][256])
{
	const Uint32	(*stops)[2];
	int				band;
	int				level;

	stops = g_nebulabrot_palettes[scheme];
	band = 0;
	while (band < 3)
	{
		level = 0;
		while (level <= 128)
		{
			gradients[band][level] = ft_gradient_rgb(0, stops[band][0], level, 128);
			level++;
		}
		while (level <= 255)
		{
			gradients[band][level] = ft_gradient_rgb(stops[band][0],
					stops[band][1], level - 128, 127);
			level++;
		}
		band++;
	}
}

static int	ft_nebulabrot_mix(Uint32 first, Uint32 second, Uint32 third)
{
	Uint32	color;
	Uint32	remainder;
	int		shift;

	color = 0;
	shift = 0;
	while (shift <= 16)
	{
		/* Combine light from the bands without clipping additive sums. */
		remainder = (255 - ((first >> shift) & 255))
			* (255 - ((second >> shift) & 255))
			* (255 - ((third >> shift) & 255));
		color |= (255 - (remainder + 32512) / 65025) << shift;
		shift += 8;
	}
	return ((int)color);
}

static void	ft_buddhabrot_draw(const t_set *set, t_data *img)
{
	const t_buddhabrot_pixel	*pixel;
	float						scale[3];
	Uint32						gradients[3][256];
	int							color;
	int							level;
	int							x;
	int							y;

	if (set->color_mode == 1)
	{
		ft_nebulabrot_gradients(set->color_scheme, gradients);
		scale[0] = ft_density_scale(set->buddhabrot_color_peaks.visits[0]);
		scale[1] = ft_density_scale(set->buddhabrot_color_peaks.visits[1]);
		scale[2] = ft_density_scale(set->buddhabrot_color_peaks.visits[2]);
	}
	else
		scale[0] = ft_density_scale(set->buddhabrot_peak);
	y = 0;
	while (y < set->screen_h)
	{
		x = 0;
		while (x < set->screen_w)
		{
			pixel = &set->buddhabrot_counts[(size_t)y * set->screen_w + x];
			if (set->color_mode == 1)
				color = ft_nebulabrot_mix(
						gradients[0][ft_density_level(pixel->visits[0], scale[0],
						set->buddhabrot_original_exposure)],
						gradients[1][ft_density_level(pixel->visits[1], scale[1],
						set->buddhabrot_original_exposure)],
						gradients[2][ft_density_level(pixel->visits[2], scale[2],
						set->buddhabrot_original_exposure)]);
			else
			{
				level = ft_density_level(ft_buddhabrot_total(pixel), scale[0],
						set->buddhabrot_original_exposure);
				color = create_trgb(0, level, level, level);
			}
			ft_put_pixel(img, x, y, color);
			x++;
		}
		y++;
	}
}

void	ft_buddhabrot_render(t_set *set, t_data *img)
{
	t_orbit_point	orbit[MAX_ITERATIONS];
	unsigned int	end;

	if (set->status)
		ft_buddhabrot_reset(set);
	end = set->buddhabrot_samples + BUDDHABROT_BATCH;
	while (set->buddhabrot_samples < end && ft_buddhabrot_pending(set))
	{
		ft_buddhabrot_sample(set, orbit);
		set->buddhabrot_samples++;
	}
	ft_buddhabrot_draw(set, img);
	set->buddhabrot_recolor = false;
}
