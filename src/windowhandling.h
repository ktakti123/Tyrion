#ifndef WINDOWCREATE_H
#define WINDOWCREATE_H
#include <stdbool.h>

#include <windows.h>

typedef struct GameBitmap {
    BITMAPINFO BitmapInfo;
    void *memory;
} GameBitmap;


#define GameResWidth 300
#define GameResHeight 300
#define GameBpp 32
#define GAME_MEMORY_SIZE (GameResWidth*GameResHeight * (GameBpp/8))
#define main() WINAPI WinMain(HINSTANCE Instance, HINSTANCE PreviousInstance, LPSTR CommandLine, int CmdShow)

extern HWND gGameWindow;
extern GameBitmap gDrawingSurface;
extern RECT windowRect;
extern int windowWidth;
extern int windowHeight;
extern BOOL run;
extern BOOL GameStart;
extern MSG Message;
extern float mousekx;
extern float mouseky;
extern bool keyDown[256];

// Function
DWORD start(void);
void checkWindowSize(void);
void Render(void);
void Events(void);
void updatecamera(void);

#endif 
