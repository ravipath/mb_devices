#include<stdio.h>

union intandfloat{
    int i;
    float f;
};

int main(void){
    union intandfloat u;

    u.i = 1234;
    u.f = 123456.789;
    
    printf("int part = %d\n", u.i);
    printf("float part = %f\n", u.f);


    return 0;
}