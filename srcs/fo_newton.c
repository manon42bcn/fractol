/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fo_newton.c                                        +:+      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 19:47:42 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 19:47:42 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

/**
 * @brief Identify a root of z cubed minus one within the tolerance.
 * @param[in] point Current complex iterate.
 * @return Root index from 0 to 2, or -1 if no root is close enough.
 */
static int	ft_newton_root(t_coor point)
{
	static const long double	root[3][2] = {
	{1.0L, 0.0L},
	{-0.5L, 0.86602540378443864676L},
	{-0.5L, -0.86602540378443864676L}
	};
	long double					dx;
	long double					dy;
	int							i;

	i = 0;
	while (i < 3)
	{
		dx = point.a - root[i][0];
		dy = point.b - root[i][1];
		if (dx * dx + dy * dy < 1e-12L)
			return (i);
		i++;
	}
	return (-1);
}

/**
 * @brief Apply the Newton step (2*z + (1/z)^2) / 3.
 * @param[in] point Current complex iterate.
 * @param[in] norm Squared magnitude of the current iterate.
 * @return Next complex iterate.
 */
static t_coor	ft_newton_step(t_coor point, long double norm)
{
	t_coor	ratio;

	ratio.a = point.a / norm;
	ratio.b = point.b / norm;
	return ((t_coor){
		(2 * point.a + ratio.a * ratio.a - ratio.b * ratio.b) / 3,
		(2 * point.b - 2 * ratio.a * ratio.b) / 3});
}

/**
 * @brief Find the Newton basin for z cubed minus one.
 * @param[in] set Pixel coordinates and iteration limit.
 * @return Final point, iteration count, and one-based basin if found.
 */
t_orbit_result	ft_newton_test(const t_set *set)
{
	t_coor		point;
	long double	norm;
	int			root;
	int			n;

	point = (t_coor){set->zr, set->zi};
	n = 0;
	while (n <= set->max_i)
	{
		root = ft_newton_root(point);
		if (root >= 0)
			return ((t_orbit_result){.point = point, .n = n,
				.basin = root + 1});
		norm = point.a * point.a + point.b * point.b;
		if (n == set->max_i || norm == 0 || !isfinite(norm))
			break ;
		point = ft_newton_step(point, norm);
		n++;
	}
	return ((t_orbit_result){.point = point, .n = n});
}
