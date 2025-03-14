
#ifndef TRANSFORMACIONES_H
#define TRANSFORMACIONES_H

#define ANGULO_ROTACION 45 // 15, 30 o 45

#include <stdio.h>
#include <math.h>

#include "dibujable.h"
#include "palabra.h"

/**
 * @brief Traslada un dibujable en la dirección indicada
 * 
 * @param dibujable Dibujable a trasladar
 * @param traslacion Punto de traslación
 */
void trasladarDibujable(struct Dibujable* dibujable, struct Punto traslacion);

void colocar_dibujable(struct Dibujable* dibujable, struct Punto destino);

void colocar_palabra(struct Palabra* palabra, struct Punto destio);

/**
 * @brief Coloca un dibujable en un punto dado
 * 
 * @param dibujable Dibujable a trasladar
 * @param destino Punto de destino
 */
void colocarDibujable(struct Dibujable* dibujable, struct Punto destino);

/**
 * @brief Rota un dibujable en sentido horario o antihorario
 * 
 * @param dibujable Dibujable a rotar
 * @param direccion 0 para izquierda, 1 para derecha
 */
void rotarDibujable(struct Dibujable* dibujable, unsigned char direccion);

/**
 * @brief Escala un dibujable en el eje x e y
 * 
 * @param dibujable Dibujable a escalar
 * @param factor Factor de escala
 */
void escalarDibujable(struct Dibujable* dibujable, float factor);

/**
 * @brief Escala una palabra en el eje x e y
 * 
 * @param palabra Palabra a escalar
 * @param factor Factor de escala
 */
void escalar_palabra_centrada(struct Palabra* palabra, float factor);

/**
 * @brief Escala un dibujable en el eje x y el eje y dados los factores de
 *        escalado de cada eje
 * 
 * @param dibujable Dibujable a escalar
 * @param factorX Factor de escala en el eje x
 * @param factorY Factor de escala en el eje y
 */
void escalarDibujableDadosEjes(struct Dibujable* dibujable, float factorX, float factorY);

/**
 * @brief Escala una palabra en el eje x y el eje y dados los factores de
 *        escalado de cada eje
 * 
 * @param palabra palabra a escalar
 * @param factorX Factor de escala en el eje x
 * @param factorY Factor de escala en el eje y
 */
void escalar_palabra_centrada_dados_ejes(struct Palabra* palabra, float factorX, float factorY);

#endif // TRANSFORMACIONES_H