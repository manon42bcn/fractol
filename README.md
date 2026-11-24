/* ************************************************ */
/*                                                  */
/*                              :::      ::::::::   */
/*                            :+:      :+:    :+:   */
/*                          +:+ +:+         +:+     */
/*                        +#+  +:+       +#+        */
/*                      +#+#+#+#+#+   +#+           */
/*                           #+#    #+#             */
/*                          ###   ########.fr       */
/* MPORRAS- 42Barcelona                             */
/* ************************************************ */
=================
FRACTOL.
=================

Fractol - 42 project.

Run ./fractol --help to further instructions.

Score 125%.

Don't copy without understand!
Be honest with yourself.

## Building with SDL3

Install the SDL3 development files (3.2.0 or newer), a C compiler,
Make and pkg-config. On macOS, SDL3 is available through Homebrew
with `make install-sdl`, which installs SDL3 and pkgconf and checks
the detected SDL3 version. On Linux, Windows and other systems, the rule
prints manual installation instructions for the detected platform.

Run `make check-sdl` to verify that pkg-config can find SDL3 and print
the installed version without building Fractol.

```sh
make
./fractol mandelbrot
```

The window and fractal texture use the same logical dimensions.

## Additional fractals

- `./fractol tricorn color` opens the Tricorn. Right-click to see the
  anti-Julia set for the complex coordinate under the cursor. Right-click
  again to choose another constant, or press **Q** to return to the Tricorn.

- `./fractol newton` shows the three basins of attraction for the roots of
  z³ − 1. Hue identifies the root and brightness shows how quickly the
  point converged. Points without convergence are black.
- `./fractol phoenix color` renders the Julia-style Phoenix with
  c = 0.5667 and p = −0.5. Right-click to use the cursor's complex
  coordinates as the new value of c.
- `./fractol buddhabrot` progressively plots the density of escaping
  Mandelbrot orbits in grayscale. Start with `./fractol buddhabrot color`
  for Nebulabrot, or press **C** for color and **B** for grayscale.
  Press **Space** in color mode to cycle through four palettes without
  restarting sampling. Each palette has a gradient for each orbit range.
  In the default RGB palette, blue represents escape in 1–8 steps,
  green in 9–24 steps, and red in 25 or more. Each range uses its own
  logarithmic scale. A brightness curve dims sparse visits by default.
  Press **S** to compare with the original logarithmic exposure without
  restarting sampling. Changing the view or the iteration and
  escape settings restarts sampling. The initial view uses 1,048,576
  samples from the same region around the Mandelbrot set. The sample
  budget increases in proportion to the inverse of the visible area,
  up to 16,777,216 samples per view. A bilinear filter spreads each
  visit across neighboring pixel centers before applying the colors.

### Experimental palette interpolation

Press **L** in `color` mode to cycle through sRGB (the default),
linear-light, and [Oklab](https://bottosson.github.io/posts/oklab/)
interpolation. This applies to the classic palettes and to the
Nebulabrot gradients. In Nebulabrot you can switch while
keeping the accumulated samples.

The RGB utilities in `rgb/` come from
[manon42bcn/rgb](https://github.com/manon42bcn/rgb) at commit
`fa9ade0a3edde5f5e5ee8737deaa3047272419c9`. Fractol adds
`ft_rgb_lerp_linear` and `ft_rgb_lerp_oklab` to that copy.

For classic fractals in `color` mode, **J** shortens palette transitions
and **K** lengthens them. The default is eight iterations between
reference colors. **P** switches between the full palette and a
black-to-white gradient through one reference color. In that mode,
**N** picks another color from the current palette. The window title
shows the palette, interpolation mode, and transition length.
