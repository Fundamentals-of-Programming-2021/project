#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#ifdef main
#undef main
#endif

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "init.h"
#include "getImage.h"
#include "menu.h"
#include "AddNewPlayer.h"
#include "score.h"
#include "map1.h"
#include "map2.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FPS = 60;
char *text ;
char *MenuLabel[3] = {"new game","scoreboard","exit"};
char *MapLabel[3] = {"map 1","map 2","random map"};


int main() {
    init();
    srand(time(NULL));

    SDL_Window *sdlWindow = SDL_CreateWindow("Test_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                             SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_Texture *sdlTexture = getImageTexture(sdlRenderer, "../im1.bmp");
    SDL_Rect texture_rect = {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT};

    bool start = true , menu = false , mapmenu = false , map1 = false , scoreBoard = false , map2 = false;
    text = malloc(200);
    *text = 0;
    int score = 0;
    SDL_bool shallExit = SDL_FALSE;
    while (shallExit == SDL_FALSE) {
        SDL_SetRenderDrawColor(sdlRenderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(sdlRenderer);
        SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &texture_rect);
        if(start){
            Uint32 color = 0xffffffff;
            stringColor(sdlRenderer,20,150,text,color);
            SDL_StartTextInput();
        }
        else if(map1){
            switch (play_map1(sdlRenderer,sdlTexture)) {
                case 0:
                    shallExit = SDL_TRUE;
                    break;
                case 1:
                    score += 50;
                    break;
                case -1:
                    score -= 50;
                    break;
            }
        }
        else if(map2){
            switch (play_map2(sdlRenderer,sdlTexture)) {
                case 0:
                    shallExit = SDL_TRUE;
                    break;
            }
        }
        else if(mapmenu){
            switch(showmenu(sdlRenderer,MapLabel)){
                case 0:
                    shallExit = SDL_TRUE;
                    mapmenu=false;
                    break;
                case 1:
                    map1 = true;
                    mapmenu=false;
                    break;
                case 2:
                    map2 = true;
                    mapmenu = false;
                    break;
                case 3:
                    mapmenu = false;
                    break;
            }
        }
        else if(menu){
            switch(showmenu(sdlRenderer,MenuLabel)){
                case 0:
                    shallExit = SDL_TRUE;
                    menu = false;
                    break;
                case 1:
                    menu = false;
                    mapmenu = true;
                    break;
                case 2:
                    scoreBoard = true;
                    menu = false;
                    break;
                case 3:
                    menu = false;
                    break;
            }
        }
        else if(scoreBoard){
            showScore(sdlRenderer);
        }
        else{
            Uint32 color = 0xffffffff;
            stringColor(sdlRenderer,SCREEN_WIDTH/2-120,SCREEN_HEIGHT/2,"please press exit button again",color);
        }
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    shallExit = SDL_TRUE;
                    break;
                case SDL_TEXTINPUT:
                        if (sdlEvent.text.text[0] == ';') {
                            sdlTexture = getImageTexture(sdlRenderer, "../im2.bmp");
                            start = false;
                            menu = true;
                        }
                        else {
                            strcat(text, sdlEvent.text.text);
                        }
                    break;
            }
        }
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000 / FPS);
        SDL_StopTextInput();
    }

    SDL_DestroyWindow(sdlWindow);
    add_new_player(text,score);
    free(text);
    SDL_Quit();
    return 0;
}
