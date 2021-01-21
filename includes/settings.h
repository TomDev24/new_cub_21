#ifndef SETTINGS_H
# define SETTINGS_H

#ifndef WIDTH
    # define WIDTH 700
#endif

#ifndef HEIGHT
    # define HEIGHT 700
#endif

#define TILE (WIDTH / 10)

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
    char**       full_map;
    int         width;
    int         height;
    t_vector         tile;
}              t_map;

typedef struct player
{
    t_vector pos;
    t_vector size;
    int      speed;
    float      angle;
}               t_player;

typedef struct ray_info
{
    float       dt_a;
    float       FOV;
    int         ray_scale;
    float       FOV_half;
    float       max_depth;
    int         rays_amount;
    float       dist;
    float       proj_coef;
}           t_ray_info;

typedef struct img
{
    void    *img;
    char    *addr;
    int     bits_per_pixel;
    int     line_length;
    int     endian;
}              t_img;

typedef struct mlx
{
    void    *win;
    void    *mlx;
}       t_mlx;

typedef struct all
{
    t_mlx   *mlx_info;
    t_player *player;
    t_img     *surface;
    t_img      *black_sc;
    t_map     *map_info;
    t_img     *tex_info;
    t_ray_info *ray_info;
    int       vert_texture; // put somewhere else
    char *    mini_ray;
}           t_all;

#endif