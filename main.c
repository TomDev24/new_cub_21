#include "cube.h"

void    init_ray_struct(t_ray_info *ray_info, t_all *game)
{
    ray_info->FOV = 1;
    ray_info->ray_scale = 1;
    ray_info->rays_amount = game->map_info->resolution.x / ray_info->ray_scale;
    ray_info->FOV_half= ray_info->FOV/2;
    ray_info->dt_a = ray_info->FOV / (float)ray_info->rays_amount;
    //printf("delta angle %f\n", ray_info->dt_a);
    ray_info->max_depth = game->map_info->resolution.x * 2;
    ray_info->dist = game->map_info->resolution.x / (2 * tan(ray_info->FOV_half));
    ray_info->proj_coef = 1 * ray_info->dist * game->map_info->tile.y;
}

int rend(t_all *game_params)
{
    fill_black(game_params->mlx_info->mlx, game_params->mlx_info->win, game_params->surface, game_params->map_info);
    dda(game_params, game_params->player);
    
    //we need to put image, after dda is done
    mlx_put_image_to_window(game_params->mlx_info->mlx, game_params->mlx_info->win, game_params->surface->img, 0,0);
    return (1);
}

int main(int argc, char **argv)
{
    t_all   game_params;
    t_player player;
    t_img surface;
    t_img black_sc;
    t_img tex;
    t_map map_info;
    t_mlx mlx_info;
    t_ray_info ray_info;
    
    if (argc > 1)
    {
        if (parse_map(argv[1], &map_info) == -1)
            return(-1);
        if(argv[2])
            game_params.mini_ray = argv[2];
    } else
        return (-1);
    
    mlx_info.mlx = mlx_init();
    mlx_info.win = mlx_new_window(mlx_info.mlx, map_info.resolution.x, map_info.resolution.y, "Yes");
    
    surface = create_surface(mlx_info.mlx, map_info.resolution.x, map_info.resolution.y);

    player.pos.x = 201; player.pos.y = 241;
    player.size.x = 10;
    player.size.y = 10;
    player.angle = 1;
    player.speed = 3;

    game_params.player = &player;
    game_params.surface = &surface;
    game_params.mlx_info = &mlx_info;
    game_params.map_info = &map_info;
    game_params.black_sc = &black_sc;
    get_tex(mlx_info.mlx, mlx_info.win, "./textures/brick.xpm", &tex);
    game_params.tex_info = &tex;
    init_ray_struct(&ray_info, &game_params);
    game_params.ray_info = &ray_info;
    
    mlx_hook(mlx_info.win, 2, 1L<<0, movement, &player); //2 is key pressed event,
    mlx_loop_hook(mlx_info.mlx, rend, &game_params);
    mlx_loop(mlx_info.mlx);
    return (0);
}