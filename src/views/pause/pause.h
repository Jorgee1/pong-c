#include <SDL.h>

#include "utils.h"
#include "controller.h"
#include "charset.h"

#include "views/index.h"

#ifndef PAUSE_H
#define PAUSE_H

enum PAUSE_OPTIONS
{
	PAUSE_RESUME,
	PAUSE_EXIT,
	PAUSE_TOTAL
};

typedef struct
{
	char* title;
	char* options[2];
	int index;
	int spacing;
} PauseLayout;

void pause_action_logic(
	Input*,
	PauseLayout*,
	Screen*
);

void pause_collision_detection();

void pause_update_world();

void pause_render
(
	Screen*,
	PauseLayout*,
	Letter[],
	Letter[]
);

#endif

