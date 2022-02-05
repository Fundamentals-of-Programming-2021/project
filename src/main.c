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
const int CELL_NUM = 8;
char *text ;
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


int main() {
    init();
    SDL_Window *sdlWindow = SDL_CreateWindow("Test_Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                             SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_Renderer *sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_Texture *sdlTexture = getImageTexture(sdlRenderer, "../im1.bmp");

    // the rectangle area you wanna put the image
    SDL_Rect texture_rect = {.x=0, .y=0, .w=SCREEN_WIDTH, .h=SCREEN_HEIGHT};
    text = malloc(200);
    *text = 0;
    SDL_bool shallExit = SDL_FALSE;
    while (shallExit == SDL_FALSE) {
        SDL_SetRenderDrawColor(sdlRenderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(sdlRenderer);

        SDL_RenderCopy(sdlRenderer, sdlTexture, NULL, &texture_rect);
        Uint32 color = 0xffffffff;
        stringColor(sdlRenderer,20,150,text,color);

        SDL_StartTextInput();
        SDL_Event sdlEvent;
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    shallExit = SDL_TRUE;
                    break;
                case SDL_TEXTINPUT:
                    /* Add new text onto the end of our text */
                    printf("%s***",text);
                    strcat(text, sdlEvent.text.text);
                    if(sdlEvent.text.text[0]==';'){
                        sdlTexture = getImageTexture(sdlRenderer, "../im2.bmp");
                        
                        printf("$");
                    }
                    printf("%s**",sdlEvent.text.text);
                    break;
                case SDL_TEXTEDITING:
                    /*
                    Update the composition text.
                    Update the cursor position.
                    Update the selection length (if any).
                    */
                    composition = sdlEvent.edit.text;
                    cursor = sdlEvent.edit.start;
                    selection_len = sdlEvent.edit.length;
                    break;
                case SDL_MOUSEBUTTONDOWN:
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
