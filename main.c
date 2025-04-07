#include "code/lunar_lander.h"
#include "code/palabra.h"
#include "resources/superficie_lunar.h"
#include "code/gestor_plataformas.h"
#include "code/variables_globales.h"
#include "resources/torreta.h"

#include <stdio.h>
#include <windows.h>

#define timer 1
#define tamano_inicial_pantalla_X 1024
#define tamano_inicial_pantalla_Y 768
#define anchura_minima_ventana 512
#define altura_minima_ventana 384

// Factor por el que escalar la escena
float factor_escalado = 1.0f; 

// 1 si la ventana esta en fullcreen, 0 si no (sin bordes ni cabecera)
uint8_t fullscreen = 0;
uint8_t esc_presionado = 0;
// Rectangulo que contiene la ventana anterior al resize de pantalla completa
RECT rectVentanaAnterior;

// Moneda presionada
uint8_t moneda_presionada = 0;

struct Punto* p1 = NULL;
struct Punto* p2 = NULL;
struct Punto* p3 = NULL;
struct Punto* p4 = NULL;

float minimo(float a, float b) {
    return (a < b) ? a : b;
}

void AttachConsoleToStdout() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);  // Redirige stdout a la consola
    freopen("CONOUT$", "w", stderr);  // Redirige stderr también
}

/**
 * @brief Funcion para realizar pruebas de dibujo de las naves
 * 
 * @param hdc
 */
void pruebasDibujables(HDC hdc){

    struct Dibujable* base = crearDibujable(&Torreta_base);
    colocar_dibujable(base, (struct Punto){50, 50});
    dibujarDibujable(hdc, base);

    struct Dibujable* dcha = crearDibujable(&Torreta_dcha);
    escalar_dibujable_en_escena_dados_ejes(dcha, 5, 5);
    colocar_dibujable(dcha, (struct Punto){100, 100});
    dibujarDibujable(hdc, dcha);

    struct Dibujable* i = crearDibujable(&Torreta_izda);
    escalar_dibujable_en_escena_dados_ejes(i, 5, 5);
    colocar_dibujable(i, (struct Punto){150, 150});
    dibujarDibujable(hdc, i);
}


void inicializar_puntos() {
    // Usar malloc para asignar memoria dinámica para las estructuras
    p1 = (struct Punto*)malloc(sizeof(struct Punto));
    p2 = (struct Punto*)malloc(sizeof(struct Punto));
    p3 = (struct Punto*)malloc(sizeof(struct Punto));
    p4 = (struct Punto*)malloc(sizeof(struct Punto));
}


// Algoritmo de Bresenham para rasterizar una línea
void dibujar_linea(HDC hdc, int x1, int y1, int x2, int y2, COLORREF color) {
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

void dibujar_bordes(HDC hdc) {
    if(p1 == NULL || p2 == NULL || p3 == NULL || p4 == NULL) {
        return;
    }
    dibujar_linea(hdc, p1->x, p1->y, p2->x, p2->y, RGB(255, 255, 255));
    dibujar_linea(hdc, p3->x, p3->y, p4->x, p4->y, RGB(255, 255, 255));
}


/**
 * @brief Escala la escena al tamaño de la ventana
 * 
 * @param hwnd Ventana a escalar
 */
void escalar(HWND hwnd) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    int ancho_cliente = rect.right - rect.left;
    int alto_cliente = rect.bottom - rect.top;
    float factor_resized_x = (float)ancho_cliente / tamano_inicial_pantalla_X;
    float factor_resized_y = (float)alto_cliente / tamano_inicial_pantalla_Y;

    escalar_escena(1/factor_escalado, 1/factor_escalado);
    factor_escalado = minimo(factor_resized_x, factor_resized_y);

    escalar_escena(factor_escalado, factor_escalado);

    int tam_escena_x = (int)(tamano_inicial_pantalla_X * factor_escalado);
    int tam_escena_y = (int)(tamano_inicial_pantalla_Y * factor_escalado);


    if (!p1 || !p2 || !p3 || !p4) {
        // Manejo de errores si malloc falla
        printf("Error al asignar memoria.\n");
        return; // Salir o manejar el error
    }

    // Inicializar los puntos
    *p1 = (struct Punto){0, tam_escena_y + 1};
    *p2 = (struct Punto){tam_escena_x + 1, tam_escena_y + 1};
    *p3 = (struct Punto){tam_escena_x + 1, 0};
    *p4 = (struct Punto){tam_escena_x + 1, tam_escena_y + 1};
}

