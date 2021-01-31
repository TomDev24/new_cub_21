#include "cube.h"

void    init_ray_struct(t_ray_info *ray_info, t_all *game)
{
    ray_info->FOV = 1;
    ray_info->ray_scale = 4;
    ray_info->d_pi = 3.14159265358979323846 * 2; // if M_PI doesn exists
    ray_info->rays_amount = game->map_info->resolution.x / ray_info->ray_scale;
    ray_info->FOV_half= ray_info->FOV/2;
    ray_info->dt_a = ray_info->FOV / (float)ray_info->rays_amount;
    ray_info->center_ray = ray_info->rays_amount / 2 - 1;
    ray_info->max_depth = game->map_info->resolution.x * 2;
    ray_info->dist = game->map_info->resolution.x / (2 * tan(ray_info->FOV_half));
    ray_info->proj_coef = 1 * ray_info->dist * game->map_info->tile.y;
}

t_vert_line *alloc_arr_of_lines(t_all *game)
{
    t_vert_line *res;

    res = (t_vert_line *)ft_calloc((game->ray_info->rays_amount + 2), sizeof(t_vert_line)); // 2 - just one sprite now, and NULL
    if (!res)
        return (NULL);
    //res[game->ray_info->rays_amount + 2] = NULL;
    return (res);
}

void    print_lines(t_vert_line *lines, t_all *game)
{
    int i;

    i = 0;
    printf("-----------Dictionary of lines\n");
    while (i < game->ray_info->rays_amount)
    {
        printf("//Line: len is %f offsset is %d  proj heigth %d\n", lines[i].ray_len, lines[i].offset, lines[i].proj_h);
        i++;
    }
}

void     draw_lines(t_all *game, t_vert_line *lines)
{
    int i;
    t_vector sprite_pos;

    sprite_pos.x = -1;
    sprite_pos.y = -1;
    i = 0;
    game->first_sprite_ray = NULL;
    while (i < game->ray_info->rays_amount)
    {

        //if (sprite_pos.x !=-1 && sprite_pos.y != -1)
            //printf("sprite at x %d  y %d\n", sprite_pos.x, sprite_pos.y);

        //printf("i is probm %d\n", i);
        //printf("//Line: len is %f offsset is %d  proj heigth %d\n", lines[i].ray_len, lines[i].offset, lines[i].proj_h);
        game->vert_texture = lines[i].vert_text;
        //if (lines[i].is_sprite != '2')
        draw_tex_rect(game, lines[i].t, lines[i].b, game->surface, lines[i].offset, lines[i].proj_h, '1');
            //draw_sprite(game, lines, &i);
        if (lines[i].sprite_pos.x != sprite_pos.x || lines[i].sprite_pos.y != sprite_pos.y)
        {
            sprite_pos.x = lines->sprite_pos.x;
            sprite_pos.y = lines->sprite_pos.y;
        
            draw_sprite2(game, &lines[i], lines);
        }
        i++;
    }
}

void    swap(t_vert_line *a, t_vert_line *b)
{
    t_vert_line t = *a;
    *a = *b;
    *b = t;
}
char    compare(t_vert_line a, t_vert_line b)
{
    // comparision determines asceding or desendet order
    if (a.ray_len < b.ray_len)
        return (1);
    else
        return (0);
}

//bubble sort, then probably better to implement better sort algo
void    sort_lines(t_vert_line *lines, int n)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (i < n - 1)
    {
        while (j < n - i - 1)
        {
            if (compare(lines[j], lines[j+1]))
                swap(&lines[j], &lines[j+1]);
            j++;
        }
        j = 0;
        i++;
    }
}

int rend(t_all *game_params)
{
    t_vert_line *lines;

    lines = NULL;
    fill_black(game_params->mlx_info->mlx, game_params->mlx_info->win, game_params->surface, game_params->map_info);
    lines = alloc_arr_of_lines(game_params);
    dda(game_params, game_params->player, lines);
    
    sort_lines(lines, game_params->ray_info->rays_amount);
    //print_lines(lines, game_params);
    draw_lines(game_params, lines);
    //we need to put image, after dda is done
    //mlx_put_image_to_window(game_params->mlx_info->mlx, game_params->mlx_info->win, game_params->surface->img, 0,0);
    //mlx_put_image_to_window(game_params->mlx_info->mlx, game_params->mlx_info->win, game_params->sprite_info->img, 0,0);
    //raycast2(game_params, game_params->player);
    //draw_grid(game_params);
    if (lines != NULL)
        free(lines);
    return (1);
}

float   tile_diagonal(t_map *map_info)
{
    float res;

    res = sqrt((map_info->tile.x * map_info->tile.x) + (map_info->tile.y * map_info->tile.y));

    return res;
}

int main(int argc, char **argv)
{
    t_all   game_params;
    t_player player;
    t_img surface;
    t_img black_sc;
    t_img tex;
    t_img sprite;
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
    player.angle = 0;
    player.speed = 3;

    game_params.player = &player;
    game_params.surface = &surface;
    game_params.mlx_info = &mlx_info;
    game_params.map_info = &map_info;
    game_params.black_sc = &black_sc;

    get_tex(mlx_info.mlx, mlx_info.win, "./textures/brick.xpm", &tex);
    get_tex(mlx_info.mlx, mlx_info.win, "./textures/pilar.xpm", &sprite);
    game_params.tex_info = &tex;
    game_params.sprite_info = &sprite;
    mlx_put_image_to_window(mlx_info.mlx, mlx_info.win, tex.img, 30, 20);

    init_ray_struct(&ray_info, &game_params);
    player.rot_speed = ray_info.dt_a;
    game_params.ray_info = &ray_info;
    game_params.tile_diagonal = tile_diagonal(&map_info);

    printf("Diagonal %f\n", game_params.tile_diagonal);
    
    mlx_hook(mlx_info.win, 2, 1L<<0, movement, &player); //2 is key pressed event,
    mlx_loop_hook(mlx_info.mlx, rend, &game_params);
    mlx_loop(mlx_info.mlx);
    return (0);
}