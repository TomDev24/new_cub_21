#ifndef CUBE_H
# define CUBE_H

#define M_WIDTH 24
#define M_HEIGTH 24

typedef struct vector
{
    int x;
    int y;
}              t_vector;

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

void fill_map(int map[M_WIDTH][M_HEIGTH]);
char **gen_map(int rows, int cols);

void print_map(int map[M_WIDTH][M_HEIGTH]);
void print_map2(char **map);

int parse_map(char *file, t_map *map_info);

//debug
void open_print(char *name);

void draw_line(void *mlx, void *win, t_vector vec1, t_vector vec2);

#endif