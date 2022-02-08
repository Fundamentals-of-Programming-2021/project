#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#ifdef main
#undef main
#endif

#include <stdio.h>
#include<time.h>
#include <stdbool.h>
#include "init.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FPS = 60;
char *text ;
char *MenuLable[3] = {"new game","scoreboard","continue"};
char *MapLable[3] = {"map 1","map 2","random map"};
char *composition;
Sint32 cursor;
Sint32 selection_len;



SDL_Texture *getImageTexture(SDL_Renderer *sdlRenderer, char *image_path) {
    SDL_Surface *image = SDL_LoadBMP(image_path);

    // Let the user know if the file failed to load
    if (!image) {
        printf("Failed to load image at %s: %s\n", image_path, SDL_GetError());
        return 0;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(sdlRenderer, image);

    SDL_FreeSurface(image);
    image = NULL;

    return texture;
}


int showmenu(SDL_Renderer *sdlRenderer , char* lables[3]){
    int selected[3] = {0,0,0};
    Uint32 color[2] = {0xffffffff,0xff0000ff};
    //Uint32 color = 0xffffffff;
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
struct map{
    int * x;
    int * y;
    //int * r;
    Uint32 * color;
};
int play_map1(SDL_Renderer *sdlRenderer ,struct map m){
    m.x = (int*)malloc(36 * sizeof(int));
    m.y = (int*)malloc(36 * sizeof(int));
    //m.r = (int*)malloc(36 * sizeof(int));
    m.color = (Uint32*)malloc(36 * sizeof(Uint32));
    Uint32 color2 = 0xffffffff;
    for(int i=0; i<48; i++){
        m.x[i] = 40 + (i%8)*80;
        m.y[i] = 40 + (i/8)*80;
        //if
        filledCircleColor(sdlRenderer, m.x[i], m.y[i], 40, color2);
        //SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000 / FPS);
    }

}


int main() {
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
            struct map m;
            play_map1(sdlRenderer,m);
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
