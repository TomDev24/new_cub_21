#include <stdio.h>
#include <math.h>
#include <mlx.h>
#include "cube.h"

typedef struct img
{
    void    *img;
    char    *addr;
    int     bits_per_pixel;
    int     line_length;
    int     endian;
}              t_img;


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
    int len;

    printf("\n ---GO inVector x: %d, y: %d", vec->x, vec->y);
    len = vec->x * vec->x + vec->y *vec->y;
    len = sqrt(len);
    printf(">>%d<<", len);
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
}


void draw_line(void *mlx, void *win, t_vector vec1, t_vector vec2)
{
    t_img img;
    t_vector dir;
    char *addr;
    unsigned int color;

    int x = 0;
    int y = 0;
    t_vector line_segment;
    line_segment.x = vec1.x;
    line_segment.y = vec1.y;

    dir = sub_vector(vec2, vec1);
    normalize(&dir);
    printf("\ndir x:%d y:%d\n",dir.x, dir.y);

    //we dedacating to much memory to draw the line,
    //it could be not 600 600, but sides of rectangle of the line
    //drawing algorithm is slow too
    img.img = mlx_new_image(mlx, 600, 600);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
    color = mlx_get_color_value(mlx, 0xABCDEF);
    while (y < 600)
    {
        while (x < 600)
        {
            //printf("\n Vector x: %d, y: %d", line_segment.x, line_segment.y);
            //printf("Vector of line seg x: %d, y: %d\n", line_segment.x, line_segment.y);
            //printf("Loop x: %d, y: %d\n", x, y);
            if ( (line_segment.x == x && line_segment.y == y) )
            {
                addr = img.addr + (y * img.line_length + x * (img.bits_per_pixel / 8));
                *(unsigned int*)addr = color;
                line_segment = add_vector(line_segment, dir);
                printf("Vector of line seg x: %d, y: %d\n", line_segment.x, line_segment.y);
            }
            if (line_segment.x == vec2.x && line_segment.y == vec2.y)
            {
                break;
            }
            x++;
        }
        x = 0;
        y++;
    }

    mlx_put_image_to_window(mlx, win, img.img, 0, 0);
}