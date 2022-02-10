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
    FILE *fptr , *fptr2;
    struct player arr[200];
    struct player temp;
    char *str = malloc(200 * sizeof (char));

    fptr = fopen( "../players.txt","r");

    int i=0 , k;
    while (fscanf(fptr,"%s", str) == 1){
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
    fclose(fptr);

    if(!found){
        i = i/2 ;
        arr[i].num = score;
        strcpy(arr[i].name,text);
        int j = i-1;
        while(score > arr[j].num && j>=0){
            //printf("%s %s\n",arr[j].name,arr[j-1].name);
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1].num = score;
        strcpy(arr[j+1].name,text);
        i++;
    }
    else{
        i = i/2;
        if(score<0){
            score += arr[k].num;
            int j = k+1;
            while(score < arr[j].num && j<i){
                //printf("%s %s\n",arr[j-1].name,arr[j].name);
                arr[j-1]= arr[j];
                j++;
            }
            arr[j-1].num = score;
            strcpy(arr[j-1].name,text);
        }
        else if(score>0){
            score += arr[k].num;
            int j = k-1;
            while(score > arr[j].num && j>=0){
                arr[j+1] = arr[j];
                j--;
            }
            arr[j+1].num = score;
            strcpy(arr[j+1].name,text);
        }
    }

    fptr2 = fopen("../temp.txt","w");
    for(int j=0; j<i; j++){
        fprintf(fptr2,"%s %d\n",arr[j].name,arr[j].num);
        printf("*%s %d\n",arr[j].name,arr[j].num);
    }
    fclose(fptr2);
    free(str);
    remove("../players.txt");
    rename("../temp.txt", "../players.txt");
}

/*
  printf(" l4 \n");
for(int j=0; j<i; j++){
    printf("*%s %d\n",arr[j].name,arr[j].num);
}
printf("%s %s\n",arr[j].name,arr[j-1].name);
 */