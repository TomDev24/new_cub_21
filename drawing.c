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

void draw_rect(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img)
{
    int x = 0;
    int y = 0;
    unsigned int color;
    char *addr;
    
    color = mlx_get_color_value(mlx, 0xABCDEF);
    x = vec1.x;
    y = vec1.y;
    while (x < vec2.x)
    {
        while (y < vec2.y)
        {
            addr = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8)); 
            *(unsigned int*)addr = color;
            y++;
        }
        y = vec1.y;
        x++;
    }
    mlx_put_image_to_window(mlx, win, img->img, 0, 0);
}

void fill_black(void *mlx, void *win, t_img *img)
{
    int x = 0;
    int y = 0;
    char *addr;
    unsigned int color;

    color = mlx_get_color_value(mlx, 0x000000);
    while (y < 600)
    {
        while (x < 600)
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

    void *img;
    t_img img_info;
    char *addr;
    char *start_addr;

    img = mlx_new_image(mlx, map->width * 20, map->height * 20);
    start_addr = mlx_get_data_addr(img, &img_info.bits_per_pixel, &img_info.line_length, &img_info.endian);

    while(y < map->height * 20)
    {
        //we could go beyond actual length of line, so we need second check
        while (x < map->width * 20)
        {
            addr = start_addr + (y * img_info.line_length + x * (img_info.bits_per_pixel / 8));
            *(unsigned int*)addr = 0xFFFFFF;
            //we could go beyond actual length of line, so we need second check
            if (map->full_map[y / 20][x / 20] == '1' && map->full_map[y / 20][x / 20] != '\0')
            {
                *(unsigned int*)addr = 0xFF0000;
                printf("1");
            }
            else if (map->full_map[y / 20][x / 20] == 'N')
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

void draw_line(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img)
{
    t_vector dir;
    char *addr;
    unsigned int color;

    int x = 0;
    int y = 0;
    
    //t_vector line_segment;
    //line_segment.x = vec1.x;
    //line_segment.y = vec1.y;

    dir = sub_vector(vec2, vec1);
    normalize(&dir);
    //printf("\ndir x:%f y:%f\n",dir.x, dir.y);

    //we dedacating to much memory to draw the line,
    //it could be not 600 600, but sides of rectangle of the line
    //drawing algorithm is slow too
    color = mlx_get_color_value(mlx, 0xABCDEF);
    if (vec1.x < vec2.x)
        x = vec1.x;
    else
    {
        x = vec2.x;
        y = vec2.y;
        vec2.x = vec1.x;
        vec2.y = vec1.y;
        vec1.x = x;
        vec1.y = y;
    }
    while (x <= vec2.x)
    {
        y = (int)floor(vec1.y + dir.y * (x - vec1.x)/dir.x);
        //printf("Y %d\n",y);
        if (y < 0)
            y = -y;
        if (y == 0)
            y = 1;
        //printf("Y %d\n",y);
        addr = img->addr + (int)floor((y * img->line_length + x * (img->bits_per_pixel / 8)));
        *(unsigned int*)addr = color;
        x++;
    }


    mlx_put_image_to_window(mlx, win, img->img, 0, 0);
}

void draw_line2(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img)
{
    //Bresenham’s Line Drawing Algorithm
    //https://www.thecrazyprogrammer.com/2017/01/bresenhams-line-drawing-algorithm-c-c.html
    // y = mx + c -- line function
    //slope m is equals dy/dx
 //   |          |
 //   | d upper  .
 //   |    .     |
 //   ,          |
 //   | d lower  |

    int dx, dy, p, x, y;
    //t_vector dir;
    char *addr;
    unsigned int color;

    //vec2.x = abs((int)vec2.x);
    //vec2.y = abs((int)vec2.y);
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

    dx = vec2.x - x;
    dy = vec2.y - y;

    p = 2 * (dy - dx);
    while (x <= vec2.x)
    {
        if (p>= 0)
        {
            addr = img->addr + ((y * img->line_length + x * (img->bits_per_pixel / 8)));
            *(unsigned int*)addr = color;
            y++;
            p = p + 2*dy - 2*dx;
        }
        else
        {
           addr = img->addr + ((y * img->line_length + x * (img->bits_per_pixel / 8)));
           *(unsigned int*)addr = color;
           p = p + 2*dy;
        }
        printf("Y is %d\n", y);
        x++;
    }
    mlx_put_image_to_window(mlx, win, img->img, 0, 0);
}

void draw_line3(void *mlx, void *win, t_vector vec1, t_vector vec2, t_img *img)
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
    if (abs(dx) > abs(dy))
        step = abs(dx);
    else
        step = abs(dy);
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
        addr = img->addr + (((int)round(vec1.y) * img->line_length + (int)round(vec1.x) * (img->bits_per_pixel / 8)));
        *(unsigned int*)addr = color;
        
        vec1.y += y;
        vec1.x += x;
        //printf("X is %f, y is %f\n", vec1.x, vec1.y);
    }
    
    //if (dy == 0)
        //slope = 0;
    

    mlx_put_image_to_window(mlx, win, img->img, 0, 0);
}