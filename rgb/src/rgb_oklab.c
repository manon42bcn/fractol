/**
 * @file rgb_oklab.c
 * @brief Perceptual palette interpolation in Oklab.
 * @see https://bottosson.github.io/posts/oklab/
 */

#include "rgb.h"
#include "rgb_internal.h"
#include <math.h>

/** @brief Oklab lightness and opponent color coordinates. */
typedef struct s_oklab
{
	double	lightness;
	double	a;
	double	b;
}	t_oklab;

/** @brief Decode packed sRGB and apply Björn Ottosson's Oklab transform. */
static t_oklab	ft_rgb_to_oklab(t_rgb color)
{
	double	r;
	double	g;
	double	b;
	double	l;
	double	m;
	double	s;

	r = ft_rgb_decode((color >> 16) & RGB_MASK);
	g = ft_rgb_decode((color >> 8) & RGB_MASK);
	b = ft_rgb_decode(color & RGB_MASK);
	l = cbrt(0.4122214708 * r + 0.5363325363 * g + 0.0514459929 * b);
	m = cbrt(0.2119034982 * r + 0.6806995451 * g + 0.1073969566 * b);
	s = cbrt(0.0883024619 * r + 0.2817188376 * g + 0.6299787005 * b);
	return ((t_oklab){
		0.2104542553 * l + 0.7936177850 * m - 0.0040720468 * s,
		1.9779984951 * l - 2.4285922050 * m + 0.4505937099 * s,
		0.0259040371 * l + 0.7827717662 * m - 0.8086757660 * s
	});
}

/** @brief Convert Oklab to packed sRGB, clipping and rounding at output. */
static t_rgb	ft_oklab_to_rgb(t_oklab color)
{
	double	l;
	double	m;
	double	s;
	t_rgb	channels[3];

	l = color.lightness + 0.3963377774 * color.a + 0.2158037573 * color.b;
	m = color.lightness - 0.1055613458 * color.a - 0.0638541728 * color.b;
	s = color.lightness - 0.0894841775 * color.a - 1.2914855480 * color.b;
	l = l * l * l;
	m = m * m * m;
	s = s * s * s;
	channels[R_I] = ft_rgb_encode(4.0767416621 * l - 3.3077115913 * m
			+ 0.2309699292 * s);
	channels[G_I] = ft_rgb_encode(-1.2684380046 * l + 2.6097574011 * m
			- 0.3413193965 * s);
	channels[B_I] = ft_rgb_encode(-0.0041960863 * l - 0.7034186147 * m
			+ 1.7076147010 * s);
	return (ft_rgb_get(channels));
}

/** @copydoc ft_rgb_lerp_oklab */
t_rgb	ft_rgb_lerp_oklab(t_rgb start, t_rgb end, double factor)
{
	t_oklab	from;
	t_oklab	to;

	if (!(factor > 0))
		return (start & 0xffffff);
	if (factor >= 1)
		return (end & 0xffffff);
	from = ft_rgb_to_oklab(start);
	to = ft_rgb_to_oklab(end);
	from.lightness += (to.lightness - from.lightness) * factor;
	from.a += (to.a - from.a) * factor;
	from.b += (to.b - from.b) * factor;
	return (ft_oklab_to_rgb(from));
}
