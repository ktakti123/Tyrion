#include<windows.h>
#include <windowsx.h>
#include "windowhandling.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include "camera.h"
#include "graphics.h"

#define PI 3.14159265358979323846 
bool keyDown[256] = {0};
HWND gGameWindow;
RECT windowRect;
int windowWidth = 640;
int windowHeight = 580;
BOOL run = true;
BOOL GameStart = 1;
GameBitmap gDrawingSurface;
float mousekx = 0;
float mouseky = 0;



MSG Message = {0};

LRESULT CALLBACK MainWindowProc(HWND WindowHandle, UINT Message, WPARAM wParam, LPARAM lParam)
{
    static BOOL dragrotate = FALSE;    // rotation
    static BOOL dragpan = FALSE;     // pan
    static BOOL dragzoom = FALSE;    // zoom
    static int lastMouseX = 0;
    static int lastMouseY = 0;

    switch (Message)
    {
    case WM_CREATE:
        return 0;

    case WM_SIZE:
        checkWindowSize();
        return 0;

    case WM_KEYDOWN:
        keyDown[wParam & 0xFF] = true;
        switch (wParam)
        {
        case 'N': fov += 0.1f; break;
        case 'M': fov -= 0.1f; break;
        }
        return 0;

    case WM_KEYUP:
        keyDown[wParam & 0xFF] = false;
        return 0;

    case WM_LBUTTONDOWN:
    {
        BOOL altPressed = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
        BOOL shiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
        BOOL ctrlPressed = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;

        lastMouseX = GET_X_LPARAM(lParam);
        lastMouseY = GET_Y_LPARAM(lParam);

        // Determine drag mode
        dragrotate = dragpan = dragzoom = FALSE; // reset all

        if (altPressed && shiftPressed)
            dragpan = TRUE;
        else if (altPressed && ctrlPressed)
            dragzoom = TRUE;
        else if (altPressed)
            dragrotate = TRUE;
    }
        return 0;

    case WM_MOUSEMOVE:
    {
        BOOL leftPressed = (wParam & MK_LBUTTON) != 0;
        BOOL altPressed = (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
        BOOL shiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
        BOOL ctrlPressed = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;

        // Stop drag if button released or modifier keys no longer match mode
        if (dragrotate && (!leftPressed || altPressed == FALSE || (shiftPressed || ctrlPressed)))
            dragrotate = FALSE;
        if (dragpan && (!leftPressed || altPressed == FALSE || !shiftPressed))
            dragpan = FALSE;
        if (dragzoom && (!leftPressed || altPressed == FALSE || !ctrlPressed))
            dragzoom = FALSE;

        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);

        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;

        lastMouseX = x;
        lastMouseY = y;

        float sensitivity = 0.005f;

        if (dragrotate)
        {
            camerayaw += deltaX * sensitivity;
            camerapitch += deltaY * sensitivity;
        }
        else if (dragpan)
        {
            camerax += deltaX * sensitivity;
            cameray -= deltaY * sensitivity;
        }
        else if (dragzoom)
        {
            cameraz -= deltaY * sensitivity * 5.0f; // zoom factor
        }
    }
        return 0;

    case WM_LBUTTONUP:
        dragrotate = dragpan = dragzoom = FALSE;
        return 0;

    case WM_DESTROY:
        run = FALSE;
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(WindowHandle, Message, wParam, lParam);
    }

    return 0;
}



DWORD start(void){

    WNDCLASSEXA WindowClass = {0};
    WindowClass.cbSize   = sizeof(WNDCLASSEXA);
    WindowClass.lpfnWndProc   = MainWindowProc;
    WindowClass.hInstance     = GetModuleHandleA(NULL);
    WindowClass.lpszClassName = "OBJ_Loader";
    WindowClass.hbrBackground = CreateSolidBrush(RGB(45,0,0));
    
    RegisterClassExA(&WindowClass);

     gDrawingSurface.BitmapInfo.bmiHeader.biSize = sizeof(gDrawingSurface.BitmapInfo.bmiHeader);
     gDrawingSurface.BitmapInfo.bmiHeader.biWidth = GameResWidth;
     gDrawingSurface.BitmapInfo.bmiHeader.biHeight = -GameResHeight;
     gDrawingSurface.BitmapInfo.bmiHeader.biBitCount = GameBpp;
     gDrawingSurface.BitmapInfo.bmiHeader.biCompression = BI_RGB;
     gDrawingSurface.BitmapInfo.bmiHeader.biPlanes = 1;
     gDrawingSurface.memory = VirtualAlloc(NULL,GAME_MEMORY_SIZE,MEM_RESERVE|MEM_COMMIT , PAGE_READWRITE );
     memset(gDrawingSurface.memory, 0x00, GAME_MEMORY_SIZE); 
     gGameWindow   = CreateWindowExA(WS_EX_CLIENTEDGE,WindowClass.lpszClassName,"OBJ_LOADER",WS_OVERLAPPEDWINDOW|WS_VISIBLE,
     CW_USEDEFAULT, CW_USEDEFAULT,640,480,NULL,NULL,GetModuleHandleA(NULL), NULL);
    if (gGameWindow == NULL)
    {
    return (0);
    }
    
}
void checkWindowSize(){
 GetClientRect(gGameWindow, &windowRect);
 windowWidth = windowRect.right - windowRect.left;
 windowHeight = windowRect.bottom - windowRect.top;
}
void Render(void){
   if (GameStart ==1){
    checkWindowSize();
    GameStart = 0;
   }
    HDC DeviceContext = GetDC(gGameWindow);

    StretchDIBits(DeviceContext, 0, 0, windowWidth, windowHeight, 0, 0, GameResWidth, GameResHeight,
                  gDrawingSurface.memory, &gDrawingSurface.BitmapInfo, DIB_RGB_COLORS, SRCCOPY);

    // Release device context
    ReleaseDC(gGameWindow, DeviceContext);
    Sleep(1);
}
void Events(void){
    while (PeekMessageA(&Message, NULL, 0, 0, PM_REMOVE))
    {
        DispatchMessageA(&Message);
    }

    updatecamera();
    
}
void updatecamera(void){
    if (keyDown['W'])
     cameraz = cameraz + 0.01f;

    if (keyDown['S'])
       cameraz = cameraz - 0.01f;

    if (keyDown['A'])
    camerax = camerax + 0.01f;

    if (keyDown['D'])
    camerax = camerax - 0.01f;

     if (keyDown['O'])
     ortho = !ortho;
       
}



