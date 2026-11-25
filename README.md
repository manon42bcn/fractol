# Fractol

Fractol is an interactive fractal explorer written in C for the 42 Barcelona
project. It draws escape-time fractals, Newton basins, and orbit-density images
with SDL3. The original project received a score of 125%.

## Installation

You need Git, a C compiler, Make, pkg-config (or pkgconf), and the
**SDL3 development files, version 3.2.0 or later**.

1. Clone the project and initialize its dependencies.

   ```sh
   git clone git@github.com:manon42bcn/fractol.git
   cd fractol
   make submodules
   ```

   The build uses the [libft] and [rgb] submodules in `lib/` and `rgb/`.
   Their configured URLs use GitHub SSH. To fetch the public submodules over
   HTTPS, run the following command in place of `make submodules`:

   ```sh
   git -c url.https://github.com/.insteadOf=git@github.com: submodule update --init --recursive
   ```

2. Install SDL3 for your platform.

   - **macOS.** With Homebrew installed, run `make install-sdl` to install
     SDL3 and pkgconf.
   - **Linux.** Install your distribution's SDL3 development package and
     pkg-config. The [SDL3 Linux instructions] cover building SDL3 from source.
   - **Windows.** In an MSYS2 UCRT64 shell with a C toolchain and Make,
     install `mingw-w64-ucrt-x86_64-sdl3` and `mingw-w64-ucrt-x86_64-pkgconf`.
     See the [SDL3 MinGW instructions].

3. Check the dependency, build, and start the explorer.

   ```sh
   make check-sdl
   make
   ./fractol
   ```

The [Makefile](Makefile) uses pkg-config to find SDL3. It builds and links
the static libraries `libft.a` and `librgb.a` from the submodules.
The application's object files go into `objs/`, which Git ignores.
The submodules build their objects in `lib/obj/` and `rgb/obj/`.
Use `make clean` to remove the compiled objects.

## Controls

Fractol opens in Mandelbrot. Use **Tab** and **Shift+Tab** to move through the
fractal list in either direction. The list wraps and each fractal opens at
its default view. Start with `./fractol color` for color, or pass `mono`,
`bw`, or `full`. The default is `bw`. Run `./fractol --help` for the controls.

| Input | Action |
| --- | --- |
| Mouse wheel, arrow keys, **Q** | Zoom at the cursor, pan, or reset the view. |
| **+** / **−**, **I** / **O** | Raise or lower the iteration limit or squared escape threshold. |
| **C**, **M**, **B**, **F** | Select color, tinted monochrome, black and white, or full mode where supported. |
| **Space**, **L** | Cycle palettes or change interpolation in color mode. |
| **D** | Toggle distance shading in Mandelbrot color mode. |
| **J** / **K**, **P**, **N** | Shorten or lengthen classic palette transitions, toggle the single-color gradient, or choose another reference color. |
| Right click | Pick a complex constant in Julia or Phoenix, or enter anti-Julia from Tricorn. Press **Q** to return to Tricorn. |
| **Esc** | Close the explorer. |

## How a fractal is calculated

An orbit is the sequence of complex values obtained by applying a formula
repeatedly. Nearby inputs can produce very different orbits. In a
parameter-plane fractal such as Mandelbrot, each pixel supplies the
parameter $c$. In a Julia-style fractal, the pixel supplies $z_0$ and a
shared constant determines the image.

Given the view origin $(x_0,y_0)$, complex extent $(w,h)$, and texture size
$(W,H)$, [the pixel iterator](srcs/fo_iterator.c) maps a pixel $(x,y)$ to

$$
p = (x - x_0)\frac{w}{W} + i(y - y_0)\frac{h}{H}.
$$

The imaginary component increases down the screen. Zoom and pan change the
view extent and origin. The initial iteration limit is 50. Most
escape-time variants check the squared magnitude $|z|^2$ against an
initial threshold of 4. Pixels whose orbits reach the iteration limit
without meeting the escape condition are black. For the newer rational
and trigonometric maps, this cutoff is a rendering rule, and division by
zero and nonfinite iterates also count as escapes.

