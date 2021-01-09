#include <stdio.h>
#include <mlx.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "cube.h"
#include "./libft/libft.h"

int     key_hook(int keycode, void *nall)
{
    keycode++;
    keycode--;
    nall++;
    nall--;
    printf("Key press %d\n", keycode);
    return (0);
}

int     movement(int keycode, t_player *player)
{
    if (keycode == 100)
    {
        player->pos.x += player->speed;
        printf("X coord %f\n", player->pos.x);
    }
    if (keycode == 97)
        player->pos.x -= player->speed;
    if (keycode == 119)
        player->pos.y -= player->speed;
    if (keycode == 115)
        player->pos.y += player->speed;
    return (0);
}