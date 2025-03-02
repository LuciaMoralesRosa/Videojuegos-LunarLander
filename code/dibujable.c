#include "dibujable.h"


struct Dibujable* crearDibujable(const struct DibujableConstante* constante) {
    // Reservar memoria para el Dibujable
    struct Dibujable* dibujable = (struct Dibujable*)malloc(sizeof(struct Dibujable));
    if (!dibujable) return NULL;  // Si falla malloc(), retorna NULL

    dibujable->origen = constante->origen;
    dibujable->num_puntos = constante->num_puntos;
    dibujable->num_aristas = constante->num_aristas;

    // Reservar memoria y copiar los puntos
    dibujable->puntos = (struct Punto*)malloc(constante->num_puntos * sizeof(struct Punto));
    if (!dibujable->puntos) {
        free(dibujable);
        return NULL;
    }
    memcpy(dibujable->puntos, constante->puntos, constante->num_puntos * sizeof(struct Punto));  

    // Reservar memoria y copiar las aristas
    dibujable->aristas = (struct Arista*)malloc(constante->num_aristas * sizeof(struct Arista));
    if (!dibujable->aristas) {
        free(dibujable->puntos);
        free(dibujable);
        return NULL;
    }
    for (uint8_t i = 0; i < constante->num_aristas; i++) {
        dibujable->aristas[i].origen = &dibujable->puntos[constante->unionAristas[i].origen];
        dibujable->aristas[i].destino = &dibujable->puntos[constante->unionAristas[i].destino];
    }
    return dibujable;
}

void destruirDibujable(struct Dibujable* dibujable) {
    if(dibujable == NULL) return;
    if(dibujable -> puntos != NULL){
        free(dibujable -> puntos);
    }
    if(dibujable -> aristas != NULL) {
        free(dibujable -> aristas);
    }
    free(dibujable);
}

void destruirArista(struct Arista* arista){
    if(arista == NULL) return;
    if(arista -> origen != NULL){
        free(arista -> origen);
    }
    if(arista -> destino != NULL){
        free(arista -> destino);
    }
    free(arista);
}


// Algoritmo de Bresenham para rasterizar una línea
void DrawLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        SetPixel(hdc, x1, y1, color); // Dibuja el pixel actual

        if (x1 == x2 && y1 == y2) break; // Si llegamos al final, salimos

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}



void dibujarDibujable(HDC hdc, const struct Dibujable* dibujable){
    for(uint8_t i = 0; i < dibujable->num_aristas; i++) {
        DrawLine(hdc, dibujable->aristas[i].origen->x,
            dibujable->aristas[i].origen->y,
            dibujable->aristas[i].destino->x,
            dibujable->aristas[i].destino->y,
            RGB(255, 255, 255));
    }
}