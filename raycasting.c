#include "cube.h"

void    init_ray_struct(t_ray_info *ray_info)
{
    ray_info->FOV = 1.1;
    ray_info->FOV_half= ray_info->FOV/2;
    ray_info->dt_a = ray_info->FOV / (float)600;
    //printf("delta angle %f\n", ray_info->dt_a);
    ray_info->max_depth = 800;
    ray_info->rays_amount = 600;
    ray_info->dist = 600 / (2 * tan(ray_info->FOV_half));
    ray_info->proj_coef = 2 * ray_info->dist * 60;
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

    float cos_a;
    float sin_a;

    t_vector t;
    t_vector b;
    t_vector endpoint;
    t_ray_info ray_info;

    ray_len = 0;
    endpoint.x = 0;
    endpoint.y = 0;
    
    init_ray_struct(&ray_info);
    cur_angle = player->angle - ray_info.FOV_half;
    while(ray_num < 600)
    {
        cos_a = cosf(cur_angle);
        sin_a = sinf(cur_angle);
        while(ray_len < ray_info.max_depth){
            endpoint.x = player->pos.x + ray_len * cos_a;
            endpoint.y = player->pos.y + ray_len * sin_a;
            //printf("Endpoints x: %d, y: %d\n", (int)(endpoint.x/20), (int)(endpoint.y/20));
            if (endpoint.x /100 >= game->map_info->width || endpoint.y /120 >= game->map_info->height || game->map_info->full_map[(int)(endpoint.y/120)][(int)endpoint.x/100] == '1')
            {
                ray_len *= cosf(player->angle - cur_angle);
                //ray
                //draw_line3(game->mlx_info->mlx, game->mlx_info->win, player->pos, endpoint, game->surface);
                proj_h = 0;
                if (ray_len > 0)
                    proj_h = (ray_info.dist * 300 )/ ray_len;
                t.x = ray_num;
                b.x = ray_num;

                t.y = 300 - proj_h;
                if (t.y < 0)
                    t.y = 0;
                b.y = proj_h;
                if (b.y > 600)
                    b.y = 600;
                //printf("End point x: %f,   y: %f\n", endpoint.x, endpoint.y);
                //printf("Project height %d\n", proj_h);
                draw_line3(game->mlx_info->mlx, game->mlx_info->win, t, b, game->surface);
                break;
            }
            ray_len++;
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
        ray_num += 2;
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
    //int x = 0;
    //int y = 0;
    t_vector endpoint;
    t_ray_info ray_info;

    ray_len = 20;
    endpoint.x = 20;
    endpoint.y = 20;
    
    init_ray_struct(&ray_info);
    cur_angle = player->angle - ray_info.FOV_half;
    while(ray_num < 600)
    {
        cos_a = cosf(cur_angle);
        sin_a = sinf(cur_angle);
        while(ray_len < ray_info.max_depth){
            endpoint.x = player->pos.x + ray_len * cos_a;
            endpoint.y = player->pos.y + ray_len * sin_a;
            //printf("Endpoints x: %d, y: %d\n", (int)(endpoint.x/20), (int)(endpoint.y/20));
            if (endpoint.x /100 >= game->map_info->width || endpoint.y /120 >= game->map_info->height || game->map_info->full_map[(int)(endpoint.y/120)][(int)endpoint.x/100] == '1')
            {
                //ray
                draw_line3(game->mlx_info->mlx, game->mlx_info->win, player->pos, endpoint, game->surface);
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