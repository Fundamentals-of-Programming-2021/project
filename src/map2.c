#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#ifdef main
#undef main
#endif

#include <stdio.h>
#include <stdbool.h>
#include "map2.h"
#include "getImage.h"

struct potion{
    int area;
    SDL_Texture *type[5];
};

int play_map2(SDL_Renderer *sdlRenderer ,SDL_Texture *sdlTexture){
    SDL_Rect texture_rect = {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT};

    struct potion potion;
    potion.type[1] = getImageTexture(sdlRenderer, "../erlen1.bmp");
    potion.type[2] = getImageTexture(sdlRenderer, "../erlen2.bmp");
    potion.type[3] = getImageTexture(sdlRenderer, "../erlen3.bmp");
    potion.type[4] = getImageTexture(sdlRenderer, "../erlen4.bmp");

    SDL_Texture *castels[2] ;
    castels[0] = getImageTexture(sdlRenderer, "../pink castel2.bmp");
    castels[1] = getImageTexture(sdlRenderer, "../red castel2.bmp");

    SDL_Rect pos[48];
    int x[48] , y[48] ,num[48] ,grow[48] , move[48]={0} ;
    double vx[48] , vy[48];
    Uint32 colorMap[48] , color;

    for(int i=0; i<48; i++) {
        if(i/8==0 || i/8==5){
            color = 0xff00ffff;//yellow : خالی
        }
        else if(i/8==4){
            color = 0xff0000ff;//red : حریف
            num[i] = 10;
            grow[i] = 1;
        }
        else if(i/8==1){
            color = 0xffff00ff;//pink : ما
            num[i] = 20;
            grow[i] = 1;
        }
        else{
            color = 0xffffffff;//white : بی طرف
            num[i] = 10;
            grow[i] = 0;
        }
        colorMap[i] = color;
        x[i] = 40 + (i % 8) * 80;
        y[i] = 40 + (i / 8) * 80;
        pos[i].x = x[i]-20 ;
        pos[i].y = y[i]-20;
        pos[i].h = 40;
        pos[i].w = 40;
    }
    int t=0 ,area;
    bool running = true , first = true , pot = false;
    while(running){
        SDL_SetRenderDrawColor(sdlRenderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(sdlRenderer);
        SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &texture_rect);
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            int a,b;
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    running = false;
                    for(int i=0; i<5; i++){
                        SDL_DestroyTexture(potion.type[i]);
                    }
                    for(int i=0; i<2; i++){
                        SDL_DestroyTexture(castels[i]);
                    }
                    return 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    a = sdlEvent.button.x;
                    b = sdlEvent.button.y;
                    int m = 8*ceil(b/80) + ceil(a/80);
                    if(m==potion.area){
                        pot = false;
                    }
                    if(m>=8 && m<40){
                        SDL_Rect outlineRect = {x[m]-40, y[m]-40, 80, 80 };
                        SDL_SetRenderDrawColor( sdlRenderer, 0x00, 0xFF, 0x00, 0xFF );
                        SDL_RenderDrawRect( sdlRenderer, &outlineRect );
                    }
            }
        }
        if(pot==false && rand()%100==0){
            pot = true;
            potion.area=rand()%16+16;
            potion.type[0] = potion.type[rand()%4+1];
        }

        for(int i=0; i<48; i++){
            if(t%60==0){
                if(i==47)
                    t=1;
                if(grow[i]==1 && num[i]<40){
                    num[i]++;
                }
                else if(grow[i]==-1){
                    num[i]--;
                }

            }

            // areas
            filledCircleColor(sdlRenderer, x[i], y[i], 40, colorMap[i]);
            // icons
            if(colorMap[i]==0xffff00ff)
                SDL_RenderCopy(sdlRenderer, castels[0], NULL, &pos[i]);
            else if(colorMap[i] == 0xff0000ff)
                SDL_RenderCopy(sdlRenderer, castels[1], NULL, &pos[i]);
            // numbers
            if (colorMap[i]==0xffffffff){
                char buffer[20];
                itoa(num[i],buffer,10);
                stringColor(sdlRenderer,x[i]-5,y[i]+22,buffer,0xff0000ff);
            }
            else if(colorMap[i]!=0xff00ffff ){
                char buffer[20];
                itoa(num[i],buffer,10);
                stringColor(sdlRenderer,x[i]-5,y[i]+22,buffer,0xffffffff);
            }
            //potion
            if(i==potion.area && pot==true){
                SDL_RenderCopy(sdlRenderer, potion.type[0], NULL, &pos[i]);
            }

        }
        t++;
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000/FPS);
    }
}

