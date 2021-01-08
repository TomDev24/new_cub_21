#include <stdio.h>
#include <mlx.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "cube.h"
#include "./libft/libft.h"
#include "./gnl/get_next_line.h"


typedef struct musor
{
    void *mlx;
    void *win;
}          t_musor;

int rend(t_musor *m)
{
    t_vector v1;
    t_vector v2;

    v1.x = 20; v1.y = 20;
    v2.x = 400; v2.y = 400;
    
    draw_line(m->mlx, m->win, v1, v2);
    v1.x = 250; v1.y = 500;
    v2.x = 400; v2.y = 10;
    draw_line(m->mlx, m->win, v1, v2);
    return (1);
}

int main(int argc, char **argv)
{
    t_map map_info;
    void *mlx;
    void *win;
    int map[M_WIDTH][M_HEIGTH];
    t_vector v1;
    t_vector v2;
    v1.x = 20; v1.y = 20;
    v2.x = 400; v2.y = 400;
    
    if (argc > 1)
    {
        //open_print(argv[1]);
        if (parse_map(argv[1], &map_info) == -1)
            return(-1);
    }
    fill_map(map);
    mlx = mlx_init();
    win = mlx_new_window(mlx, 600, 600, "Yes");
    
    //map_info.map = gen_map(3, 3); now wont work
    //print_map2(map_info.map);

    //draw_line(mlx, win, v1, v2);
    v1.x = 50; v1.y = 50;
    v2.x = 100; v2.y = 50;
    draw_line(mlx, win, v1, v2);
    
    //mlx_loop_hook(mlx, rend, &musor);
    mlx_loop(mlx);
    return (0);
}