#ifndef CUBE_H
# define CUBE_H

#define M_WIDTH 24
#define M_HEIGTH 24

typedef struct vector
{
    float x;
    float y;
}              t_vector;

typedef struct player
{
    t_vector pos;
    t_vector size;
    int      speed;
}               t_player;


typedef struct map
{
    t_vector resolution;
    char*    north_t;
    char*    south_t;
    char*    west_t;
    char*    east_t;
    char*    sprite;
    unsigned int floor_c;
    unsigned int ceilling_c;
    char*        map;
    size_t         line_length;
}              t_map;

typedef struct img
{
    void    *img;
    char    *addr;
    int     bits_per_pixel;
    int     line_length;
    int     endian;
}              t_img;

t_vector add_vector(t_vector vec1, t_vector vec2);
void fill_map(int map[M_WIDTH][M_HEIGTH]);
char **gen_map(int rows, int cols);

void print_map(int map[M_WIDTH][M_HEIGTH]);
void print_map2(char **map);

int parse_map(char *file, t_map *map_info);
void fill_black(void *mlx, void *win, t_img *img);
//debug
void open_print(char *name);
int     key_hook(int keycode, void *nall);
int     movement(int keycode, t_player *player);

t_img create_surface(void *mlx, int width, int height);
void draw_line(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img);
void draw_rect(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img);

#endif