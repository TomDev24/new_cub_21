#include "cube.h"

t_vector add_vector(t_vector vec1, t_vector vec2)
{
    t_vector res;

    res.x = vec1.x + vec2.x;
    res.y = vec1.y + vec2.y;
    return (res);
}

t_vector sub_vector(t_vector vec1, t_vector vec2)
{
    t_vector res;

    res.x = vec1.x - vec2.x;
    res.y = vec1.y - vec2.y;
    return (res);
}

void    normalize(t_vector *vec)
{
    float len;

    //printf("\n ---GO inVector x: %f, y: %f", vec->x, vec->y);
    len = vec->x * vec->x + vec->y *vec->y;
    len = sqrt(len);
    //printf(">>%f<<", len);
    vec->x = vec->x / len;
    vec->y = vec->y / len;
    /*
    if (vec->x > 0)
        vec->x = 1;
    else if (vec->x == 0)
        vec->x = 0;
    else
        vec->x = -1;

    if (vec->y > 0)
        vec->y = 1;
    else if (vec->y == 0)
        vec->y = 0;
    else
        vec->y = -1;
    */
}

t_img create_surface(void *mlx, int width, int height)
{
    t_img img;

    img.img = mlx_new_image(mlx, width, height);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);

    return (img);
}

char *aplly_tex(t_all *game, int offset, float y) // offset is x, and it will be incremented
{
    float tex_x_scale = game->tex_info->w_h / (float)game->map_info->tile.x; // 32 is texture witdth and heigth
    //int tex_y_scale = 32 / game->map_info->tile.y; // texture always are rects
    char *res;
    int tex_y;

    if (game->vert_texture == 1)
           tex_x_scale = game->tex_info->w_h / (float)game->map_info->tile.y;
    
    tex_y = y * game->tex_info->w_h;
    //printf("Offst is %d\n", offset);
    //printf("Tex_x_scale is is %d\n", tex_x_scale);
    //printf("x is %d\n", offset * tex_x_scale);
    res = get_color_ftex(game->tex_info, offset * tex_x_scale, tex_y);

    return (res);
}

char *aplly_sprite(t_all *game, int offset, float y) // offset is x, and it will be incremented
{
    float tex_x_scale = game->sprite_info->w_h / (float)game->map_info->tile.x; // 32 is texture witdth and heigth
    //int tex_y_scale = 32 / game->map_info->tile.y; // texture always are rects
    char *res;
    int tex_y;

    if (game->vert_texture == 1)
           tex_x_scale = game->sprite_info->w_h / (float)game->map_info->tile.y;
    
    tex_y = y * game->sprite_info->w_h;
    //printf("Offst is %d\n", offset);
    //printf("Tex_x_scale is is %d\n", tex_x_scale);
    //printf("x is %d\n", offset * tex_x_scale);
    res = get_color_ftex(game->sprite_info, offset * tex_x_scale, tex_y);

    return (res);
}

void draw_tex_rect(t_all *game, t_vector vec1, t_vector vec2, t_img *img, int offset, int proj_h, char is_sprite)
{
    int x = 0;
    int y = 0;
    char *addr;
    int color;

    //float tex_x_scale = game->sprite_info->w_h / (float)game->map_info->tile.x;
    
    if (game->first_sprite_ray != NULL)
    {
        vec1.y = game->first_sprite_ray->t.y;
        vec2.y = game->first_sprite_ray->b.y;
    }
    x = vec1.x;
    y = vec1.y;
    while (x < vec2.x)
    {
        while (y < vec2.y)
        {
            addr = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
            if (is_sprite == '2')
            {
                color = create_trgb(0,0,255,0);
                color++;
                /*
                get_t(*(int*)aplly_sprite(game, offset, ((y - vec1.y)/ (float)proj_h)));
                get_r(*(int*)aplly_sprite(game, offset, ((y - vec1.y)/ (float)proj_h)));
                get_g(*(int*)aplly_sprite(game, offset, ((y - vec1.y)/ (float)proj_h)));
                get_b(*(int*)aplly_sprite(game, offset, ((y - vec1.y)/ (float)proj_h)));
                */
                *(unsigned int*)addr = *(unsigned int*)aplly_sprite(game, offset, ((y - vec1.y)/ (float)game->first_sprite_ray->proj_h));
            }
            else
                *(unsigned int*)addr = *(unsigned int*)aplly_tex(game, offset, ((y - vec1.y)/ (float)proj_h));
            y++;
        }
        y = vec1.y;
        x++;
    }
    if (is_sprite == '2')
    {
        //game->first_sprite_ray->offset += 0.1;
        //game->first_sprite_ray->offset = game->first_sprite_ray->offset % 64;
    }
}

