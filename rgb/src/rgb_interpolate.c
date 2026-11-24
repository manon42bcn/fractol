#include "rgb.h"
#include "rgb_internal.h"
#include <math.h>

double	ft_rgb_decode(int channel)
{
	double	value;

	value = channel / 255.0;
	if (value <= 0.04045)
		return (value / 12.92);
	return (pow((value + 0.055) / 1.055, 2.4));
}

int	ft_rgb_encode(double value)
{
	value = fmin(1, fmax(0, value));
	if (value <= 0.0031308)
		value *= 12.92;
	else
		value = 1.055 * pow(value, 1.0 / 2.4) - 0.055;
	return ((int)(255 * value + 0.5));
}

/* Interpolate in linear light and round once when encoding back to sRGB. */
t_rgb	ft_rgb_lerp_linear(t_rgb start, t_rgb end, double factor)
{
	t_rgb	channels[3];
	double	first;
	double	second;
	int		i;
	int		shift;

	if (!(factor > 0))
		return (start & 0xffffff);
	if (factor >= 1)
		return (end & 0xffffff);
	i = 0;
	while (i < 3)
	{
		shift = (2 - i) * 8;
		first = ft_rgb_decode((start >> shift) & RGB_MASK);
		second = ft_rgb_decode((end >> shift) & RGB_MASK);
		channels[i] = ft_rgb_encode(first + (second - first) * factor);
		i++;
	}
	return (ft_rgb_get(channels));
}
