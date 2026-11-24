#include "fractol.h"

static int	ft_newton_root(long double a, long double b)
{
	static const long double	root[3][2] = {
		{1.0L, 0.0L},
		{-0.5L, 0.86602540378443864676L},
		{-0.5L, -0.86602540378443864676L}
	};
	long double	dx;
	long double	dy;
	int			i;

	i = 0;
	while (i < 3)
	{
		dx = a - root[i][0];
		dy = b - root[i][1];
		if (dx * dx + dy * dy < 1e-12L)
			return (i);
		i++;
	}
	return (-1);
}

static int	ft_newton_color(int root, int iterations)
{
	int	light;

	light = (int)(255 / (1 + 0.08 * iterations));
	if (root == 0)
		return (create_trgb(0, light, light / 4, light / 3));
	if (root == 1)
		return (create_trgb(0, light / 5, light, light / 2));
	return (create_trgb(0, light / 4, light / 2, light));
}

int	ft_newton_test(t_set *set)
{
	long double	a;
	long double	b;
	long double	norm;
	long double	next_a;
	int			root;
	int			n;

	a = set->zr;
	b = set->zi;
	n = 0;
	set->color = 0;
	while (n <= set->max_i)
	{
		root = ft_newton_root(a, b);
		if (root >= 0)
		{
			set->color = ft_newton_color(root, n);
			break;
		}
		norm = a * a + b * b;
		if (n == set->max_i || norm == 0 || !isfinite(norm))
			break;
		/* Newton step for z^3 - 1: (2*z + (1/z)^2) / 3. */
		next_a = (2 * a + (a / norm) * (a / norm) - (b / norm) * (b / norm)) / 3;
		b = (2 * b - 2 * (a / norm) * (b / norm)) / 3;
		a = next_a;
		n++;
	}
	return (1);
}
