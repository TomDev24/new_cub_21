#include "cube.h"

void    init_ray_struct(t_ray_info *ray_info, t_all *game)
{
    ray_info->FOV = 1;
    ray_info->ray_scale = 5;
    ray_info->rays_amount = game->map_info->resolution.x / ray_info->ray_scale;
    ray_info->FOV_half= ray_info->FOV/2;
    ray_info->dt_a = ray_info->FOV / (float)ray_info->rays_amount;
    //printf("delta angle %f\n", ray_info->dt_a);
    ray_info->max_depth = game->map_info->resolution.x * 2;
    ray_info->dist = game->map_info->resolution.x / (2 * tan(ray_info->FOV_half));
    ray_info->proj_coef = 1 * ray_info->dist * game->map_info->tile.y;
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

    tile = game->map_info->tile.y;
    ray_len = 0;
    endpoint.x = 0;
    endpoint.y = 0;
    
    init_ray_struct(&ray_info, game);
    cur_angle = player->angle - ray_info.FOV_half;
    
    //printf("rays %d\n", ray_info.rays_amount);
    while(ray_num < ray_info.rays_amount)
    {
        printf("ray num %d\n", ray_num);
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
                t.x = ray_num * ray_info.ray_scale;
                b.x = (ray_num * ray_info.ray_scale)+ ray_info.ray_scale;

                t.y = game->map_info->resolution.y / 2 - (proj_h / 2);
                if (t.y < 0)
                    t.y = 0;
                b.y = game->map_info->resolution.y / 2 + (proj_h / 2);
                if (b.y > game->map_info->resolution.y)
                    b.y = game->map_info->resolution.y;
                //printf("End point x: %f,   y: %f\n", endpoint.x, endpoint.y);
                //printf("Project height %d\n", proj_h);

                //color = get_color_ftex(game->tex_info, (t.x / game->map_info->resolution.x) * 31, (t.y / (float)proj_h) * 31);
                color++;
                //my vector swap in draw line algo, does interesting thing
                if (b.y > t.y)

                    draw_rect(game->mlx_info->mlx, game->mlx_info->win, t, b, game->surface, NULL);
                    //draw_line3(game->mlx_info->mlx, game->mlx_info->win, t, b, game->surface, NULL);
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

    tile = game->map_info->tile.y;
    ray_len = 20;
    endpoint.x = 20;
    endpoint.y = 20;
    
    init_ray_struct(&ray_info, game);
    cur_angle = player->angle - ray_info.FOV_half;
    while(ray_num < ray_info.rays_amount)
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
        ray_num += 1;
        ray_len = 0;
    }   
}

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
    t_ray_info ray_info;
    
    init_ray_struct(&ray_info, game);
    cur_angle = player->angle - ray_info.FOV_half;
    
    //printf("tiles of x %d and y %d \n", tile_x, tile_y);
    //printf("x_m %d y_m %d\n", x_m, y_m);
    while(ray_num < ray_info.rays_amount)
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
            //endpoint.x = buff.x;
            //endpoint.y = buff.y;
            buff.x++;
            offset = endpoint.y % tile_y;
            ray_len = dep_v;
        }
        else
        {
            offset = endpoint.x % tile_x;
            ray_len = dep_h;
        }
        ray_len *= cosf(player->angle - cur_angle);
        proj_h = 0;
        if (ray_len < 0.05)
            proj_h = game->map_info->resolution.y;
        else
            proj_h = (ray_info.proj_coef)/ ray_len;
        t.x = ray_num * ray_info.ray_scale;
        b.x = (ray_num * ray_info.ray_scale) + ray_info.ray_scale;

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
        //printf("SECOND endpoint x %f  and y %f\n", endpoint.x, endpoint.y);
        /*
        if (endpoint.x < 0)
            endpoint.x = 0;
        if (endpoint.y < 0)
            endpoint.y = 0;
        
        if (endpoint.x >= game->map_info->resolution.x)
            endpoint.x = (game->map_info->resolution.x) - 1;
        if (endpoint.y >= (game->map_info->resolution.y))
            endpoint.y = (game->map_info->resolution.y) - 1;
        */
        //printf("SECOND endpoint x %f  and y %f\n", endpoint.x, endpoint.y);
        //draw_line3(game->mlx_info->mlx, game->mlx_info->win, player->pos, endpoint, game->surface, NULL);
        
        cur_angle += ray_info.dt_a;
        ray_num += 1;
    }   
}

