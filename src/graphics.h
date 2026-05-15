 #ifndef GRAPHICS_H
#define GRAPHICS_H
#include <windows.h>
#include<stdbool.h> 

#define swap(a,b){int temp=a;a=b;b=temp;}
#define color_black 0,0,0
#define color_red 255,0,0
#define color_blue 0,0,255
#define color_green 0,255,0
#define color_white 255,255,255



void graphics_fill_screen(BYTE , BYTE , BYTE );
void set_point(int , int , BYTE , BYTE , BYTE );
void draw_line(int , int , int , int, byte,byte,byte);
void draw_triangle(int,int,int,int,int,int,byte,byte,byte);

 #endif 