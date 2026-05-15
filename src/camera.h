#include <stdbool.h>
void camera(int);
void updatecamera(void);

typedef struct{
    float x,y,z;
    float pitch,yaw,fov;
  }Camera;

  extern Camera cameradefault[2];
  extern float camerax,cameray,cameraz,camerapitch,camerayaw,camerafov,camerazoom,fx,fy,fz;