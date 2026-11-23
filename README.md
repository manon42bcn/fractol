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
