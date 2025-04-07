#include "asteroides.h"


const struct DibujableConstante Asteroide_pequeno = {
    CENTRO_ASTEROIDE_PEQUENO,
    (struct Punto[]){
        {5, 17}, {0, 11}, {4, 9}, {0, 7}, {7, 0}, {13, 0},
        {ANCHURA_ASTEROIDE_PEQUENO, 6}, {ANCHURA_ASTEROIDE_PEQUENO, 12},
        {14, ALTURA_ASTEROIDE_PEQUENO}, {9, ALTURA_ASTEROIDE_PEQUENO}, {9, 11} 
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}, {10, 0}
    },
    11,
    11
};

const struct DibujableConstante Asteroide_mediano = {
    CENTRO_ASTEROIDE_MEDIANO,
    (struct Punto[]){
        {9,ALTURA_ASTEROIDE_MEDIANO},{0,20},{0,8},{13,8},{9,0},{20,0},
        {ANCHURA_ASTEROIDE_MEDIANO,7},{ANCHURA_ASTEROIDE_MEDIANO,13},{20,16},
        {ANCHURA_ASTEROIDE_MEDIANO,24},{24,ALTURA_ASTEROIDE_MEDIANO},{20,27}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 10}, {10, 11}, {11, 0}
    },
    12,
    12
};

const struct DibujableConstante Asteroide_grande = {
    CENTRO_ASTEROIDE_MEDIANO,
    (struct Punto[]){
        {14,ALTURA_ASTEROIDE_GRANDE},{0,40},{0,14},{16,0},{26,15},{31,0},
        {ANCHURA_ASTEROIDE_GRANDE,14},{47,25},{ANCHURA_ASTEROIDE_GRANDE,40},
        {34,ALTURA_ASTEROIDE_GRANDE}
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 0}
    },
    10,
    10
};