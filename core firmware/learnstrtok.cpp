#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int incomingR;
int incomingG;
int incomingB;

//int parse(String inputString);


int main (){
    char inputString[] = "255,0,123";
    char * tok;

    printf("\nSplitting string \"%s\" into tokens:\n",inputString);
    tok = strtok(inputString,",");
    while (tok!=NULL){
        printf("%s\n",tok);
        tok = strtok(NULL,",");
    }

    printf("Now let's convert these to integers\n");
    char anotherInputString[] = "255,0,123";
    int colors[3];
    int i=0;

    tok = strtok(anotherInputString,",");
    while (tok!=NULL){
        colors[i] = atoi(tok);
        tok = strtok(NULL,",");
        i++;
    }

    printf("R: %d\n", colors[0]);
    printf("G: %d\n", colors[1]);
    printf("B: %d\n", colors[2]);


    return 0;
}
