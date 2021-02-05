#include "cube.h"

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
    t_ray_info *ray_info;
    
    
    ray_info = game->ray_info;
    tile = game->map_info->tile.y;
    ray_len = 20;
    endpoint.x = 20;
    endpoint.y = 20;
    
    cur_angle = player->angle - ray_info->FOV_half;
    while(ray_num < ray_info->rays_amount)
    {
        cos_a = cosf(cur_angle);
        sin_a = sinf(cur_angle);
        while(ray_len < ray_info->max_depth){
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
        cur_angle += ray_info->dt_a;
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

char    ray_hit_check(t_all *game, t_vector endpoint, int dx, int dy, int tile_x, int tile_y, char *is_sprite, t_vector *sprite_pos)
{
    if (endpoint.x < 0 || endpoint.y < 0 || (endpoint.x + dx) /tile_x >= game->map_info->width || (endpoint.y + dy) /tile_y >= game->map_info->height 
        || game->map_info->full_map[(int)(endpoint.y + dy)/tile_y][(int)((endpoint.x + dx)/tile_x)] == '1')
    {
        //*is_sprite = '0';
        return (1);
    }
    if (endpoint.x < 0 || endpoint.y < 0 || (endpoint.x + dx) /tile_x >= game->map_info->width || (endpoint.y + dy) /tile_y >= game->map_info->height 
        || game->map_info->full_map[(int)(endpoint.y + dy)/tile_y][(int)((endpoint.x + dx)/tile_x)] == '2')
    {
        //if (game->sprite_rays > 0)
        game->sprite_rays++;
        sprite_pos->x = (int)(endpoint.x + dx)/tile_x;
        sprite_pos->y = (int)(endpoint.y + dy)/tile_y;
        *is_sprite = '2';
    }
    return (0);
}


// cool unit cicle
//http://live.mephist.ru/show/unit-circle/
void    dda(t_all *game, t_player *player, t_vert_line *lines)
{
    float cur_angle;
    float ray_len = 0;
    int proj_h = 0;
    int ray_num = 0;
    int offset = 0;
    char *color;
    char is_sprite = '0';

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

    float coef;
    int spite_ray_amount = 0;

    t_vector t;
    t_vector b;
    t_vector buff; //for fisrt endpoint.x and .y
    t_vector endpoint;
    t_vector sprite_pos;
    sprite_pos.x = -1;
    sprite_pos.y = -1;
    cur_angle = player->angle - game->ray_info->FOV_half;
    game->sprite_rays = 0;


    //printf("tiles of x %d and y %d \n", tile_x, tile_y);
    //printf("x_m %d y_m %d\n", x_m, y_m);
    while(ray_num < game->ray_info->rays_amount)
    {
        is_sprite = '0';
        cos_a = round3(cosf(cur_angle));
        sin_a = round3(sinf(cur_angle));
        lines[ray_num].len_to_sprite = 10000;

        invert_cos = (1 / cos_a); // becaouse of round, wall detect doesnt work sometime
        invert_sin = (1 / sin_a);

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
            if (ray_hit_check(game, endpoint, dx, 0, tile_x, tile_y, &is_sprite, &sprite_pos))
                break;
            if (is_sprite == '2' && lines[ray_num].len_to_sprite == 10000)
            {
                lines[ray_num].len_to_sprite = dep_v;
                is_sprite = '0';
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

        for (int j = 0; j < game->map_info->resolution.y; j += tile_y)
        {
            dep_h = (endpoint.y - player->pos.y) * invert_sin;
            //printf("dep_h is %f\n", dep_h);
            endpoint.x = player->pos.x + (dep_h * cos_a);
            if (ray_hit_check(game, endpoint, 0, dy, tile_x, tile_y, &is_sprite, &sprite_pos))
                break;
            if (is_sprite == '2')
            {
                if (dep_h < lines[ray_num].len_to_sprite )
                    lines[ray_num].len_to_sprite = dep_h;
                is_sprite = '0';
            }
            endpoint.y += dy * tile_y;
        }
        

        coef = 0;
        coef++;
        if (dep_v < dep_h)
        {
            //endpoint.x = buff.x;
            endpoint.y = buff.y;

            buff.y++;
            game->vert_texture = 1;
            offset = (int)endpoint.y % tile_y;
            ray_len = dep_v;
        }
        else
        {
            game->vert_texture = 0;
            offset = (int)endpoint.x % tile_x;
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

        color = get_color_ftex(game->tex_info, 10, 10);
        color++;

        //printf("SECOND endpoint x %d  and y %d\n", endpoint.x, endpoint.y);
        //will work only if vectors are floats
        lines[ray_num].ray_len = ray_len;
        lines[ray_num].offset = offset;
        lines[ray_num].t = t;
        lines[ray_num].b = b;
        lines[ray_num].proj_h = proj_h;
        lines[ray_num].vert_text = game->vert_texture;
        lines[ray_num].is_sprite = is_sprite;
        lines[ray_num].sprite_dis = -1;
        lines[ray_num].sprite_pos = sprite_pos;
        
        /*
        if (is_sprite == '2')
            draw_line3(game->mlx_info->mlx, game->mlx_info->win, player->pos, endpoint, game->surface, color);
        else
            draw_line3(game->mlx_info->mlx, game->mlx_info->win, player->pos, endpoint, game->surface, NULL);
        */

        //draw_tex_rect(game, t, b, game->surface, offset, proj_h);
        cur_angle += game->ray_info->dt_a;
        spite_ray_amount++;
        ray_num += 1;
    }
}