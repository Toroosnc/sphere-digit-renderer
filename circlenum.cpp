#include <windows.h>
#include <GL/gl.h>
#include <cmath>
#include <vector>

#pragma comment(lib, "opengl32.lib")

float angleX = 0, angleY = 0;
const double PI = 3.14159265358;
GLuint base; 

void printText(float x, float y, float z, const char* text) {
    glRasterPos3f(x, y, z);
    glPushAttrib(GL_LIST_BIT);
    glListBase(base - 32);
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
    glPopAttrib();
}

void drawDigit(float x, float y, float z, int val, float brightness) {
    glColor3f(0.3f, 0.3f, 0.3f); 
    char numStr[2];
    itoa(val, numStr, 10); 
    printText(x, y, z, numStr);
}

void buildFont(HDC hDC) {
    base = glGenLists(96);
    HFONT font = CreateFont(-18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH, L"Courier New");
    SelectObject(hDC, font);
    wglUseFontBitmaps(hDC, 32, 96, base);
}

void display() {
    glViewport(0, 0, 1920, 1080); 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.77, 1.77, -1.0, 1.0, -5.0, 5.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);

    float radius = 0.7f;
    for (float phi = 0; phi < PI; phi += 0.12f) { 
        for (float theta = 0; theta < 2 * PI; theta += 0.12f) {
            float x = radius * sin(phi) * cos(theta);
            float y = radius * sin(phi) * sin(theta);
            float z = radius * cos(phi);

            float brightness = (z + radius) / (2.0f * radius);
            int val = (int)(abs(x + y + z) * 10) % 10;
            drawDigit(x, y, z, val, brightness);
        }
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CLOSE: PostQuitMessage(0); return 0;
        default: return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

int main() {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"SphereGL";
    RegisterClass(&wc);

    HWND hWnd = CreateWindow(L"SphereGL", L"OpenGL Sphere Digital", 
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 1920, 1080, NULL, NULL, wc.hInstance, NULL);

    HDC hDC = GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = {sizeof(pfd), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32};
    SetPixelFormat(hDC, ChoosePixelFormat(hDC, &pfd), &pfd);
    HGLRC hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);

    glEnable(GL_DEPTH_TEST);
    buildFont(hDC); 

    MSG msg = {0};
    while (msg.message != WM_QUIT) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            display();
            SwapBuffers(hDC);
            angleX += 0.3f; 
            angleY += 0.5f;
            Sleep(60); 
        }
    }
    return 0;
}
