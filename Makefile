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
					fo_fractal_dir.c \
					fo_fractal_tests.c \
					fo_fractal_extra.c \
					fo_fractal_utils.c \
					fo_handlers.c \
					fo_input_utils.c \
					fo_iterator.c \
					fo_messages.c \
					fo_pixel_utils.c \
					fo_mouse_zoom.c \
					fo_string_to_double.c \
					fo_struct_utils.c \
					main.c
OBJS			=	$(addprefix $(OBJ_DIR)/,$(SRCS_FILES:.c=.o))
LIB_DIR			=	lib
LIB				=	libft.a
RM				=	rm -rf
CFLAGS			=	-Wall -Wextra -Werror
PKG_CONFIG		?=	pkg-config
SDL_MIN_VERSION	=	3.2.0
SDL_CFLAGS		=	$(shell $(PKG_CONFIG) --cflags sdl3)
SDL_LIBS		=	$(shell $(PKG_CONFIG) --libs sdl3)
INCLUDES		=	-Iinc -Ilib/inc
FACTOR			=	-D FACTOR=3

all: $(NAME)

$(NAME): $(OBJS) $(LIB_DIR)/$(LIB)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIB_DIR)/$(LIB) $(SDL_LIBS) -lm -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c inc/fractol.h $(LIB_DIR)/inc/libft.h | $(OBJ_DIR) check-sdl
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

clean:
	$(MAKE) -C $(LIB_DIR) clean
	$(RM) $(OBJS)

fclean: clean
	$(MAKE) -C $(LIB_DIR) fclean
	$(RM) $(NAME) $(OBJ_DIR)

re: fclean
	$(MAKE) all

.PHONY: all install-sdl check-sdl clean fclean re FORCE
