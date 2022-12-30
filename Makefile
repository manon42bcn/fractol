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
OBJ_SUBS		=	objs
HEADER_DIR		=	inc
SRCS_FILES		=	fo_colorize.c \
					fo_fractal_dir.c \
					fo_fractal_tests.c \
					fo_fractal_extra.c \
					fo_fractal_utils.c \
					fo_handlers.c \
					fo_input_utils.c \
					fo_iterator.c \
					fo_messages.c \
					fo_minilibx_utils.c \
					fo_mouse_zoom.c \
					fo_string_to_double.c \
					fo_struct_utils.c \
					main.c
SRCS 			=	$(addprefix $(SRC_DIR)/,$(SRCS_FILES))
OBJS			=	$(addprefix $(OBJ_DIR)/,$(SRCS_FILES:.c=.o))
LIB_DIR			=	lib
MLX_DIR			=	mlx
LIB 			=	libft.a
MLX 			=	libmlx.dylib
CC				=	gcc
RM				= 	rm	-rf
CFLAGS			=	-Wall -Wextra -Werror
MLX_LNK			= 	-L./$(MLX_DIR)/ -lmlx -framework OpenGL -framework AppKit
INCLUDES		=	-Iinc -I./mlx/ -Ilib/inc
FACTOR 			=	-D FACTOR=3

all: $(OBJ_SUBS) libs $(NAME)
 
$(NAME): $(OBJS) $(LIB_DIR)/$(LIB)
	$(CC) $(OBJS) $(CFLAGS) $(LIB_DIR)/$(LIB) $(MLX_DIR)/$(MLX) $(MLX_LNK) -lm -o $(NAME)


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c inc/fractol.h $(LIB_DIR)/inc/libft.h
	$(CC) $(CFLAGS) $(INCLUDES) $(FACTOR) -c $< -o $@

$(OBJ_SUBS):
	-mkdir $(OBJ_DIR)

libs:
	$(MAKE) -C $(LIB_DIR)
	$(MAKE) -C ./mlx/
	cp mlx/libmlx.dylib .

clean:
	$(MAKE) -C $(LIB_DIR) clean
	$(MAKE) -C ./mlx/ clean
	$(RM) $(MLX)
	$(RM) $(OBJS)

fclean: clean
	$(MAKE) -C $(LIB_DIR) fclean
	$(RM) $(NAME)
	$(RM) $(OBJ_SUBS)

re:	fclean all

.PHONY: all clean fclean re
