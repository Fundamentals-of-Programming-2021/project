#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#ifdef main
#undef main
#endif

#include <stdio.h>
#include <stdbool.h>
#include "menu.h"

int showmenu(SDL_Renderer *sdlRenderer , char* lables[3]){
    int selected[3] = {0,0,0};
    Uint32 color[2] = {0xffffffff,0xff0000ff};
    bool running = true;
    int x,y;
    while(running){
        stringColor(sdlRenderer,SCREEN_WIDTH/2-50,SCREEN_HEIGHT/2-50,lables[0],color[selected[0]]);
        stringColor(sdlRenderer,SCREEN_WIDTH/2-50,SCREEN_HEIGHT/2,lables[1],color[selected[1]]);
        stringColor(sdlRenderer,SCREEN_WIDTH/2-50,SCREEN_HEIGHT/2+50,lables[2],color[selected[2]]);
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    running = false;
                    printf(" ha ");
                    return 0;
                    break;
                case SDL_MOUSEMOTION:
                    x = sdlEvent.motion.x;
                    y = sdlEvent.motion.y;
                    if(x>=SCREEN_WIDTH/2-50 && x<=SCREEN_WIDTH/2+30 && y<=SCREEN_HEIGHT/2-45 && y>=SCREEN_HEIGHT/2-52){
                        selected[0]=1;
                    }
                    else{
                        if(selected[0]==1){
                            selected[0]=0;
                        }
                    }
                    if(x>=SCREEN_WIDTH/2-50 && x<=SCREEN_WIDTH/2+30 && y>=SCREEN_HEIGHT/2-4 && y<=SCREEN_HEIGHT/2+4){
                        selected[1]=1;
                    }
                    else{
                        if(selected[1]==1){
                            selected[1]=0;
                        }
                    }
                    if(x>=SCREEN_WIDTH/2-50 && x<=SCREEN_WIDTH/2+30 && y>=SCREEN_HEIGHT/2+47 && y<=SCREEN_HEIGHT/2+55){
                        selected[2]=1;
                    }
                    else{
                        if(selected[2]==1){
                            selected[2]=0;
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    x = sdlEvent.button.x;
                    y = sdlEvent.button.y;
                    if(x>=SCREEN_WIDTH/2-50 && x<=SCREEN_WIDTH/2+30 && y<=SCREEN_HEIGHT/2-45 && y>=SCREEN_HEIGHT/2-52){
                        return 1;
                    }
                    if(x>=SCREEN_WIDTH/2-50 && x<=SCREEN_WIDTH/2+30 && y>=SCREEN_HEIGHT/2-4 && y<=SCREEN_HEIGHT/2+4){
                        return 2;
                    }
                    if(x>=SCREEN_WIDTH/2-50 && x<=SCREEN_WIDTH/2+30 && y>=SCREEN_HEIGHT/2+47 && y<=SCREEN_HEIGHT/2+55){
                        return 3;
                    }
                    break;
            }
        }
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000 / FPS);
    }
}

