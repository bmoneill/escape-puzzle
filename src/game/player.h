// player.h
#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    float x;
    float y;

    float move_speed;

    int width;
    int height;
} Player;

void player_init(Player* player);
void player_move(Player* player, float dx, float dy);

#endif