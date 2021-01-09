NAME = cube.a
CC = gcc
FLAGS = -Wall -Wextra -Werror
LINUX_FLAGS = -lbsd -lmlx -lXext -lX11
GNL_FILES = ./gnl/get_next_line.c ./gnl/get_next_line_utils.c

all:
	$(CC) $(FLAGS) main.c map.c drawing.c control.c $(GNL_FILES) ./libft/libft.a -lm $(LINUX_FLAGS)

.PHONY: all clean fclean re