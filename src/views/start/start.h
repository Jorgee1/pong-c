#include <SDL.h>

#include "utils.h"
#include "controller.h"
#include "charset.h"

#include "views/index.h"

#ifndef START_H
#define START_H

typedef struct
{
	char* title;
	char* message;

	int blink_flag;
	int blink_frame_count;
	int blink_max_frame;
} StartLayout;

void start_action_logic(Input*, int*);

void start_collision_detection();

void start_update_world();

void start_render(Screen*, StartLayout*, Letter[]);

#endif