#include <stdio.h>
#include <mlx.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "cube.h"
#include "./libft/libft.h"
#include "./gnl/get_next_line.h"


typedef struct musor
{
    void *mlx;
    void *win;
}          t_musor;


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
}           t_all;

int rend(t_all *game_params)
{
    fill_black(game_params->mlx_info->mlx, game_params->mlx_info->win, game_params->surface);
    draw_rect(game_params->mlx_info->mlx, game_params->mlx_info->win, game_params->player->pos, add_vector(game_params->player->pos, game_params->player->size), game_params->surface);
    return (1);
}

int main(int argc, char **argv)
{
    t_all   game_params;
    t_player player;
    t_img surface;
    t_map map_info;
    t_mlx mlx_info;
    int map[M_WIDTH][M_HEIGTH];
    t_vector v1;
    t_vector v2;
    v1.x = 20; v1.y = 20;
    v2.x = 200; v2.y = 400;
    
    if (argc > 1)
    {
        //open_print(argv[1]);
        if (parse_map(argv[1], &map_info) == -1)
            return(-1);
    }
    fill_map(map);
    mlx_info.mlx = mlx_init();
    mlx_info.win = mlx_new_window(mlx_info.mlx, 600, 600, "Yes");
    
    surface = create_surface(mlx_info.mlx, 600, 600);
    //map_info.map = gen_map(3, 3); now wont work
    //print_map2(map_info.map);

    draw_line(mlx_info.mlx, mlx_info.win, v1, v2, &surface);
    player.pos.x = 300; player.pos.y = 300;
    player.size.x = 30;
    player.size.y = 30;
    player.speed = 3;
    //player
    draw_rect(mlx_info.mlx, mlx_info.win, player.pos, add_vector(player.pos, player.size), &surface);
    //draw_rect(mlx, win, v1, v2, &surface);

    game_params.player = &player;
    game_params.surface = &surface;
    game_params.mlx_info = &mlx_info;
    /*
    if (&game_params == (t_all*)&player)
        v1.x = v1.x; */

    //mlx_key_hook(win, key_hook, NULL);
    mlx_hook(mlx_info.win, 2, 1L<<0, movement, &player); //2 is key pressed event,
    mlx_loop_hook(mlx_info.mlx, rend, &game_params);
    mlx_loop(mlx_info.mlx);
    return (0);
}