Each per-pixel orbit function returns a `t_orbit_result` containing its final
point, iteration count, and escape or convergence information.
[The colorizer](srcs/fo_colorize.c) turns that result into a packed pixel
color. Buddhabrot draws from a histogram of many orbits.

### Escape-time formulas

Mandelbrot uses $z_{n+1}=z_n^2+c$ with $z_0=0$. The quadratic and cubic
Mandelbrot loops start with the first iterate $z_1=c$ already computed.
For $z=a+bi$, the quadratic step uses the old values of both components:

$$
a' = a^2-b^2+\operatorname{Re}(c),\qquad
b' = 2ab+\operatorname{Im}(c).
$$

Fractol uses explicit component arithmetic and C complex arithmetic.
For example, the seventh-power Mandelbrot step evaluates the real and
imaginary polynomials using $a^2$ and $b^2$. The fourth-power variant
squares twice. In $z^2+c+c^2$, the term $c+c^2$ is computed once per pixel.
[Seventh-power maps](srcs/fo_fractal_degree7.c) · [Polynomial variants](srcs/fo_fractal_polynomial.c)

| Family | Recurrence and initial point |
| --- | --- |
| Mandelbrot powers | $z^m+c$ for $m=2,3,4,7$, from zero. |
| Julia powers | $z^m+c$ for $m=2,3,7$, with the pixel as $z_0$. These variants add the selected constant's real and imaginary components in swapped order. |
| Burning Ship | $(\lvert a\rvert+i\lvert b\rvert)^2+c$ for $z=a+bi$, with the loop initialized at $z=c$. |
| Tricorn and anti-Julia | $\overline{z}^{\,2}+c$. Tricorn uses the pixel as $c$ and starts at zero. Anti-Julia uses the pixel as $z_0$ and the selected constant as $c$. |
| Phoenix | $z_{n+1}=z_n^2+c-0.5z_{n-1}$, with the pixel as $z_0$, $z_{-1}=0$, and default $c=0.5667$. Its escape check accounts for the previous point. |
| Polynomial variants | $z^2+z+c$, $z^3-z+c$, and $z^2+c+c^2$, all from zero. |
| Rational quadratic | $z^2+c^2/(z^2+c)+c$, from zero. |
| Rational reciprocal | $(1+z)/z^2+c$, starting at the finite critical point $z_0=-2$. |
| Negative powers | $z^{-2}+c$ and $z^{-5}+c$, both from $z_0=1$. |
| Rational offsets | $z^2+c/(z^2+k)$, with $k=0.1$ or $k=0.001$ and $z_0=0$. |
| Rational square | $\left((z^2+c-1)/(2z+c-2)\right)^2$, from zero. |
| Trigonometric ratios | $\cos(c/z)$, $\cos(z/c)$, and $\sin(z/\sqrt{c})$, all from $z_0=1$. The sine map uses the principal complex square root. |

Drop and Drop7 apply quadratic and seventh-power iteration to an inverted
pixel parameter. Bird, Spider, Worm, and Hiper have their own coordinate
transforms and escape checks.
[Shape variants](srcs/fo_fractal_shapes.c) · [Experimental maps](srcs/fo_iterator.c)

The $k=0.001$ offset variant opens zoomed in around $c=0$ because its first
step gives $z_1=1000c$. Press **Q** to return to that framing.

### Newton and Buddhabrot

**Newton** starts at the pixel coordinate and applies Newton's method to
$f(z)=z^3-1$. Its step is $z_{n+1}=(2z_n+1/z_n^2)/3$.
The converged root determines hue, and faster convergence gives a brighter
pixel. Points without a converged root are black. [Newton iteration](srcs/fo_newton.c)

