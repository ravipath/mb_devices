#include <stdio.h>
#include <stdint.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

int main(void){

    char c1 = 'R';
    char c2 = 'A';
    char c3 = 'V';
    char c4 = 'I';
    char c5 = 'P';
    char c6 = 'A';
    char c7 = 'T';
    char c8 = 'H';
    char c9 = 'A';
    char c10 = 'K';

    uint16_t twobytes = 0;

    twobytes = twobytes | (uint16_t)(c1 << 8);
    twobytes = twobytes | c2;
    printf("Two bytes: %d\n", twobytes);
    printf("Two bytes(H): %d(%c)\n", twobytes>>8, twobytes>>8);
    printf("Two bytes(L): %d(%c)\n\n", (char)twobytes, (char)twobytes);

    twobytes = 0;
    twobytes = twobytes | (uint16_t)(c3 << 8);
    twobytes = twobytes | c4;
    printf("Two bytes: %d\n", twobytes);
    printf("Two bytes(H): %d(%c)\n", twobytes>>8, twobytes>>8);
    printf("Two bytes(L): %d(%c)\n\n", (char)twobytes, (char)twobytes);
    
    twobytes = 0;
    twobytes = twobytes | (uint16_t)(c5 << 8);
    twobytes = twobytes | c6;
    printf("Two bytes: %d\n", twobytes);
    printf("Two bytes(H): %d(%c)\n", twobytes>>8, twobytes>>8);
    printf("Two bytes(L): %d(%c)\n\n", (char)twobytes, (char)twobytes);
    
    twobytes = 0;
    twobytes = twobytes | (uint16_t)(c7 << 8);
    twobytes = twobytes | c8;
    printf("Two bytes: %d\n", twobytes);
    printf("Two bytes(H): %d(%c)\n", twobytes>>8, twobytes>>8);
    printf("Two bytes(L): %d(%c)\n\n", (char)twobytes, (char)twobytes);
    
    twobytes = 0;
    twobytes = twobytes | (uint16_t)(c9 << 8);
    twobytes = twobytes | c10;
    printf("Two bytes: %d\n", twobytes);
    printf("Two bytes(H): %d(%c)\n", twobytes>>8, twobytes>>8);
    printf("Two bytes(L): %d(%c)\n\n", (char)twobytes, (char)twobytes);
    

    /*printf("Two bytes(H): %d(%c)\n", twobytes>>8, twobytes>>8);
    printf("Two bytes(L): %d(%c)\n", (char)twobytes, (char)twobytes);
    printf("m: "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(twobytes>>8), BYTE_TO_BINARY(twobytes));*/

    return 0;
}