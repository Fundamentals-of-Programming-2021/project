#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#ifdef main
#undef main
#endif

#include <stdio.h>
#include "score.h"
int showScore(SDL_Renderer *sdlRenderer ){
    FILE *fptr;
    char name[200];
    char num[20];
    fptr = fopen( "../players.txt","r");
    fscanf(fptr,"%s %s",name,num);
    stringColor(sdlRenderer,190,180,"The First Player:",0xffffffff);
    stringColor(sdlRenderer,400,180,name,0xffffffff);

    stringColor(sdlRenderer,190,230,"With Score:",0xffffffff);
    stringColor(sdlRenderer,400,230,num,0xffffffff);
    fclose(fptr);
}
