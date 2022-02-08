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
    Uint32 * color;
    char* text[48];
};
int play_map1(SDL_Renderer *sdlRenderer ,SDL_Texture *sdlTexture , struct map map){
    SDL_Rect texture_rect = {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT};
    SDL_Texture *castels[2] ;
    castels[0] = getImageTexture(sdlRenderer, "../pink castel.bmp");
    castels[1] = getImageTexture(sdlRenderer, "../red castel.bmp");
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
            //castels[i] = SDL_LoadBMP("../red castel.bmp");
                    //getImageTexture(sdlRenderer, "../red castel.bmp");
        }
        else if(i/8==4){
            color2 = 0xffff00ff;//pink : ما
            strcpy(map.text[i],"20");
            //castels[i] = SDL_LoadBMP("../pink castel.bmp");
                    //getImageTexture(sdlRenderer, "../pink castel.bmp");
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
    bool running = true;
    while(running){
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
                        printf(" %d %d %d\n",x,y,m);
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

        }
        
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000);
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
//            play_map1(sdlRenderer,m);
            switch (play_map1(sdlRenderer,sdlTexture,m)) {
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
