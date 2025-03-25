/**
 * @file dibujable.h
 * @brief Estructuras y funciones para manejar objetos dibujables
 * 
 * Este archivo contiene las estructuras y funciones necesarias para manejar
 * objetos dibujables en un plano cartesiano.
 */

#ifndef DIBUJABLE_H
#define DIBUJABLE_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <wtypes.h>

/** 
 * @brief Estructura que representa un punto en el plano cartesiano
 * 
 * @param x Coordenada x del punto
 * @param y Coordenada y del punto
 */
struct Punto{
    float x, y;
};

/**
 * @brief Estructura que representa una union entre dos puntos
 * 
 * @param origen Indice del punto de origen
 * @param destino Indice del punto de destino
 */
struct UnionAristas{
    uint8_t origen;
    uint8_t destino;
};

/**
 * @brief Estructura que representa una arista
 * 
 * @param origen Punto de origen de la arista
 * @param destino Punto de destino de la arista
 */
struct Arista{
    struct Punto* origen;
    struct Punto* destino;
};

/**
 * @brief Estructura que representa un objeto dibujable
 * 
 * @param origen Punto de origen del objeto
 * @param puntos Puntos que conforman el objeto
 * @param aristas Aristas que conforman el objeto
 */
struct Dibujable {
    struct Punto origen;
    struct Punto* puntos; // Arreglo dinamico de puntos
    struct Arista* aristas; // Arreglo dinamico de aristas
    uint8_t num_puntos;
    uint8_t num_aristas;
};

/**
 * @brief Estructura que representa un objeto dibujable constante
 * 
 * @param origen Punto de origen del objeto
 * @param puntos Puntos que conforman el objeto
 * @param unionAristas Uniones entre los puntos que conforman las aristas
 * @param num_puntos Numero de puntos que conforman el objeto
 * @param num_aristas Numero de aristas que conforman el objeto 
 */
struct DibujableConstante {
    struct Punto origen; // Esquina derecha superior
    struct Punto* puntos; 
    struct UnionAristas* unionAristas;
    uint8_t num_puntos;
    uint8_t num_aristas;
};

/**
 * @brief Crea un objeto dibujable
 * 
 * @param constante Puntero a la estructura que contiene los datos del objeto dibujable
 * 
 * @return struct Dibujable* Puntero al objeto dibujable creado
 */
struct Dibujable* crearDibujable(const struct DibujableConstante* constante);


/**
 * @brief Libera la memoria ocupada por un objeto Dibujable
 * 
 * @param dibujable Puntero al objeto dibujable a liberar
 */
void destruirDibujable(struct Dibujable* dibujable);

void destruirArista(struct Arista* arista);

/**
 * @brief Dibuja por pantalla un dibujable
 *
 * @param dibujable Dibujable a pintar por pantalla
 */
void dibujarDibujable(HDC hdc, const struct Dibujable* dibujable);

#endif // DIBUJABLE_H
