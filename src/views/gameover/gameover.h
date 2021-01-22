#include <SDL.h>

#include "utils.h"
#include "controller.h"
#include "charset.h"

#include "views/index.h"
#include "views/pong/pong.h"

#ifndef GAMEOVER_H
#define GAMEOVER_H

enum GAMEOVER_OPTIONS
{
	GAMEOVER_EXIT_YES,
	GAMEOVER_EXIT_NO,
	GAMEOVER_TOTAL
};

typedef struct
{
	char* title;
	char* question;
	char* options[2];
	int index;
	int spacing;
} GameOverLayout;

void gameover_action_logic(
	Input*,
	GameOverLayout*,
	Screen*,
	Ball*,
	Player[2],
	PongLayout*
);

void gameover_collision_detection();

void gameover_update_world();

void gameover_render
(
	Screen*,
	GameOverLayout*,
	Letter[],
	Letter[]
);


#endif