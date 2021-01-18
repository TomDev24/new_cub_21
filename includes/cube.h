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
void fill_black(void *mlx, void *win, t_img *img, t_map *map);
//debug
void    display_tex(void *mlx, void *win, char *file);
void open_print(char *name);
int     key_hook(int keycode, void *nall);
int     movement(int keycode, t_player *player);

char *get_color_ftex(t_img *tex_info, int x, int y);
void    get_tex(void *mlx, void *win, char *file, t_img *tex_info);
t_img create_surface(void *mlx, int width, int height);
void draw_line(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img);
void draw_line2(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img);
void draw_line3(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img, char *color);
void draw_rect(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img, char *col);
void draw_tex_rect(t_all *game, t_vector vec1, t_vector vec2, t_img *img, int offset, int proj_h);

void    raycast(t_all *game, t_player *player);
void    raycast2(t_all *game, t_player *player);
void    dda(t_all *game, t_player *player);
//void    dda2(t_all *game, t_player *player);
void    draw_grid(t_all *game);

#endif