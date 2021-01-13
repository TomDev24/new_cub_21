#ifndef CUBE_H
# define CUBE_H

#include <stdio.h>
#include <math.h>
#include <mlx.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../libft/libft.h"
#include "../gnl/get_next_line.h"

#include "vector.h"
#include "settings.h"

void draw_map(void *mlx, void *win, t_map *map);

int parse_map(char *file, t_map *map_info);
void fill_black(void *mlx, void *win, t_img *img);
//debug
void    raycast2(t_all *game, t_player *player);
void open_print(char *name);
int     key_hook(int keycode, void *nall);
int     movement(int keycode, t_player *player);

t_img create_surface(void *mlx, int width, int height);
void draw_line(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img);
void draw_line2(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img);
void draw_line3(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img);
void draw_rect(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img);

void    raycast(t_all *game, t_player *player);

#endif