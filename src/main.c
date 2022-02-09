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

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FPS = 60;
char *text ;
char *MenuLable[3] = {"new game","scoreboard","exit"};
char *MapLable[3] = {"map 1","map 2","random map"};
//char *composition;
//Sint32 cursor;
//Sint32 selection_len;

struct map{
    int * x;
    int * y;
    Uint32 * color;
    char* text[48];
};
struct potion{
    int area;
    SDL_Texture *type[4];
};
struct moving{
    int *x1;
    int *y1;
    int *x2;
    int *y2;
    Uint32 * color;
};
int play_map1(SDL_Renderer *sdlRenderer ,SDL_Texture *sdlTexture , struct map map , struct potion potion){
    potion.area=rand()%16+16;
    potion.type[0] = getImageTexture(sdlRenderer, "../erlen2.bmp");
    SDL_Rect texture_rect = {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT};
    SDL_Texture *castels[2] ;
    castels[0] = getImageTexture(sdlRenderer, "../pink castel2.bmp");
    castels[1] = getImageTexture(sdlRenderer, "../red castel2.bmp");
    SDL_Rect pos[48];
    map.x = (int*)malloc(48 * sizeof(int));
    map.y = (int*)malloc(48 * sizeof(int));
    map.color = (Uint32*)malloc(48 * sizeof(Uint32));
    Uint32 color2 , color ;
    for(int i=0; i<48; i++) {
        map.text[i]=malloc(2 * sizeof(char));
        if(i/8==0 || i/8==5){
            color2 = 0xff00ffff;//yellow : خالی
        }
        else if(i/8==1){
            color2 = 0xff0000ff;//red : حریف
            strcpy(map.text[i],"10");
        }
        else if(i/8==4){
            color2 = 0xffff00ff;//pink : ما
            strcpy(map.text[i],"20");
        }
        else{
            color2 = 0xffffffff;//white : بی طرف
            strcpy(map.text[i],"10");
        }
        map.color[i] = color2;
        map.x[i] = 40 + (i % 8) * 80;
        map.y[i] = 40 + (i / 8) * 80;
        pos[i].x = map.x[i]-20 ;
        pos[i].y = map.y[i]-20;
        pos[i].h = 40;
        pos[i].w = 40;
    }
    int x,y,t=0;
    bool running = true , first = true;
    while(running){
        printf("%ld %d\n",time(NULL),rand());
        SDL_SetRenderDrawColor(sdlRenderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(sdlRenderer);
        SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &texture_rect);
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    running = false;
                    //free-----------
                    return 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    x = sdlEvent.button.x;
                    y = sdlEvent.button.y;
                    int m = 8*ceil(y/80) + ceil(x/80);
                    if(m>=8 && m<40){
                        //printf(" %d %d %d\n",x,y,m);
                        if(map.color[m]==0xffff00ff && first==true){
                            first = false;

                        }
                        SDL_Rect outlineRect = {map.x[m]-40, map.y[m]-40, 80, 80 };
                        SDL_SetRenderDrawColor( sdlRenderer, 0x00, 0xFF, 0x00, 0xFF );
                        SDL_RenderDrawRect( sdlRenderer, &outlineRect );
                    }
            }
        }
        for(int i=0; i<48; i++){

//            if(t%5 == 0){
//                printf("%d\n",t);
                if(map.text[i][0]<'9' && map.color[i]!=0xffffffff){
                    if(map.text[i][1]>='9'){
                        map.text[i][1] = '0';
                        map.text[i][0]++;
                    }
                    else{
                        map.text[i][1]++;
                    }
                }
//                t = 1;
//            }

            // areas
            filledCircleColor(sdlRenderer, map.x[i], map.y[i], 40, map.color[i]);
            // icons
            if(map.color[i]==0xffff00ff)
                SDL_RenderCopy(sdlRenderer, castels[0], NULL, &pos[i]);
            else if(map.color[i] == 0xff0000ff)
                SDL_RenderCopy(sdlRenderer, castels[1], NULL, &pos[i]);
            // numbers
            if (map.color[i]==0xffffffff){
                color = 0xff0000ff;//red
                stringColor(sdlRenderer,map.x[i]-5,map.y[i]+22,map.text[i],color);
            }
            else if(map.color[i]!=0xff00ffff ){
                color = 0xffffffff;//white
                stringColor(sdlRenderer,map.x[i]-5,map.y[i]+22,map.text[i],color);
            }
            if(i==potion.area){
                SDL_RenderCopy(sdlRenderer, potion.type[0], NULL, &pos[i]);
            }
        }

        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000);
    }
}


int main() {
    srand(time(NULL));
    init();
    SDL_Window *sdlWindow = SDL_CreateWindow("Test_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                             SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_Texture *sdlTexture = getImageTexture(sdlRenderer, "../im1.bmp");
    bool start = true , menu = false , mapmenu = false , map1 = false;
    // the rectangle area you wanna put the image
    SDL_Rect texture_rect = {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT};
    text = malloc(200);
    *text = 0;
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
            struct map m ;
            struct potion p;
//            play_map1(sdlRenderer,m);
            switch (play_map1(sdlRenderer,sdlTexture,m,p)) {
                case 0:
                    shallExit = SDL_TRUE;
                    break;
            }
//            for(int i=0; i<48; i++){
//                free(m.color[i]);
//            }
//            printf("sdsds");
        }
        else if(mapmenu){
            switch(showmenu(sdlRenderer,MapLable)){
                case 0:
                    shallExit = SDL_TRUE;
                    mapmenu=false;
                    break;
                case 1:
                    map1=true;
                    mapmenu=false;
                    break;
                case 2:
                    mapmenu = false;
                    break;
                case 3:
                    mapmenu = false;
                    break;
            }
        }
        else if(menu){
            switch(showmenu(sdlRenderer,MenuLable)){
                case 0:
                    shallExit = SDL_TRUE;
                    menu = false;
                    break;
                case 1:
                    menu = false;
                    mapmenu = true;
                    break;
                case 2:
                    menu = false;
                    break;
                case 3:
                    menu = false;
                    break;
            }
        }
        else{
            Uint32 color = 0xffffffff;
            stringColor(sdlRenderer,SCREEN_WIDTH/2-120,SCREEN_HEIGHT/2,"please press exit button again",color);
        }
        int x,y;
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
                            printf("$");
                        } else {
                            strcat(text, sdlEvent.text.text);
                        }
                    printf("%s",sdlEvent.text.text);
                    break;
                /*case SDL_TEXTEDITING:
//                  Update the composition text.
//                  Update the cursor position.
//                  Update the selection length (if any).
                    composition = sdlEvent.edit.text;
                    cursor = sdlEvent.edit.start;
                    selection_len = sdlEvent.edit.length;
                    break;*/
                case SDL_MOUSEBUTTONDOWN:
                    x = sdlEvent.button.x;
                    y = sdlEvent.button.y;
                    printf("%d %d \n",x,y);
                    break;
            }
        }

        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000 / FPS);
        SDL_StopTextInput();
    }

    SDL_DestroyWindow(sdlWindow);

    printf("Hello World\n");
    printf("%s",text);
    SDL_Quit();
    return 0;
}
