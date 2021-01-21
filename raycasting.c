#include "cube.h"

static float round3(float num)
{
    float val;

    val = (int)(num * 10000 + .5);
    return ((float)val/10000);
}


// cool unit cicle
//http://live.mephist.ru/show/unit-circle/
void    dda(t_all *game, t_player *player)
{
    float cur_angle;
    float ray_len = 0;
    int proj_h = 0;
    int ray_num = 0;
    int offset = 0;
    char *color;

    float cos_a;
    float sin_a;

    float invert_cos;
    float invert_sin;

    int tile_x = game->map_info->tile.x;
    int tile_y = game->map_info->tile.y;

    int x_m = (player->pos.x / tile_x) * tile_x;
    int y_m = (player->pos.y / tile_y) * tile_y;
    int dx;
    int dy;
    float dep_v;
    float dep_h;

    t_vector t;
    t_vector b;
    t_vector buff; //for fisrt endpoint.x and .y
    t_vector endpoint;
    
    cur_angle = player->angle - game->ray_info->FOV_half;
    
    //printf("tiles of x %d and y %d \n", tile_x, tile_y);
    //printf("x_m %d y_m %d\n", x_m, y_m);
    while(ray_num < game->ray_info->rays_amount)
    {
        cos_a = round3(cosf(cur_angle));
        sin_a = round3(sinf(cur_angle));

        invert_cos = (1 / cos_a); // becaouse of round, wall detect doesnt work sometime
        invert_sin = (1 / sin_a);

        //printf("Inv_Cos is %f and Inv_sin is %f\n", invert_cos, invert_sin);
        /*
        if (sin_a == 0)
            sin_a = 0.00001;
        if (cos_a == 0)
            cos_a = 0.00001;
        */
        if (cos_a >= 0)
        {
            endpoint.x = x_m + tile_x;
            dx = 1;
        } else
        {
            endpoint.x = x_m;
            dx = -1;
        }
        for (int i = 0; i < game->map_info->resolution.x; i += tile_x)
        {
            dep_v = (endpoint.x - player->pos.x) * invert_cos;
            //printf("dep_v is %f\n", dep_v);
            endpoint.y = player->pos.y + (dep_v * sin_a);
            //printf("FIRST endpoint x %f  and y %f\n", endpoint.x, endpoint.y);
            //printf("FIRST endpoint x %d  and y %d\n", ((int)(endpoint.x + dx)/tile_x), (int)(endpoint.y / tile_y) );
            if (endpoint.x < 0 || endpoint.y < 0 || (endpoint.x + dx) /tile_x >= game->map_info->width || endpoint.y /tile_y >= game->map_info->height 
                || game->map_info->full_map[(endpoint.y/tile_y)][((endpoint.x + dx)/tile_x)] == '1')
            {
                //printf("FIRST endpoint x %d  and y %d\n", ((int)(endpoint.x + dx)/tile_x), (int)(endpoint.y / tile_y) );
                break; 
            }
            endpoint.x += dx * tile_x;
            //printf("wtf %d", dx * tile_x);
        }
        
        buff.x = endpoint.x;
        buff.y = endpoint.y;

        if (sin_a >= 0)
        {
            endpoint.y = y_m + tile_y;
            dy = 1;
        } else
        {
            endpoint.y = y_m;
            dy = -1;
        }
        //printf("BEFORE SEC LOOP player pos.y %f  and y %f\n", player->pos.y, endpoint.y);
        //printf("map y heigth %d\n", game->map_info->height * tile_y);
        //printf("RES y heigth %f\n", game->map_info->resolution.y );
        for (int j = 0; j < game->map_info->resolution.y; j += tile_y)
        {
            dep_h = (endpoint.y - player->pos.y) * invert_sin;
            //printf("dep_h is %f\n", dep_h);
            endpoint.x = player->pos.x + (dep_h * cos_a);
        
            //printf("SECOND endpoint x %f  and y %f\n", endpoint.x, endpoint.y);
            //printf("SECOND endpoint x %d  and y %d\n", ((int)endpoint.x/tile_x), (int)(endpoint.y + dy)/tile_y );
            if ( endpoint.x < 0 || endpoint.y < 0 || (endpoint.x) /tile_x >= game->map_info->width || (endpoint.y + dy)/tile_y >= game->map_info->height 
                || game->map_info->full_map[(int)((endpoint.y + dy)/tile_y)][((int)(endpoint.x))/tile_x] == '1'
            )
            {
                //printf("SECOND endpoint x %d  and y %d\n", ((int)endpoint.x/tile_x), (int)(endpoint.y + dy)/tile_y );
                break; 
            }
            endpoint.y += dy * tile_y;
        }
        
        if (dep_v < dep_h)
        {
            endpoint.y = buff.y;

            game->vert_texture = 1;
            offset = endpoint.y % tile_y;
            //printf("y %d mod tile_y %d is %d \n", endpoint.y, tile_y, endpoint.y % tile_y);
            ray_len = dep_v;
        }
        else
        {
            game->vert_texture = 0;
            offset = endpoint.x % tile_x;
            //printf("x is %d\n", tile_x);
            ray_len = dep_h;
        }
        ray_len *= cosf(player->angle - cur_angle);
        proj_h = 0;
        if (ray_len < 0.05)
            proj_h = game->map_info->resolution.y;
        else
            proj_h = (game->ray_info->proj_coef)/ ray_len;
        t.x = ray_num * game->ray_info->ray_scale;
        b.x = (ray_num * game->ray_info->ray_scale) + game->ray_info->ray_scale;

        t.y = game->map_info->resolution.y / 2 - (proj_h / 2);
        if (t.y < 0)
            t.y = 0;
        b.y = game->map_info->resolution.y / 2 + (proj_h / 2);
        if (b.y > game->map_info->resolution.y)
            b.y = game->map_info->resolution.y;

        //color = aplly_tex(game, offset);
        color++;
        if (b.y > t.y)
            //draw_line3(game->mlx_info->mlx, game->mlx_info->win, t, b, game->surface, color);
            draw_tex_rect(game, t, b, game->surface, offset, proj_h);
        
        cur_angle += game->ray_info->dt_a;
        ray_num += 1;
    }   
}