// Función de ventana
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:{
            SetTimer(hwnd, timer, intervalo_fisicas_ms, NULL);
        }
        break;

        case WM_SYSCOMMAND: {
            if((wParam & 0xFFF0) == SC_RESTORE){
                if(fullscreen == 1 && esc_presionado == 1) {
                    // Si se ha pulsado ESC y se estaba en fullscreen -> se restaura la ventana
                    fullscreen = 0;
                    // Restaurar estilo de ventana
                    SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
                    SetWindowPos(
                        hwnd, NULL,
                        rectVentanaAnterior.left,
                        rectVentanaAnterior.top,
                        rectVentanaAnterior.right - rectVentanaAnterior.left,
                        rectVentanaAnterior.bottom - rectVentanaAnterior.top,
                        SWP_NOZORDER | SWP_FRAMECHANGED // Necesario para aplicar los bordes
                    );
                } else if (esc_presionado == 1){
                    // Si se ha pulsado ESC pero no se estaba en fullscreen -> no se propaga el restore
                    esc_presionado = 0;
                    return 0;
                }
                esc_presionado = 0;
            } else if ((wParam & 0xFFF0) == SC_MAXIMIZE) {
                fullscreen = 1;
                GetWindowRect(hwnd, &rectVentanaAnterior);
                // Escalar y quitar bordes y cabecera
                SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
                escalar(hwnd);
            }
        }
        break;

        case WM_GETMINMAXINFO: {
            // Establecer tamaño minimo del area del cliente
            MINMAXINFO* mmi = (MINMAXINFO*)lParam;
            RECT rc = {0, 0, anchura_minima_ventana, altura_minima_ventana};
            AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
            mmi->ptMinTrackSize.x = rc.right - rc.left;
            mmi->ptMinTrackSize.y = rc.bottom - rc.top;
        }
        break;

        case WM_SIZE: {
            // La ventana ha cambiado de tamaño
            escalar(hwnd);
        }
        break;

        case WM_TIMER:{
            if (wParam == timer) {
                manejar_instante();
                manejar_teclas();
                InvalidateRect(hwnd, NULL, FALSE); // Fuerza un repintado 
            }
        }
        break;

		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
		
			// Crear un buffer en memoria
			HDC hdcMem = CreateCompatibleDC(hdc);
			RECT rect;
			GetClientRect(hwnd, &rect);
			HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
			HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
			HGDIOBJ hOld = SelectObject(hdcMem, hbmMem);
		
			// Limpiar el buffer (pintarlo de negro)
			FillRect(hdcMem, &rect, brush);
			DeleteObject(brush);
		
			// Dibujar en el buffer en memoria
            dibujar_bordes(hdc);
			pruebasDibujables(hdcMem);
			pintar_pantalla(hdcMem);
		
			// Copiar el buffer en la ventana
			BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);
		
			// Liberar recursos
			SelectObject(hdcMem, hOld);
			DeleteObject(hbmMem);
			DeleteDC(hdcMem);
		
			EndPaint(hwnd, &ps);
		}
		break;

        case WM_KEYDOWN: {
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                esc_presionado = 1;
                SendMessage(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
            }
            if (GetAsyncKeyState(VK_UP) & 0x8000) pulsar_tecla(ARRIBA);
            if (GetAsyncKeyState(VK_LEFT) & 0x8000) pulsar_tecla(IZQUIERDA);
            if (GetAsyncKeyState(VK_RIGHT) & 0x8000) pulsar_tecla(DERECHA);
            if (GetAsyncKeyState(VK_SPACE) & 0x8000) pulsar_tecla(ESPACIO);
            if (GetAsyncKeyState(0x35) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000) pulsar_tecla(MONEDA);
        }
		break;

        case WM_KEYUP: {
            if (!(GetAsyncKeyState(VK_UP) & 0x8000)) levantar_tecla(ARRIBA);
            if (!(GetAsyncKeyState(VK_LEFT) & 0x8000)) levantar_tecla(IZQUIERDA);
            if (!(GetAsyncKeyState(VK_RIGHT) & 0x8000)) levantar_tecla(DERECHA);
            if (!(GetAsyncKeyState(VK_SPACE) & 0x8000)) levantar_tecla(ESPACIO);
            if (!(GetAsyncKeyState(0x35) & 0x8000 || GetAsyncKeyState(VK_NUMPAD5) & 0x8000)) levantar_tecla(MONEDA);
        }
		break;
        
        case WM_DESTROY:{
            KillTimer(hwnd, timer);
            PostQuitMessage(0);
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Función principal
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    AttachConsoleToStdout(); // Activa la consola para ver printf
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "RasterWindow";

    // Fondo negro
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

    RegisterClass(&wc);

    // Creacion de la ventana
    RECT rc = {0, 0, tamano_inicial_pantalla_X, tamano_inicial_pantalla_Y};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    HWND hwnd = CreateWindowEx(0, "RasterWindow", "Lunar Lander",
                            WS_OVERLAPPEDWINDOW, 0, 0,
                            (rc.right - rc.left), (rc.bottom - rc.top), NULL,
                            NULL, hInstance, NULL);
    inicializar_puntos();
    inicializar_aleatoriedad(); // Inicializar rand

    if (!hwnd) return 0;
    ShowWindow(hwnd, nCmdShow);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
