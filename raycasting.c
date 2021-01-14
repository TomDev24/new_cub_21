#include "cube.h"

void    init_ray_struct(t_ray_info *ray_info, t_all *game)
{
    ray_info->FOV = 1;
    ray_info->FOV_half= ray_info->FOV/2;
    ray_info->dt_a = ray_info->FOV / game->map_info->resolution.x;
    //printf("delta angle %f\n", ray_info->dt_a);
    ray_info->max_depth = game->map_info->resolution.x * 2;
    //ray_info->rays_amount = 600;
    ray_info->dist = game->map_info->resolution.x / (2 * tan(ray_info->FOV_half));
    ray_info->proj_coef = 1 * ray_info->dist * game->map_info->tile;
}

/*
static t_vector endray_point(t_player *player, float angle)
{
    t_vector endpoint;
    int ray_length = 100;

    endpoint.x = player->pos.x + ray_length * cosf(angle);
    endpoint.y = player->pos.y + ray_length * sinf(angle);

    
    printf("sin %f\n", sinf(player->angle));
    printf("cos %f\n", cosf(player->angle));
    printf("x end %f\n", endpoint.x);
    printf("y end %f\n", endpoint.y );
    
    //printf("\nEnd point x:%f and y: %f\n", endpoint.x, endpoint.y);
    return endpoint;
} */

void    raycast(t_all *game, t_player *player)
{
    float cur_angle;
    float ray_len = 0;
    int proj_h = 0;
    int ray_num = 0;
    int tile;
    char *color;

    float cos_a;
    float sin_a;

    t_vector t;
    t_vector b;
    t_vector endpoint;
    t_ray_info ray_info;

    tile = game->map_info->tile;
    ray_len = 0;
    endpoint.x = 0;
    endpoint.y = 0;
    
    init_ray_struct(&ray_info, game);
    cur_angle = player->angle - ray_info.FOV_half;
    while(ray_num < game->map_info->resolution.x)
    {
        cos_a = cosf(cur_angle);
        sin_a = sinf(cur_angle);
        while(ray_len < ray_info.max_depth){
            endpoint.x = player->pos.x + ray_len * cos_a;
            endpoint.y = player->pos.y + ray_len * sin_a;
            //printf("Endpoints x: %d, y: %d\n", (int)(endpoint.x/20), (int)(endpoint.y/20));
            
            //it looks when we divide endpoints by same value (but width and heigth could be diffrent, check this)
            if (endpoint.x /tile >= game->map_info->width || endpoint.y /tile >= game->map_info->height || game->map_info->full_map[(int)(endpoint.y/tile)][(int)endpoint.x/tile] == '1')
            {
                ray_len *= cosf(player->angle - cur_angle);
                //ray
                //draw_line3(game->mlx_info->mlx, game->mlx_info->win, player->pos, endpoint, game->surface);
                proj_h = 0;
                if (ray_len > 0)
                    proj_h = (ray_info.proj_coef)/ ray_len;
                t.x = ray_num;
                b.x = ray_num;

                t.y = game->map_info->resolution.y / 2 - (proj_h / 2);
                if (t.y < 0)
                    t.y = 0;
                b.y = game->map_info->resolution.y / 2 + (proj_h / 2);
                if (b.y > game->map_info->resolution.y)
                    b.y = game->map_info->resolution.y;
                //printf("End point x: %f,   y: %f\n", endpoint.x, endpoint.y);
                //printf("Project height %d\n", proj_h);

                color = get_color_ftex(game->tex_info, (t.x / game->map_info->resolution.x) * 31, (t.y / (float)proj_h) * 31);
                //my vector swap in draw line algo, does interesting thing
                if (b.y > t.y)
                    draw_line3(game->mlx_info->mlx, game->mlx_info->win, t, b, game->surface, color);
                break;
            }
            ray_len++;
        }

        cur_angle += ray_info.dt_a;
        ray_num += 1;
        ray_len = 0;
    }   
}

void    raycast2(t_all *game, t_player *player)
{
    float cur_angle;
    float ray_len = 0;
    int ray_num = 0;

    float cos_a;
    float sin_a;
    int tile;
    //int x = 0;
    //int y = 0;
    t_vector endpoint;
    t_ray_info ray_info;

    tile = game->map_info->tile;
    ray_len = 20;
    endpoint.x = 20;
    endpoint.y = 20;
    
    init_ray_struct(&ray_info, game);
    cur_angle = player->angle - ray_info.FOV_half;
    while(ray_num < game->map_info->resolution.x)
    {
        cos_a = cosf(cur_angle);
        sin_a = sinf(cur_angle);
        while(ray_len < ray_info.max_depth){
            endpoint.x = player->pos.x + ray_len * cos_a;
            endpoint.y = player->pos.y + ray_len * sin_a;
            //printf("Endpoints x: %d, y: %d\n", (int)(endpoint.x/20), (int)(endpoint.y/20));
            if (endpoint.x /tile >= game->map_info->width || endpoint.y /tile >= game->map_info->height || game->map_info->full_map[(int)(endpoint.y/tile)][(int)endpoint.x/tile] == '1')
            {
                //ray
                draw_line3(game->mlx_info->mlx, game->mlx_info->win, player->pos, endpoint, game->surface, NULL);
                break;
            }
            ray_len += 1;
        }
        
        /*
        while(game->map_info->full_map[(int)(endpoint.y/20)][2] != '1' || ray_len == 0)
        {
            draw_line(game->mlx_info->mlx, game->mlx_info->win, player->pos, endpoint, game->surface);
            ray_len++;
        }*/
        //draw_line(game->mlx_info->mlx, game->mlx_info->win, player->pos, endray_point(player, cur_angle), game->surface);
        //printf("Current angle %f\n", cur_angle);
        cur_angle += ray_info.dt_a;
        ray_num ++;
        ray_len = 0;
    }   
}