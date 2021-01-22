#include <SDL.h>

#include "utils.h"
#include "controller.h"
#include "charset.h"

#include "views/index.h"

#ifndef PONG_H
#define PONG_H

typedef struct
{
	SDL_Rect box;
	SDL_Color color;
	int score;
	int speed;
	int max_speed;
} Player;

typedef struct
{
	SDL_Rect box;
	SDL_Color color;
	int speed_x;
	int speed_y;
	int max_speed;
} Ball;


typedef struct
{
	int score_padding;
	int players_padding;
	SDL_Point line[2];

	SDL_Point player_start_position[2];
	SDL_Point ball_start_position;
} PongLayout;

void reset_players(Player[2], PongLayout*);
void reset_ball_speed(Ball*, PongLayout*);

void game_action_logic
(
	Input*,
	Player[],
	Ball*,
	int*
);

void game_collision_detection
(
	Player[],
	int,
	Ball*,
	Screen*,
	PongLayout*
);

void game_update_world
(
	Player[],
	int,
	Ball*,
	Screen*
);

void game_render
(
	Screen*,
	PongLayout*,
	Player[],
	int,
	Ball*,
	Letter[]
);


#endif