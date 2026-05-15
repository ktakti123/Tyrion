 #ifndef OBJ_H
#define OBJ_H
#include <windows.h>
#include<stdbool.h> 

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

void loadobj();
void drawobj();
void draw_obj();
void sort_obj();
void rotate_obj();
void objtocamera();
void cnnormalize(float,float,float,float,float,float,byte);
void rotate_translate_axis(int,float,float,float,float,float,float,float,float,float,float,float,float);

 #endif 