#include <windows.h>
#include <GL/gl.h>
#include <cmath>
#include <vector>
#include <time.h>

#pragma comment(lib, "opengl32.lib")

float angleX = 0, angleY = 0;
const double PI = 3.14159265358;
GLuint base; 
float colorTime = 0.0f;

enum RenderMode {
    MODE_EMPTY,  
    MODE_SPHERE, 
    MODE_CUBE    
};

RenderMode currentMode = MODE_EMPTY; 

void printText(float x, float y, float z, const char* text) {
    glRasterPos3f(x, y, z);
    glPushAttrib(GL_LIST_BIT);
    glListBase(base - 32);
    glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
    glPopAttrib();
}

void drawDigit(float x, float y, float z, int val) {
    glColor3f(0.0f, 0.545f, 0.545f); // Warna Teal
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

void renderSphere() {
    float radius = 0.6f;
    for (float phi = 0; phi < PI; phi += 0.12f) { 
        for (float theta = 0; theta < 2 * PI; theta += 0.12f) {
            float x = radius * sin(phi) * cos(theta);
            float y = radius * sin(phi) * sin(theta);
            float z = radius * cos(phi);

            int val = (int)(abs(x + y + z) * 10) % 10;
            drawDigit(x, y, z, val);
        }
    }
}

void renderCube() {
    float size = 0.5f; 
    float step = 0.12f; 

    for (float i = -size; i <= size + 0.01f; i += step) {
        for (float j = -size; j <= size + 0.01f; j += step) {
            drawDigit(i, j, size, (int)(abs(i + j + size) * 10) % 10);
            drawDigit(i, j, -size, (int)(abs(i + j - size) * 10) % 10);
            drawDigit(i, size, j, (int)(abs(i + size + j) * 10) % 10);
            drawDigit(i, -size, j, (int)(abs(i - size + j) * 10) % 10);
            drawDigit(size, i, j, (int)(abs(size + i + j) * 10) % 10);
            drawDigit(-size, i, j, (int)(abs(-size + i + j) * 10) % 10);
        }
    }
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

    float maxGray = 0.3f;
    float intensity = ((sin(colorTime) + 0.0f) / 2.0f) * maxGray;
    glClearColor(intensity, intensity, intensity, 1.0f);
    colorTime += 0.01f;

    if (currentMode == MODE_SPHERE) {
        renderSphere();
    } 
    else if (currentMode == MODE_CUBE) {
        renderCube();
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_KEYDOWN:
            if (wParam == 'C') {           
                currentMode = MODE_SPHERE; 
            } 
            else if (wParam == 'S') {      
                currentMode = MODE_CUBE;   
            }
            else if (wParam == VK_SPACE) { 
                currentMode = MODE_EMPTY;  
            }
            return 0;
            
        case WM_CLOSE: 
            PostQuitMessage(0); 
            return 0;
            
        default: 
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

int main() {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = L"ShapeGL";
    RegisterClass(&wc);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    HWND hWnd = CreateWindow(L"ShapeGL", L"OpenGL Digit Renderer C/S/Space", 
        WS_POPUP | WS_VISIBLE, 0, 0, screenWidth, screenHeight, NULL, NULL, wc.hInstance, NULL);

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
            Sleep(1); 
        }
    }
    return 0;
}