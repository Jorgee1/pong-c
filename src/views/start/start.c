#include "start.h"


void start_action_logic(Input* input, int* view_current)
{
	if (input->start.action_state)
	{
		*view_current = VIEW_GAME;
	}
}

void start_collision_detection() {}

void start_update_world(StartLayout* layout)
{
	layout->blink_frame_count += 1;

	if (layout->blink_frame_count > layout->blink_max_frame)
	{
		layout->blink_frame_count = 0;

		if (layout->blink_flag)
		{
			layout->blink_flag = 0;
		}
		else
		{
			layout->blink_flag =  1;
		}
	}
}

void start_render(StartLayout* layout)
{
    SDL_Renderer* renderer = layout->screen->renderer;
    SDL_Rect rect = layout->screen->rect;
    
	int x = rect.w/2;
	int y = rect.h/4;

    char* title = layout->title;

    int title_width = get_string_w(layout->letters, title);

	render_string(
		renderer,
		x - title_width/2,
		y,
		layout->letters,
		title
	);


	if (layout->blink_flag)
	{
        y = 2*rect.h/3;
		render_string(
			renderer,
			x - get_string_w(layout->letters, layout->message)/2,
			y,
			layout->letters,
			layout->message
		);
	}
}