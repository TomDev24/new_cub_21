#include "cube.h"

t_vector endray_point(t_player *player)
{
    t_vector endpoint;
    int ray_length = 100;

    endpoint.x = (player->pos.x + ray_length * cosf(player->angle));
    endpoint.y = (player->pos.y + ray_length * sinf(player->angle));

    //printf("End point x:%f and y: %f\n", endpoint.x, endpoint.y);
    return endpoint;
}

int rend(t_all *game_params)
{
    /*
    t_img surface;

    mlx_destroy_image(game_params->mlx_info->mlx, game_params->surface->img);
    surface = create_surface(game_params->mlx_info->mlx, 600, 600);
    game_params->surface = &surface;
    */

    // cpu drops from 14 to 2, when i dont use fill black
    fill_black(game_params->mlx_info->mlx, game_params->mlx_info->win, game_params->surface, game_params->map_info);
    

    //draw_map(game_params->mlx_info->mlx, game_params->mlx_info->win, game_params->map_info);

    //draw_grid(game_params);
    dda(game_params, game_params->player);
    if (game_params->mini_ray)
        raycast2(game_params, game_params->player);
    //draw_rect(game_params->mlx_info->mlx, game_params->mlx_info->win, game_params->player->pos, add_vector(game_params->player->pos, game_params->player->size), game_params->surface);
    //draw_line3(game_params->mlx_info->mlx, game_params->mlx_info->win, game_params->player->pos, endray_point(game_params->player), game_params->surface);
    //clear window is much faster and doesnt work
    //mlx_clear_window(game_params->mlx_info->mlx, game_params->mlx_info->win);
    return (1);
}

int main(int argc, char **argv)
{
    t_all   game_params;
    t_player player;
    t_img surface;
    t_img tex;
    t_map map_info;
    t_mlx mlx_info;
    t_vector v1;
    t_vector v2;
    v1.x =300; v1.y = 300;
    v2.x = 500; v2.y = 400;
    
    if (argc > 1)
    {
        //open_print(argv[1]);
        if (parse_map(argv[1], &map_info) == -1)
            return(-1);
        if(argv[2])
            game_params.mini_ray = argv[2];
    }
    mlx_info.mlx = mlx_init();
    mlx_info.win = mlx_new_window(mlx_info.mlx, map_info.resolution.x, map_info.resolution.y, "Yes");
    
    surface = create_surface(mlx_info.mlx, map_info.resolution.x, map_info.resolution.y);
    //map_info.map = gen_map(3, 3); now wont work
    //print_map2(map_info.map);

    draw_line3(mlx_info.mlx, mlx_info.win, v1, v2, &surface, NULL);
    player.pos.x = 201; player.pos.y = 241;
    player.size.x = 10;
    player.size.y = 10;
    player.angle = 3.141;
    player.speed = 3;
    //player
    draw_rect(mlx_info.mlx, mlx_info.win, player.pos, add_vector(player.pos, player.size), &surface);
    //draw_map(mlx_info.mlx, mlx_info.win, &map_info);

    //it work when there is no mlx_loop_hook
    //mlx_clear_window(mlx_info.mlx, mlx_info.win);

    game_params.player = &player;
    game_params.surface = &surface;
    game_params.mlx_info = &mlx_info;
    game_params.map_info = &map_info;

    get_tex(mlx_info.mlx, mlx_info.win, "./textures/brick.xpm", &tex);
    game_params.tex_info = &tex;
    
    
    //mlx_key_hook(mlx_info.win, key_hook, NULL);
    mlx_hook(mlx_info.win, 2, 1L<<0, movement, &player); //2 is key pressed event,
    mlx_loop_hook(mlx_info.mlx, rend, &game_params);
    mlx_loop(mlx_info.mlx);
    return (0);
}