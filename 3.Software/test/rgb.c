#include <stdio.h>
#include <stdint.h>

int main(){
    uint8_t a[4][4] = {};
    uint8_t b = 0;
    for(int i = 0; i < 4; i ++){
        for(int j = 0; j < 4; i ++){
            b ++;
            a[i][j] = b;
        }
    }

    for(int i = 0; i < 16; i++){
        printf("%d|", **a+i);
    }
}