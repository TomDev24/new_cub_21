#include "cube.h"

void    display_tex(void *mlx, void *win, char *file)
{
    void *tex;
    int w = 32;
    int h = 32;

    tex = mlx_xpm_file_to_image(mlx, file, &w, &h);

    mlx_put_image_to_window(mlx, win, tex, 0, 0);
}

char *get_color_ftex(t_img *tex_info, int x, int y)
{
    int w = 32; //tex size
    int h = 32;
    char *addr;
    char *start_addr;

    addr = NULL;
    //printf("The x and y of texture %d %d \n", x, y);
    start_addr = mlx_get_data_addr(tex_info->img, &tex_info->bits_per_pixel, &tex_info->line_length, &tex_info->endian);
    if (x < w && y < h)
    {
        //if (x == 30)
        //printf("X is %d and y is %d\n", x, y);
        addr = start_addr + (y * tex_info->line_length + x * (tex_info->bits_per_pixel / 8));
    }
    return (addr);
}

void    get_tex(void *mlx, void *win, char *file, t_img *tex_info)
{
    void *tex;
    int w = 32; //tex size
    int h = 32;
    int x = 0;
    int y = 0;

    char *addr;
    char *start_addr;

    tex = mlx_xpm_file_to_image(mlx, file, &w, &h);
    tex_info->img = tex;
    start_addr = mlx_get_data_addr(tex, &tex_info->bits_per_pixel, &tex_info->line_length, &tex_info->endian);
    printf("line len %d\n", tex_info->line_length);
    while (x < w)
    {
        while (y < h)
        {
            addr = start_addr + (y * tex_info->line_length + x * (tex_info->bits_per_pixel / 8));
            mlx_pixel_put(mlx, win, x, y, mlx_get_color_value(mlx, (unsigned int)*addr));
            //printf("color is %d\n", (unsigned int)*addr);
            y++;
        }
        y = 0;
        x++;
    }
    tex++;
}