void draw_rect(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img, char *col)
{
    int x = 0;
    int y = 0;
    unsigned int color;
    char *addr;
    
    color = mlx_get_color_value(mlx, 0xFF00EE);
    x = vec1.x;
    y = vec1.y;
    while (x <= vec2.x)
    {
        while (y < vec2.y)
        {
            addr = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8)); 
            if (col != NULL)
                *(unsigned int*)addr = *(unsigned int*)col;
            else
                *(unsigned int*)addr = color;
            y++;
        }
        y = vec1.y;
        x++;
    }
    mlx_put_image_to_window(mlx, win, img->img, 0, 0);
}

void fill_black(void *mlx, void *win, t_img *img, t_map *map)
{
    int x = 0;
    int y = 0;
    char *addr;
    unsigned int color;

    color = mlx_get_color_value(mlx, 0x000000);
    while (y < map->resolution.y)
    {
        while (x < map->resolution.x)
        {
                addr = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
                *(unsigned int*)addr = color;
                x++;
        }
        x = 0;
        y++;
    }
    mlx_put_image_to_window(mlx, win, img->img, 0, 0);
}

void draw_map(void *mlx, void *win, t_map *map)
{
    int x = 0;
    int y = 0;
    int scaler = 20;

    void *img;
    t_img img_info;
    char *addr;
    char *start_addr;

    img = mlx_new_image(mlx, map->width * scaler, map->height * scaler);
    start_addr = mlx_get_data_addr(img, &img_info.bits_per_pixel, &img_info.line_length, &img_info.endian);

    while(y < map->height * scaler)
    {
        //we could go beyond actual length of line, so we need second check
        while (x < map->width * scaler)
        {
            addr = start_addr + (y * img_info.line_length + x * (img_info.bits_per_pixel / 8));
            *(unsigned int*)addr = 0xFFFFFF;
            //we could go beyond actual length of line, so we need second check
            if (map->full_map[y / scaler][x / scaler] == '1' && map->full_map[y / scaler][x / scaler] != '\0')
            {
                *(unsigned int*)addr = 0xFF0000;
                printf("1");
            }
            else if (map->full_map[y / scaler][x / scaler] == 'N')
            {
                *(unsigned int*)addr = 0x001000;
                printf("0");
            }
            else
            {
                printf("0");
                addr++;
            }
            
            x++;
        }
        printf("\n");
        x = 0;
        y++;
    }
    mlx_put_image_to_window(mlx, win, img, 0, 0);
}

void draw_line3(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img, char *col)
{
    float x;
    float y;

    int dx;
    int dy;
    int step;
    //float slope;

    char *addr;
    unsigned int color;

    color = mlx_get_color_value(mlx, 0xABCDEF);
    if (vec1.x <= vec2.x)
    {
        x = vec1.x;
        y = vec1.y;
    }
    else
    {
        x = vec2.x;
        y = vec2.y;
        vec2.x = vec1.x;
        vec2.y = vec1.y;
        vec1.x = x;
        vec1.y = y;
    }

    //dx and dy could be zero
    dx = vec2.x - vec1.x;
    dy = vec2.y - vec1.y;

    //draw line is corrupted
    //because i do not use abs func anymore
    if (dx > dy)
        step = dx;
    else
        step = dy;
    //slope = dy / dx; // casting to float
    x = dx / (float)step;
    y = dy / (float)step;
    if (x > 1)
        x = 1;
    if (y > 1)
        y = 1;
    //printf("X is %f, y is %f\n", x, y);
    while (step--)
    {
        addr = img->addr + (((int)(vec1.y) * img->line_length + (int)(vec1.x) * (img->bits_per_pixel / 8)));
        
        if (col != NULL)
            *(unsigned int*)addr = *(unsigned int*)col;
        else
            *(unsigned int*)addr = color;
        
        
        vec1.y += y;
        vec1.x += x;
        //printf("X is %f, y is %f\n", vec1.x, vec1.y);
    }
    
    //if (dy == 0)
        //slope = 0;
    
    col++;
    mlx_put_image_to_window(mlx, win, img->img, 0, 0);
}

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
                draw_rect(game->mlx_info->mlx, game->mlx_info->win, vec1, len, game->surface, NULL);
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