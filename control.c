#include "cube.h"

int     key_hook(int keycode, void *nall)
{
    printf("Key press %d\n, just ignore this %p", keycode, nall);
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
        player->angle -= 0.04;
        //player_rot speed didnt help
        //if (player->angle < 0) there were segfaults at angle = 0
    }
    if (keycode == 65363) // right
        player->angle += 0.04;
    
    player->angle = fmod(player->angle, D_PI); //remainder
    return (0);
}