NAME = cube.a
CC = gcc
INCLUDES = ./includes
FLAGS = -Wall -Wextra -Werror
LINUX_FLAGS = -lbsd -lmlx -lXext -lX11
GNL_FILES = ./gnl/get_next_line.c ./gnl/get_next_line_utils.c

all:
	$(CC) -I$(INCLUDES) $(FLAGS) main.c map.c drawing.c control.c raycasting.c $(GNL_FILES) ./libft/libft.a -lm $(LINUX_FLAGS)

.PHONY: all clean fclean re