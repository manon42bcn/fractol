#!/bin/sh
set -eu

fractol=$1
export SDL_VIDEODRIVER=fractol-test-invalid

help_output=$("$fractol" --help)
printf '%s\n' "$help_output" | grep -Fq 'Fractol starts in Mandelbrot.'
printf '%s\n' "$help_output" | grep -Fq 'Shift+Tab: previous fractal.'

for old_name in mandelbrot julia buddhabrot rational-square; do
    if output=$("$fractol" "$old_name" 2>&1); then
        printf 'Unexpectedly accepted fractal name: %s\n' "$old_name" >&2
        exit 1
    fi
    printf '%s\n' "$output" | grep -Fq 'Usage: ./fractol'
done
printf '%s\n' 'Command-line tests passed.'
