#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "utils.h"
#include "controller.h"
#include "charset.h"

#include "views/index.h"
#include "views/start/start.h"
#include "views/pong/pong.h"
#include "views/pause/pause.h"
#include "views/gameover/gameover.h"


int main(int argc, char* argv[])
{
    time_t rand_seed;
    srand((unsigned) time(&rand_seed));

    ///////////////////////////////////////////////////////////////////////////////////////
    // COLOR //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////

    Colors colors;
    colors.red   = (SDL_Color) {0xFF, 0x00, 0x00, 0xFF};
    colors.green = (SDL_Color) {0x00, 0xFF, 0x00, 0xFF};
    colors.blue  = (SDL_Color) {0x00, 0x00, 0xFF, 0xFF};
    colors.white = (SDL_Color) {0xFF, 0xFF, 0xFF, 0xFF};
    colors.black = (SDL_Color) {0x00, 0x00, 0x00, 0xFF};

    ///////////////////////////////////////////////////////////////////////////////////////
    // SDL INIT ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////

    Screen screen;
    screen.name = "Pong";
    screen.exit = SDL_GAME_RUN;
    screen.view_index = VIEW_START;
    screen.clear_color = colors.black;

    screen.rect.x = 0;
    screen.rect.y = 0;
    screen.rect.w = 640;
    screen.rect.h = 480;

    screen.window = NULL;
    screen.renderer = NULL;


    if (SDL_Init(SDL_INIT_VIDEO)) return 1;
    screen.window = SDL_CreateWindow(
        screen.name,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screen.rect.w,
        screen.rect.h,
        SDL_WINDOW_HIDDEN
    );
    if (screen.window == NULL) return 1;

    screen.renderer = SDL_CreateRenderer(
        screen.window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (screen.renderer == NULL) return 1;

    /*
     * Window blinks for a second if its shown before creating the renderer at start
     */

    SDL_ShowWindow(screen.window);

    ///////////////////////////////////////////////////////////////////////////////////////
    // BUTTONS ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////

    Input input;
    input.up    = (Button) {SDL_SCANCODE_UP,    0,0,0};
    input.down  = (Button) {SDL_SCANCODE_DOWN,  0,0,0};
    input.left  = (Button) {SDL_SCANCODE_LEFT,  0,0,0};
    input.right = (Button) {SDL_SCANCODE_RIGHT, 0,0,0};

    input.action = (Button) {SDL_SCANCODE_Z,      0,0,0};
    input.cancel = (Button) {SDL_SCANCODE_X,      0,0,0};
    input.start  = (Button) {SDL_SCANCODE_RETURN, 0,0,0};

    const int key_size = 7;
    Button* buttons[key_size] = {
        &input.up,
        &input.down,
        &input.left,
        &input.right,
        &input.action,
        &input.cancel,
        &input.start
    };

    ///////////////////////////////////////////////////////////////////////////////////////
    // TTF INIT ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////

    int font_size = 30;

    const int charset_size = 128;
    Letter letters_white[charset_size];
    Letter letters_red[charset_size];
    Letter letters_green[charset_size];

    TTF_Font* font = NULL;
    if (TTF_Init() < 0) return 1;

    font = TTF_OpenFont("assets/fonts/LiberationMono-Regular.ttf", font_size);
    if (font == NULL) return 1;

    create_charset(screen.renderer, font, letters_white, charset_size, colors.white);
    create_charset(screen.renderer, font, letters_red, charset_size, colors.red);
    create_charset(screen.renderer, font, letters_green, charset_size, colors.green);

    TTF_CloseFont(font);
    font = NULL;

    ///////////////////////////////////////////////////////////////////////////////////////
    // START LAYOUT ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////

    StartLayout start_layout;
    start_layout.title = "PONG";
    start_layout.message = "Press Start";

    start_layout.blink_flag = 1;
    start_layout.blink_frame_count = 0;
    start_layout.blink_max_frame = 30;

    start_layout.screen = &screen;
    start_layout.letters = letters_white;

    ///////////////////////////////////////////////////////////////////////////////////////
    // PAUSE LAYOUT ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////

    PauseLayout pause_layout;
    pause_layout.title = "PAUSE";

    pause_layout.options[PAUSE_RESUME] = "CONTINUE";
    pause_layout.options[PAUSE_EXIT]   =     "EXIT";

    pause_layout.spacing = 10;
    pause_layout.index = PAUSE_RESUME;

    ///////////////////////////////////////////////////////////////////////////////////////
    // GAMEOVER LAYOUT ////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////

    GameOverLayout gameover_layout;
    gameover_layout.title = "GAME OVER";
    gameover_layout.question = "Continue?";

    gameover_layout.options[GAMEOVER_EXIT_YES] = "Yes";
    gameover_layout.options[GAMEOVER_EXIT_NO]  = "No";

    gameover_layout.spacing = 10;
    gameover_layout.index = GAMEOVER_EXIT_YES;

    ///////////////////////////////////////////////////////////////////////////////////////
    // PONG LAYOUT ////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////

    const int player_count = 2;
    Player players[player_count];

    players[0].box.x = 0;
    players[0].box.y = 0;
    players[0].box.h = 100;
    players[0].box.w =  20;

    players[0].speed = 0;
    players[0].max_speed = 5;
    players[0].score = 0;
    players[0].color = colors.white;

    players[1].box.x = 0;
    players[1].box.y = 0;
    players[1].box.h = 100;
    players[1].box.w =  20;

    players[1].speed = 0;
    players[1].max_speed = 4;
    players[1].score = 0;
    players[1].color = colors.white;

    Ball ball;
    ball.box.x = 0;
    ball.box.y = 0;
    ball.box.h = 20;
    ball.box.w = 20;

    ball.max_speed = 5;
    ball.speed_x = ball.max_speed*1.2;
    ball.speed_y = ball.max_speed/1.2;

    ball.color = colors.white;

    PongLayout pong_layout;
    pong_layout.score_padding   = 20;
    pong_layout.players_padding = 60;
    pong_layout.line[0] = (SDL_Point) {screen.rect.w/2, 0};
    pong_layout.line[1] = (SDL_Point) {screen.rect.w/2, screen.rect.h};


    pong_layout.player_start_position[0].x = pong_layout.players_padding;
    pong_layout.player_start_position[0].y = screen.rect.h/2 - players[0].box.h/2;
    pong_layout.player_start_position[1].x = screen.rect.w - players[1].box.w  - pong_layout.players_padding;
    pong_layout.player_start_position[1].y = screen.rect.h/2 - players[1].box.h/2;

    pong_layout.ball_start_position.x = screen.rect.w/2 - ball.box.w/2;
    pong_layout.ball_start_position.y = screen.rect.h/2 - ball.box.h/2;		


    reset_players(players, &pong_layout);
    reset_ball_speed(&ball, &pong_layout);

    ///////////////////////////////////////////////////////////////////////////////////////

    while (screen.exit == SDL_GAME_RUN)
    {
        ///////////////////////////////////////////////////////////////////////////////////
        // Events /////////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////

        
        while (true)
        {
            SDL_Event event;
            if (SDL_PollEvent(&event) == 0) break;

            if (event.type == SDL_QUIT) screen.exit = SDL_GAME_EXIT;
        }

        ///////////////////////////////////////////////////////////////////////////////////
        // Check Keyboard /////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        update_buttons(buttons, key_size);

        ///////////////////////////////////////////////////////////////////////////////////
        // Game Viewa /////////////////////////////////////////////////////////////////////
        ///////////////////////////////////////////////////////////////////////////////////

        switch (screen.view_index)
        {
            case VIEW_START:
            {
                ///////////////////////////////////////////////////////////////////////////
                // Action Logic ///////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////
                start_action_logic(&input, &screen.view_index);
                ///////////////////////////////////////////////////////////////////////////
                // Colition Detection /////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////
                start_collision_detection();
                ///////////////////////////////////////////////////////////////////////////
                // Update World ///////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////
                start_update_world(&start_layout);
                ///////////////////////////////////////////////////////////////////////////
                // Render /////////////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////
                set_render_draw_color(screen.renderer, screen.clear_color);
                SDL_RenderClear(screen.renderer);

                start_render(&start_layout);

                SDL_RenderPresent(screen.renderer);
            }
            break;
            
            case VIEW_GAME: 
            {
                ///////////////////////////////////////////////////////////////////////////
                // Action Logic ///////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////

                game_action_logic(
                    &input,
                    players,
                    &ball,
                    &screen.view_index
                );

                ///////////////////////////////////////////////////////////////////////////
                // Colition Detection /////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////

                game_collision_detection(players,
                    player_count,
                    &ball,
                    &screen,
                    &pong_layout
                );

                ///////////////////////////////////////////////////////////////////////////
                // Update World ///////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////
                
                game_update_world(players, player_count, &ball, &screen);

                ///////////////////////////////////////////////////////////////////////////
                // Render /////////////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////
                set_render_draw_color(screen.renderer, screen.clear_color);
                SDL_RenderClear(screen.renderer);

                if ((players[0].score < 5) && (players[1].score < 5))
                {
                    game_render(
                        &screen,
                        &pong_layout,
                        players,
                        player_count,
                        &ball,
                        letters_white
                    );
                }
            
                SDL_RenderPresent(screen.renderer);
            }
            break;

            case VIEW_PAUSE:
            {
                ///////////////////////////////////////////////////////////////////////////
                // Action Logic ///////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////
                
                pause_action_logic(&input, &pause_layout, &screen);

                ///////////////////////////////////////////////////////////////////////////
                // Colition Detection /////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////

                ///////////////////////////////////////////////////////////////////////////
                // Update World ///////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////

                ///////////////////////////////////////////////////////////////////////////
                // Render /////////////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////
                set_render_draw_color(screen.renderer, screen.clear_color);
                SDL_RenderClear(screen.renderer);

                game_render(
                    &screen,
                    &pong_layout,
                    players,
                    player_count,
                    &ball,
                    letters_white
                );
                pause_render(
                    &screen,
                    &pause_layout,
                    letters_white,
                    letters_red
                );

                SDL_RenderPresent(screen.renderer);
            }
            break;

            case VIEW_GAMEOVER:
            {
                ///////////////////////////////////////////////////////////////////////////
                // Action Logic ///////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////
                
                gameover_action_logic(
                    &input,
                    &gameover_layout,
                    &screen,
                    &ball,
                    players,
                    &pong_layout
                );

                ///////////////////////////////////////////////////////////////////////////
                // Colition Detection /////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////

                ///////////////////////////////////////////////////////////////////////////
                // Update World ///////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////

                ///////////////////////////////////////////////////////////////////////////
                // Render /////////////////////////////////////////////////////////////////
                ///////////////////////////////////////////////////////////////////////////
                
                set_render_draw_color(screen.renderer, screen.clear_color);
                SDL_RenderClear(screen.renderer);

                gameover_render(
                    &screen,
                    &gameover_layout,
                    letters_white,
                    letters_red
                );

                SDL_RenderPresent(screen.renderer);
            }
            break;

            default:
            {
                set_render_draw_color(screen.renderer, screen.clear_color);
                SDL_RenderClear(screen.renderer);

                render_string(screen.renderer, 0, 0, letters_white, "Unkown view");

                SDL_RenderPresent(screen.renderer);
            }

        }	
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    // Cleanup ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////

    destroy_charset(letters_white, charset_size);
    destroy_charset(letters_red, charset_size);
    destroy_charset(letters_green, charset_size);

    SDL_DestroyRenderer(screen.renderer);
    SDL_DestroyWindow(screen.window);

    TTF_Quit();
    SDL_Quit();

    screen.renderer = NULL;
    screen.window = NULL;

    ///////////////////////////////////////////////////////////////////////////////////////
    return 0;
}

