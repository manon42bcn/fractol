# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mporras <mporras@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/28 09:42:53 by mporras-          #+#    #+#              #
#    Updated: 2022/12/30 03:00:32 by mporras-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	fractol
SRC_DIR			=	srcs
OBJ_DIR			=	objs
SRCS_FILES		=	fo_colorize.c \
					fo_palette.c \
					fo_fractal_dir.c \
					fo_fractal_tests.c \
					fo_fractal_extra.c \
					fo_fractal_degree7.c \
					fo_fractal_phoenix.c \
					fo_fractal_tricorn.c \
					fo_fractal_shapes.c \
					fo_fractal_cosine.c \
					fo_fractal_sine.c \
					fo_fractal_rational.c \
					fo_fractal_rational_offset.c \
					fo_fractal_rational_square.c \
					fo_fractal_polynomial.c \
					fo_newton.c \
					fo_buddhabrot.c \
					fo_buddhabrot_orbit.c \
					fo_buddhabrot_draw.c \
					fo_nebulabrot_palette.c \
					fo_fractal_utils.c \
					fo_handlers.c \
					fo_color_keys.c \
					fo_events.c \
					fo_input_utils.c \
					fo_iterator.c \
					fo_messages.c \
					fo_pixel_utils.c \
					fo_mouse_zoom.c \
					fo_struct_utils.c \
					fo_video.c \
					fo_video_init.c \
					main.c
OBJS			=	$(addprefix $(OBJ_DIR)/,$(SRCS_FILES:.c=.o))
LIB_DIR			=	lib
LIB				=	libft.a
RGB_DIR			=	rgb
RGB_LIB			=	$(OBJ_DIR)/librgb.a
RM				=	rm -rf
CFLAGS			=	-Wall -Wextra -Werror
PKG_CONFIG		?=	pkg-config
SDL_MIN_VERSION	=	3.2.0
SDL_CFLAGS		=	$(shell $(PKG_CONFIG) --cflags sdl3)
SDL_LIBS		=	$(shell $(PKG_CONFIG) --libs sdl3)
INCLUDES		=	-Iinc -I$(LIB_DIR)/inc -I$(RGB_DIR)/inc
FACTOR			=	-D FACTOR=2
ORBIT_TEST_OBJS	=	$(addprefix $(OBJ_DIR)/,fo_fractal_degree7.o \
					fo_fractal_polynomial.o fo_fractal_rational.o \
					fo_fractal_rational_offset.o fo_fractal_rational_square.o \
					fo_fractal_utils.o fo_newton.o)
NAV_TEST_OBJS	=	$(filter-out $(OBJ_DIR)/main.o,$(OBJS))

all: $(NAME)

test: $(NAME) $(OBJ_DIR)/test-orbits $(OBJ_DIR)/test-navigation
	./$(OBJ_DIR)/test-orbits
	SDL_VIDEODRIVER=dummy SDL_RENDER_DRIVER=software ./$(OBJ_DIR)/test-navigation
	sh tests/test_cli.sh ./$(NAME)
	# An up-to-date build must not invoke the compiler or linker.
	$(MAKE) all CC=false

$(OBJ_DIR)/test-orbits: tests/test_orbits.c $(ORBIT_TEST_OBJS) | check-sdl
	$(CC) $(CPPFLAGS) $(CFLAGS) -UNDEBUG $(SDL_CFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $^ -lm

$(OBJ_DIR)/test-navigation: tests/test_navigation.c $(NAV_TEST_OBJS) $(LIB_DIR)/$(LIB) $(RGB_LIB) | check-sdl
	$(CC) $(CPPFLAGS) $(CFLAGS) -UNDEBUG $(SDL_CFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $^ $(SDL_LIBS) -lm

submodules:
	git submodule update --init --recursive

$(NAME): $(OBJS) $(LIB_DIR)/$(LIB) $(RGB_LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIB_DIR)/$(LIB) $(RGB_LIB) $(SDL_LIBS) -lm -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c inc/fractol.h $(RGB_DIR)/inc/rgb.h $(LIB_DIR)/inc/libft.h | $(OBJ_DIR) check-sdl
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SDL_CFLAGS) $(INCLUDES) $(FACTOR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

install-sdl:
	@system=$$(uname -s); \
	case "$$system" in \
		Darwin) \
			command -v brew >/dev/null 2>&1 || { \
				echo 'Homebrew not found. Install Homebrew to use install-sdl.' >&2; \
				exit 1; \
			}; \
			brew install sdl3 pkgconf && $(MAKE) check-sdl; \
			exit $$? ;; \
		Linux) \
			echo 'Linux detected. Install the SDL3 development package and pkg-config for your distribution.'; \
			echo 'SDL build instructions: https://wiki.libsdl.org/SDL3/README-linux' ;; \
		MINGW*|MSYS*|CYGWIN*|Windows_NT) \
			echo 'Windows detected. In MSYS2 UCRT64, install SDL3 and pkgconf with:'; \
			echo '  pacman -S mingw-w64-ucrt-x86_64-sdl3 mingw-w64-ucrt-x86_64-pkgconf'; \
			echo 'SDL setup instructions: https://github.com/libsdl-org/SDL/blob/main/docs/INTRO-mingw.md' ;; \
		*) \
			printf 'System detected: %s\n' "$$system"; \
			echo 'Install the SDL3 development package and pkg-config for your system.'; \
			echo 'SDL downloads: https://github.com/libsdl-org/SDL/releases/latest' ;; \
	esac; \
	echo 'After installation, run make check-sdl.'; \
	exit 1

check-sdl:
	@command -v "$(firstword $(PKG_CONFIG))" >/dev/null 2>&1 || { \
		echo 'pkg-config not found. Install pkg-config or pkgconf.' >&2; \
		exit 1; \
	}
	@version=$$($(PKG_CONFIG) --modversion sdl3 2>/dev/null) || { \
		echo 'SDL3 development files not found by pkg-config (SDL3 >= $(SDL_MIN_VERSION) required).' >&2; \
		exit 1; \
	}; \
	$(PKG_CONFIG) --atleast-version=$(SDL_MIN_VERSION) sdl3 || { \
		echo "SDL3 $$version detected. SDL3 >= $(SDL_MIN_VERSION) required." >&2; \
		exit 1; \
	}; \
	echo "SDL3 $$version detected."

$(LIB_DIR)/$(LIB): FORCE
	$(MAKE) -C $(LIB_DIR) -j1

$(RGB_LIB): FORCE | $(OBJ_DIR)
	$(MAKE) -C $(RGB_DIR) CC="$(CC)" NAME="$(abspath $@)"

clean:
	$(MAKE) -C $(LIB_DIR) clean
	$(MAKE) -C $(RGB_DIR) clean
	$(RM) $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(LIB_DIR) fclean
	$(MAKE) -C $(RGB_DIR) fclean NAME="$(abspath $(RGB_LIB))"
	$(RM) $(NAME)

re: fclean
	$(MAKE) all

.PHONY: all test submodules install-sdl check-sdl clean fclean re FORCE
