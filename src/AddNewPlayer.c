#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

#ifdef main
#undef main
#endif

#include <stdio.h>
#include <stdbool.h>
#include "AddNewPlayer.h"

struct player{
    char name[200];
    int  num;
};

void add_new_player(char*text,int score){
    bool found;
    FILE *fptr ;
    struct player arr[200];
    struct player temp;
    char *str = malloc(200 * sizeof (char));

    fptr = fopen( "../players.txt","r+");

    int i=0 , k;
    printf(" l1 \n");
    while (fscanf(fptr,"%s", str) == 1){
        printf(" l2 \n");
        if(strcmp(text,str)==0){
            k = i/2;
            found = true;
        }
        if(i%2==0){
            strcpy(arr[i/2].name , str);
        }
        else{
            arr[i/2].num = atoi(str);
        }
        i++;
    }
    if(!found){

        printf(" l3 \n");
        for(int j=0; j<i; j++){
            printf("*%s %d\n",arr[j].name,arr[j].num);
        }

        strcpy(arr[i/2].name , text);
        arr[i/2].num = score;

        printf(" l3.5 \n");
        for(int j=0; j<i; j++){
            printf("*%s %d\n",arr[j].name,arr[j].num);
        }

        //fprintf(fptr,"%s \n",text);

        for(int j=0; j<i; j++){
            if(arr[j].num < score){
                temp = arr[j];
                arr[j] = arr[i/2];
                k = j;
                break;
            }
        }

        printf(" l3.75 \n");
        for(int j=0; j<i; j++){
            printf("*%s %d\n",arr[j].name,arr[j].num);
        }

        for(int j=i/2; j>k; j--){
            arr[j] = arr[j-1];
        }
        arr[k+1] = temp;
        printf(" l3.9 \n");
        for(int j=0; j<i; j++){
            printf("*%s %d\n",arr[j].name,arr[j].num);
        }
    }
    else{
        for(int j=0; j<i; j++){
            if(arr[j].num < arr[k].num){
                temp = arr[j];
                arr[j] = arr[i/2];
                k = j;
                break;
            }
        }
    }
    printf(" l4 \n");
    for(int j=0; j<i; j++){
        printf("*%s %d\n",arr[j].name,arr[j].num);
    }
    free(str);
    fclose(fptr);
}

/*
 * if(strcmp(text,str)==0){
            found = true;
            arr[i].num = atoi(str) + score;
        }
        else if(i%2==0){
            strcpy(arr[i].name , str);
        }
        else{
            arr[i].num = atoi(str);
        }
        i++;
 */