/*
void    dda2(t_all *game, t_player *player)
{
    float cur_angle;
    float ray_len = 0;
    int proj_h = 0;
    int ray_num = 0;
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
    t_ray_info ray_info;
    
    init_ray_struct(&ray_info, game);
    cur_angle = player->angle - ray_info.FOV_half;
    
    //printf("tiles of x %d and y %d \n", tile_x, tile_y);
    //printf("x_m %d y_m %d\n", x_m, y_m);
    while(ray_num < ray_info.rays_amount)
    {
        cos_a = round3(cosf(cur_angle));
        sin_a = round3(sinf(cur_angle));

        invert_cos = (1 / cos_a); // becaouse of round, wall detect doesnt work sometime
        invert_sin = (1 / sin_a);

        //printf("Inv_Cos is %f and Inv_sin is %f\n", invert_cos, invert_sin);
        //
        if (sin_a == 0)
            sin_a = 0.00001;
        if (cos_a == 0)
            cos_a = 0.00001;
        //
        if (cos_a >= 0)
        {
            endpoint.x = x_m + tile_x;
            dx = 1;
        } else
        {
            endpoint.x = x_m;
            dx = -1;
        }

        if (sin_a >= 0)
        {
            endpoint.y = y_m + tile_y;
            dy = 1;
        } else
        {
            endpoint.y = y_m;
            dy = -1;
        }

        
        if (dep_v < dep_h)
        {
            //endpoint.x = buff.x;
            //endpoint.y = buff.y;
            buff.x++;
            ray_len = dep_v;
        }
        else
        {
            ray_len = dep_h;
        }
        ray_len *= cosf(player->angle - cur_angle);
        proj_h = 0;
        if (ray_len < 0.05)
            proj_h = game->map_info->resolution.y;
        else
            proj_h = (ray_info.proj_coef)/ ray_len;
        t.x = ray_num * ray_info.ray_scale;
        b.x = (ray_num * ray_info.ray_scale) + ray_info.ray_scale;

        t.y = game->map_info->resolution.y / 2 - (proj_h / 2);
        if (t.y < 0)
            t.y = 0;
        b.y = game->map_info->resolution.y / 2 + (proj_h / 2);
        if (b.y > game->map_info->resolution.y)
            b.y = game->map_info->resolution.y;

        color = get_color_ftex(game->tex_info, (t.x / game->map_info->resolution.x) * 31, (t.y / (float)proj_h) * 31);
        color++;
        if (b.y > t.y)
            //draw_line3(game->mlx_info->mlx, game->mlx_info->win, t, b, game->surface, color);
            draw_rect(game->mlx_info->mlx, game->mlx_info->win, t, b, game->surface);
        //printf("SECOND endpoint x %f  and y %f\n", endpoint.x, endpoint.y);
        //
        if (endpoint.x < 0)
            endpoint.x = 0;
        if (endpoint.y < 0)
            endpoint.y = 0;
        
        if (endpoint.x >= game->map_info->resolution.x)
            endpoint.x = (game->map_info->resolution.x) - 1;
        if (endpoint.y >= (game->map_info->resolution.y))
            endpoint.y = (game->map_info->resolution.y) - 1;
        ///
        //printf("SECOND endpoint x %f  and y %f\n", endpoint.x, endpoint.y);
        //draw_line3(game->mlx_info->mlx, game->mlx_info->win, player->pos, endpoint, game->surface, NULL);
        
        cur_angle += ray_info.dt_a;
        ray_num += 1;
    }   
}
*/