**Buddhabrot** samples $c$ in a fixed region around Mandelbrot and iterates
$z^2+c$ from zero. When an orbit escapes, the renderer projects its recorded
points into a histogram and distributes visits among neighboring pixels
using bilinear weights. It starts with a budget of 1,048,576 samples.
Zooming in increases the budget with the inverse of the visible area, up
to 16,777,216 samples per view. Changes to the view, iteration limit, or
escape threshold restart sampling. [Sampler](srcs/fo_buddhabrot.c)

In color mode, **Nebulabrot** groups escaping orbits by their escape step.
The default palette uses blue for 1–8 steps, green for 9–24, and red for 25
or more. Each band has its own logarithmic intensity scale. **Space** cycles
four palettes and **S** toggles the brightness curve that dims sparse visits.
Both controls preserve the accumulated histogram.

### Color and palettes

Classic palettes convert escape counts to positions on a circular gradient.
The default is eight iterations between reference colors. Mandelbrot, Julia,
Tricorn, Burning Ship, and the supported higher-degree polynomial maps
use fractional escape counts for smoother transitions. The smoothing
uses the degree of the recurrence where applicable.

Palettes 2, 3, and 6 close their cycles with a gradual return to the
starting tone. Palette 2 goes from dark blue through turquoise to ivory,
palette 3 from graphite to silver, and palette 6 from dark copper through
amber to cream.

**L** cycles sRGB (the default), linear light, and [Oklab] interpolation.
The window title shows the active palette, interpolation, and transition
length when applicable. In Nebulabrot, changing interpolation recolors
the current histogram.

In Mandelbrot color mode, **D** toggles an optional exterior distance
estimate based on the derivative of the orbit. The renderer uses it to
darken colors near the boundary over a few fractions of a pixel. The
window title indicates when distance shading is active.

## SDL3 rendering

[Video initialization](srcs/fo_video_init.c) creates the SDL window, renderer,
and streaming ARGB8888 texture. Fractol calculates the pixels on the CPU.
[The render loop](srcs/fo_video.c) locks the texture and writes each row using
SDL's reported pitch. It then unlocks the texture, draws it with
`SDL_RenderTexture`, and presents the frame. SDL3 also supplies keyboard,
mouse, window, and renderer events. The loop reuses the current texture
until a redraw is needed. Buddhabrot keeps requesting frames while
sampling is in progress. [SDL streaming texture API]

The window and texture use the same logical dimensions. Both are sized
by dividing the primary display width and height by `FACTOR`, currently
2 in the [Makefile](Makefile). A lower factor gives more pixels at the
same complex extent and increases the work per redraw. Deep zoom also
depends on numerical precision. Some calculations use `long double`,
whose precision varies by platform.

## Running tests

After installing the build dependencies and initializing the submodules,
run `make test`. It checks numerical orbits, poles, Newton basins,
palette cycles, smoothing, distance shading, and keyboard navigation.
It also checks command-line startup and renders
every fractal with SDL's dummy video driver, checks that **Q** restores
the small-offset view, and verifies that an unchanged build skips
compilation and linking. GitHub Actions uses the same target on macOS.

Run `norminette srcs inc tests` locally for the 42 style check. GitHub
Actions includes it in the **Fractol CI** check.

## Contributing

Branch from `dev` and open a pull request back into `dev`. To promote
the tested development branch, open a pull request from this repository's
`dev` into `master`.

Both protected branches require a pull request with a passing
**Fractol CI** check. Only [@manon42bcn] can merge into them.

[libft]: https://github.com/manon42bcn/libft
[rgb]: https://github.com/manon42bcn/rgb
[SDL3 Linux instructions]: https://wiki.libsdl.org/SDL3/README-linux
[SDL3 MinGW instructions]: https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-mingw.md
[SDL streaming texture API]: https://wiki.libsdl.org/SDL3/SDL_LockTexture
[Oklab]: https://bottosson.github.io/posts/oklab/
[@manon42bcn]: https://github.com/manon42bcn
