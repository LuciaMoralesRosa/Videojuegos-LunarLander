#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <cstdlib>
#include <ctime>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;


// Variables globales
ULONG_PTR gdiplusToken;
Image* sprite;
float scale = 1;
float posX, posY, speedX, speedY;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float MIN_SPEED = 5.0f;
const float MAX_SPEED = 15.0f;

// Función para convertir de std::string a std::wstring
std::wstring StringToWString(const std::string& str) {
    return std::wstring(str.begin(), str.end());
}

// Función de procesamiento de eventos
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // Inicializar GDI+
            GdiplusStartupInput gdiplusStartupInput;
            GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
            
            // Cargar el sprite (imagen)
            sprite = new Image(StringToWString("sprite.png").c_str()); // Asegúrate de tener una imagen llamada sprite.png en el directorio correcto
            
            // Inicializar posiciones aleatorias
            srand((unsigned)time(0));
            posX = rand() % (WINDOW_WIDTH - sprite->GetWidth());
            posY = rand() % (WINDOW_HEIGHT - sprite->GetHeight());
            speedX = MIN_SPEED + (rand() % (int)(MAX_SPEED - MIN_SPEED));
            speedY = MIN_SPEED + (rand() % (int)(MAX_SPEED - MIN_SPEED));
            
            // Configurar el temporizador para el movimiento
            SetTimer(hwnd, 1, 16, NULL); // Aproximadamente 60 fps
            break;
        }
        
        case WM_KEYDOWN: {
            if (wParam == VK_RETURN) { // Tecla enter
                // Reubicar el sprite aleatoriamente cuando se presiona Enter
                posX = rand() % (WINDOW_WIDTH - sprite->GetWidth());
                posY = rand() % (WINDOW_HEIGHT - sprite->GetHeight());
                speedX = MIN_SPEED + (rand() % (int)(MAX_SPEED - MIN_SPEED));
                speedY = MIN_SPEED + (rand() % (int)(MAX_SPEED - MIN_SPEED));
            } else if (wParam == VK_UP) { // Tecla flecha-arriba
                if(scale <= 1.8){
                    scale += 0.2;
                }
            } else if (wParam == VK_DOWN) {
                if(scale > 0.4){
                    scale -= 0.2;
                }
            }
            break;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            Graphics graphics(hdc);

            // Dibujar la imagen (sprite) en la ventana
            graphics.DrawImage(sprite, (int)posX, (int)posY, (int)(sprite->GetWidth()*scale), (int)(sprite->GetHeight()*scale));

            EndPaint(hwnd, &ps);
            break;
        }

        case WM_TIMER:{

            int window_width = WINDOW_WIDTH;
            int window_height = WINDOW_HEIGHT;
     
            RECT rect;
            if(GetWindowRect(hwnd, &rect)){
                window_width = rect.right - rect.left;
                window_height = rect.bottom - rect.top;
            }
            // Actualizar la posición del sprite
            posX += speedX;
            posY += speedY;

            // Rebote en los bordes de la ventana
            if (posX <= 0 || posX + sprite->GetWidth() >= window_width) {
                speedX = -speedX;
            }
            if (posY <= 0 || posY + sprite->GetHeight() >= window_height) {
                speedY = -speedY;
            }

            // Solicitar la actualización de la ventana (redibujar)
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }

        case WM_DESTROY:
            // Limpiar recursos
            delete sprite;
            GdiplusShutdown(gdiplusToken);
            PostQuitMessage(0);
            break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Estructura para registrar la clase de ventana
    const wchar_t CLASS_NAME[] = L"SpriteWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    // Registrar la clase de ventana
    RegisterClass(&wc);

    // Crear la ventana
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Ventana con Sprite en Movimiento",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Bucle de mensajes de Windows
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
