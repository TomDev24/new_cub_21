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

 
// cool unit cicle
//http://live.mephist.ru/show/unit-circle/
void    dda(t_all *game, t_player *player)
{
    float cur_angle;
    float ray_len = 0;
    int proj_h = 0;
    int ray_num = 0;
    char *color;

    float cos_a;
    float sin_a;

    int tile_x = (game->map_info->resolution.x / game->map_info->width);
    int tile_y = (game->map_info->resolution.y / game->map_info->height);
    int x_m = (int)(player->pos.x / tile_x) * tile_x; // (int) its a great fix ???!!!!
    int y_m = (int)(player->pos.y / tile_y) * tile_y;
    int dx;
    int dy;
    float dep_v;
    float dep_h;

    t_vector t;
    t_vector b;
    t_vector buff; //for fisrt endpoint.x and .y
    t_vector endpoint;
    t_ray_info ray_info;

    ray_len = 0;
    endpoint.x = 0;
    endpoint.y = 0;
    
    init_ray_struct(&ray_info, game);
    cur_angle = player->angle - ray_info.FOV_half;
    
    //printf("tiles of x %d and y %d \n", tile_x, tile_y);
    //printf("x_m %d y_m %d\n", x_m, y_m);
    while(ray_num < game->map_info->resolution.x)
    {
        cos_a = cosf(cur_angle);
        sin_a = sinf(cur_angle);

        //printf("Cos is %f and sin is %f\n", cos_a, sin_a);
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
        for (int i = 0; i < game->map_info->width * tile_x; i += tile_x)
        {
            dep_v = (endpoint.x - player->pos.x) / cos_a;
            //printf("dep_v is %f\n", dep_v);
            endpoint.y = player->pos.y + (dep_v * sin_a);
            //if (endpoint.y > game->map_info->resolution.y)
                //break;
            //printf("FIRST endpoint x %f  and y %f\n", endpoint.x, endpoint.y);
            //printf("FIRST endpoint x %d  and y %d\n", ((int)(endpoint.x + dx)/tile_x), (int)(endpoint.y / tile_y) );
            if (endpoint.x < 0 || endpoint.y < 0 || (endpoint.x + dx) /tile_x >= game->map_info->width || endpoint.y /tile_y >= game->map_info->height 
                || game->map_info->full_map[(int)(endpoint.y/tile_y)][(int)((endpoint.x + dx)/tile_x)] == '1')
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
        for (int j = 0; j < game->map_info->height * tile_y; j += tile_y)
        {
            //printf("sinus %f\n", sin_a);
            dep_h = (endpoint.y - player->pos.y) / sin_a;
            //printf("dep_h is %f\n", dep_h);
            endpoint.x = player->pos.x + (dep_h * cos_a);
            //if (endpoint.x > game->map_info->resolution.x)
                //break;
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
            endpoint.x = buff.x;
            endpoint.y = buff.y;
            ray_len = dep_v;
        }
        else
        {
            ray_len = dep_h;
        }
        ray_len *= cosf(player->angle - cur_angle);
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

        color = get_color_ftex(game->tex_info, (t.x / game->map_info->resolution.x) * 31, (t.y / (float)proj_h) * 31);
        //color++;
        if (b.y > t.y)
            draw_line3(game->mlx_info->mlx, game->mlx_info->win, t, b, game->surface, color);
        //printf("SECOND endpoint x %f  and y %f\n", endpoint.x, endpoint.y);
        if (endpoint.x < 0)
            endpoint.x = 0;
        if (endpoint.y < 0)
            endpoint.y = 0;
        
        if (endpoint.x >= game->map_info->resolution.x)
            endpoint.x = (game->map_info->resolution.x) - 1;
        if (endpoint.y >= (game->map_info->resolution.y))
            endpoint.y = (game->map_info->resolution.y) - 1;
        //printf("SECOND endpoint x %f  and y %f\n", endpoint.x, endpoint.y);
        //draw_line3(game->mlx_info->mlx, game->mlx_info->win, player->pos, endpoint, game->surface, NULL);
        
        cur_angle += ray_info.dt_a;
        ray_num += 1;
    }   
}

/*
void    dda2(t_all *game, t_player *player)
{

} */

void    draw_grid(t_all *game)
{
    t_vector vec1;
    t_vector vec2;
    t_vector len;

    t_vector tile;
    tile.x = (game->map_info->resolution.x / game->map_info->width);
    tile.y = (game->map_info->resolution.y / game->map_info->height);

    vec1.y = 0;
    vec2.y = 0;

    vec1.x = 0;
    vec2.x = 0;

    for (int i = 0; i < game->map_info->width * tile.x; i += tile.x)
    {
        for (int j = 0; j < game->map_info->height * tile.y; j += tile.y)
        {
            if (game->map_info->full_map[(int)(j / (int)tile.y)][(int)(i/ (int)tile.x)] == '1')
            {
                len.x = i + tile.x;
                len.y = j + tile.y;
                vec1.x = i;
                vec1.y = j;
                draw_rect(game->mlx_info->mlx, game->mlx_info->win, vec1, len, game->surface);
            }
        }
    }

    vec1.y = 0;
    vec2.y = 0;

    vec1.x = 0;
    vec2.x = game->map_info->resolution.x;
    
    for (int j = 0; j < game->map_info->height * tile.y; j += tile.y)
    {
        draw_line3(game->mlx_info->mlx, game->mlx_info->win, vec1, vec2, game->surface, NULL);
        vec1.y +=tile.y;
        vec2.y += tile.y;
    }

    vec1.y = 0;
    vec2.y = game->map_info->resolution.y;

    vec1.x = 0;
    vec2.x = 0;
    
    for (int j = 0; j < game->map_info->width * tile.x; j += tile.x)
    {
        draw_line3(game->mlx_info->mlx, game->mlx_info->win, vec1, vec2, game->surface, NULL);
        vec1.x +=tile.x;
        vec2.x += tile.x;
    }
}