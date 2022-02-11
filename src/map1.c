#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#ifdef main
#undef main
#endif

#include <stdio.h>
#include <stdbool.h>
#include "map1.h"
#include "getImage.h"

struct point{
    double x,y;
};

struct potion{
    int area;
    SDL_Texture *type[5];
};

struct sarbaz{
    double x,y;
    struct sarbaz * next;
};

struct point push( struct sarbaz **head, int data ,double vx ,int y,double vy){
    struct point p;
    struct sarbaz *new_node = (struct sarbaz*) malloc( sizeof( struct sarbaz ));

    new_node->x = data;
    new_node->y = y;

    new_node->next = (*head);

    (*head) = new_node;

    struct sarbaz *last = *head;
    while (last->next != NULL) {
        printf("%f ",last-> x);
        printf("%f \n",last-> y);
        p.x = last->x;
        p.y = last->y;
        last-> x += vx;
        last-> y += vy;
        last = last->next;
    }
    printf("$$%f ",last-> x);
    printf("%f \n",last-> y);

    printf("&&%f ",p. x);
    printf("%f \n",p. y);


    return p;
}
void output( struct sarbaz **head ,SDL_Renderer *sdlRenderer)
{
    for( struct sarbaz *current =*head; current != NULL; current = current->next )
    {
        filledCircleColor(sdlRenderer,current->x,current->y,5,0xffff00ff);
        circleColor(sdlRenderer,current->x,current->y,5,0xffffffff);
        //printf( "%d ", current->x );
    }

}
int play_map1(SDL_Renderer *sdlRenderer ,SDL_Texture *sdlTexture){
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
    struct point p[48];
    struct sarbaz *sar[48] = {0} ;

    for(int i=0; i<48; i++) {
        if(i/8==0 || i/8==5){
            color = 0xff00ffff;//yellow : خالی
        }
        else if(i/8==1){
            color = 0xff0000ff;//red : حریف
            num[i] = 10;
            grow[i] = 1;
        }
        else if(i/8==4){
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
                        if(colorMap[m]==0xffff00ff && first==true){
                            first = false;
                            area = m;
                            printf(" f%d\n",m);
                        }
                        else if(first==false && colorMap[m]!= 0xff00ffff && m!=area){
                            first = true;
                            p[area].x = a;
                            p[area].y = b;
                            move[area] = 1;
                            grow[area] = 0;
                            double r = sqrt((x[m]-x[area])*(x[m]-x[area]) + (y[m]-y[area])*(y[m]-y[area]));
                            vx[area] = (x[m]-x[area])/r * 20;
                            vy[area] = (y[m]-y[area])/r * 20;
                            printf(" s%d\n",m);
                        }
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
//                if(i==47)
//                    t=1;
                if(grow[i]==1 && num[i]<40){
                    num[i]++;
                }
                else if(grow[i]==-1){
                    num[i]--;
                    if(num[i]<0){
                        num[i]=1;
                        colorMap[i] = 0xffff00ff;
                        grow[i]=1;
                    }
                }
                if(move[i]==1 && num[i]>0){
                    num[i]--;
                    struct point temp = push(&sar[i],x[i],vx[i],y[i],vy[i]);
                    int m = 8*ceil((int)(p[i].y)/80) + ceil((int)(p[i].x)/80);
                    int n = 8*ceil((int)temp.y/80) + ceil((int)temp.x/80);
                    printf("m=%d n=%d %d %d\n",m,n,p[i].x,p[i].y);
                    if(n==m){
                        printf("residam");
                        if(colorMap[m]==colorMap[i]){
                            num[m]++;
                        }
                        else{
                            grow[m]=-1;
                        }
                    }
                    printf("*%f*\n",vy[i]);
                }
//                else if(move[i]==1 && num[i]>0)
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

            output(&sar[i],sdlRenderer);
        }
        t++;
        SDL_RenderPresent(sdlRenderer);
        SDL_Delay(1000/FPS);
    }
}

