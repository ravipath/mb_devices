#include <stdio.h>

int main(void){

    int i = 7;

    printf("Before inc i: %d\n", i);

    if(i++ == 8){
        printf("i is 8\n");
    }
    else{
        printf("i is 7\n");
    }
    printf("After inc i: %d\n", i);
    return 0;
}