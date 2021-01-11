#include <stdio.h>
#include <math.h>
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
    //formula prividenia
    if (keycode == 100)
    {
        player->pos.x += -sinf(player->angle) * player->speed;
        player->pos.y += cosf(player->angle) * player->speed;
    }
    if (keycode == 97)
    {
        player->pos.x += sinf(player->angle) * player->speed;
        player->pos.y += -cosf(player->angle) * player->speed;
    }
    if (keycode == 119)
    {
        player->pos.x += cosf(player->angle) * player->speed;
        player->pos.y += sinf(player->angle) * player->speed;
    }
    if (keycode == 115)
    {
        player->pos.x += -cosf(player->angle) * player->speed;
        player->pos.y += -sinf(player->angle) * player->speed;
    }
    if (keycode == 65361) //left
    {
        player->angle -= 0.03;
        //if (player->angle < 0) there were segfaults at angle = 0
    }
    if (keycode == 65363) // right
        player->angle += 0.03;
    return (0);
}