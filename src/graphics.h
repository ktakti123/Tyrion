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

extern bool ortho;
extern float fov;

// structures
  typedef struct {
    float x , y , z;
  }Vertex;
  typedef struct {
    float x , y , z;
  }Normal;
  typedef struct{
    int v1 , v2 , v3;
    int nx , ny , nz;
  }Triangleindex;

  typedef struct{
    float x1,y1,z1;
    float x2,y2,z2;
    float x3,y3,z3;
    float nx3d,ny3d,nz3d;
  }Trianglebuffer;




void fill_screen(BYTE , BYTE , BYTE );
void set_point(int , int , BYTE , BYTE , BYTE );
void draw_line(int , int , int , int, byte,byte,byte);
void draw_triangle(int,int,int,int,int,int,byte,byte,byte);
void loadobj();
void drawobj();
void draw_obj();
void sort_obj();
void rotate_obj();
void objtocamera();
void cnnormalize(float,float,float,float,float,float,byte);
void rotate_translate_axis(int,float,float,float,float,float,float,float,float,float,float,float,float);

 #endif 