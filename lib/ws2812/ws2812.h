/*
    Aluno: Lucas Carneiro de Araújo Lima
*/

#ifndef WS2812_H
#define WS2812_H

#include "../config/config.h"
#include <string.h>

void frame_cleaner(Led_Matrix *frame) {
    for(int i = 0; i < 5; i++) {
        for(int j = 0; j < 5; j++) {
            (*frame)[i][j].red = 0.0;
            (*frame)[i][j].blue = 0.0;
            (*frame)[i][j].green = 0.0;
        }
    }
}

//Definição da cor escolhida
uint32_t color(double b, double r, double g) {
  return (((unsigned char)(g * 255)) << 24) | (((unsigned char)(r * 255)) << 16) | (((unsigned char)(b * 255)) << 8);
}

//Imprimindo o frame do número na matriz de led
void symbol_printing(Led_Matrix *frame){
    for (int i = 4; i >= 0; i--){
        if(i % 2) {
            for (int j = 0; j < 5; j ++) 
                pio_sm_put_blocking((PIO)pio, sm, color((*frame)[i][j].blue, (*frame)[i][j].red, (*frame)[i][j].green));
        }else {
            for (int j = 4; j >= 0; j --)
                pio_sm_put_blocking((PIO)pio, sm, color((*frame)[i][j].blue, (*frame)[i][j].red, (*frame)[i][j].green));
        }
    }
}

//Forma o frame do número
void symbol_framer(Led_Matrix *frame, float r, float g, float b, int row, int col, bool clear, bool print) {
    if(clear) frame_cleaner(frame);
    (*frame)[row][col].red = r;
    (*frame)[row][col].blue = b;
    (*frame)[row][col].green = g;
    if(print) symbol_printing(frame); 
}

void ws2812_draw_column(Led_Matrix *frame, float r, float g, float b, int col, bool clear, bool print) {
    symbol_framer(frame,r,g,b,0,col,clear,false);
    symbol_framer(frame,r,g,b,1,col,false,false);
    symbol_framer(frame,r,g,b,2,col,false,false);
    symbol_framer(frame,r,g,b,3,col,false,false);
    symbol_framer(frame,r,g,b,4,col,false,print);
}

void ws2812_clean(Led_Matrix *frame) {
    frame_cleaner(frame);
    symbol_printing(frame); 
}

#endif