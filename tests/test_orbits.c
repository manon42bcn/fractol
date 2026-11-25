/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_orbits.c                                      +:+      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mporras <mporras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/25 19:47:42 by mporras-          #+#    #+#             */
/*   Updated: 2026/11/25 19:47:42 by mporras-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"
#include <assert.h>
#include <stdio.h>

/**
 * @brief Check polynomial orbits against known complex points.
 */
static void	test_polynomials(void)
{
	t_set			set;
	t_orbit_result	result;

	set = (t_set){.zr = 0.5, .zi = 0.5, .esc = 4, .max_i = 2};
	result = ft_mandelbrot_test4(&set);
	assert(result.point.a == 0.25L && result.point.b == 0.5L);
	assert(!result.escaped && result.n == 2);
	result = ft_cubic_minus_linear(&set);
	assert(result.point.a == -0.25L && result.point.b == 0.25L);
	assert(!result.escaped && result.n == 2);
	result = ft_quadratic_c_plus_c_squared(&set);
	assert(result.point.a == -0.25L && result.point.b == 2);
	assert(result.escaped && result.n == 2);
	set.zr = 0;
	set.zi = 0;
	result = ft_mandelbrot_test7(&set);
	assert(result.point.a == 0 && result.point.b == 0);
	assert(!result.escaped && result.n == 2);
	set.zr = 1;
	set.zi = 1;
	result = ft_mandelbrot_test7(&set);
	assert(result.point.a == 9 && result.point.b == -7);
	assert(result.escaped && result.n == 2);
}

/**
 * @brief Check the two denominator offsets against their first orbit steps.
 */
static void	test_offsets(void)
{
	t_set			set;
	t_orbit_result	result;

	set = (t_set){.zi = 0.05, .esc = 4, .max_i = 1};
	result = ft_rational_offset_01(&set);
	assert(result.point.a == 0 && fabsl(result.point.b - 0.5L) < 1e-12L);
	assert(!result.escaped);
	result = ft_rational_offset_0001(&set);
	assert(result.point.a == 0 && fabsl(result.point.b - 50) < 1e-12L);
	assert(result.escaped && result.n == 1);
}

/**
 * @brief Check singularities at startup and after an orbit step.
 */
static void	test_poles(void)
{
	t_set			set;
	t_orbit_result	result;

	set = (t_set){.zr = 2, .esc = 4, .max_i = 2};
	result = ft_rational_square(&set);
	assert(result.escaped && result.n == 1);
	assert(isinf(result.point.a) && isinf(result.point.b));
	set.zr = -0.25;
	result = ft_rational_quadratic(&set);
	assert(result.escaped && result.n == 2);
	assert(isinf(result.point.a) && isinf(result.point.b));
	set.zr = -1;
	result = ft_inverse_square(&set);
	assert(result.escaped && result.n == 2);
	result = ft_inverse_fifth(&set);
	assert(result.escaped && result.n == 2);
}

/**
 * @brief Check the Newton basin at a root and at the singular origin.
 */
static void	test_newton(void)
{
	t_set			set;
	t_orbit_result	result;

	set = (t_set){.zr = 1, .max_i = 2};
	result = ft_newton_test(&set);
	assert(result.point.a == 1 && result.point.b == 0);
	assert(result.basin == 1 && result.n == 0 && !result.escaped);
	set.zr = 0;
	result = ft_newton_test(&set);
	assert(result.point.a == 0 && result.point.b == 0);
	assert(result.basin == 0 && result.n == 0 && !result.escaped);
}

/**
 * @brief Run the numerical regression tests.
 * @return Zero if all assertions pass.
 */
int	main(void)
{
	test_polynomials();
	test_offsets();
	test_poles();
	test_newton();
	puts("Orbit tests passed.");
	return (0);
}
