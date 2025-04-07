
#include "torreta.h"

const struct DibujableConstante Torreta_base = {
    CENTRO_TORRETA,
    (struct Punto[]){
        {0, ALTURA_TORRETA}, {2, 6}, {8, 6}, {ANCHURA_TORRETA, ALTURA_TORRETA}, // Base
        {4, 6}, {6, 6}, // Base cañon
        {4, 0}, {6, 0}  // Punta cañon
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 4}, {4, 5}, {5, 2}, {2, 3}, {3, 0}, // Base
        {4, 6}, {6, 7}, {7, 5} // Cañon
    },
    8,
    9
};

const struct DibujableConstante Torreta_izda = {
    CENTRO_TORRETA,
    (struct Punto[]){
        {0, ALTURA_TORRETA}, {2, 6}, {8, 6}, {ANCHURA_TORRETA, ALTURA_TORRETA}, // Base
        {4, 6}, {6, 6}, // Base cañon
        {1, 3}, {2, 1}  // Punta cañon
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 4}, {4, 5}, {5, 2}, {2, 3}, {3, 0}, // Base
        {4, 6}, {6, 7}, {7, 5} // Cañon
    },
    8,
    9
};

const struct DibujableConstante Torreta_dcha = {
    CENTRO_TORRETA,
    (struct Punto[]){
        {0, ALTURA_TORRETA}, {2, 6}, {8, 6}, {ANCHURA_TORRETA, ALTURA_TORRETA}, // Base
        {4, 6}, {6, 6}, // Base cañon
        {7, 1}, {8, 3}  // Punta cañon
    },
    (struct UnionAristas[]){
        {0, 1}, {1, 4}, {4, 5}, {5, 2}, {2, 3}, {3, 0}, // Base
        {4, 6}, {6, 7}, {7, 5} // Cañon
    },
    8,
    9
};