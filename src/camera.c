#include <windows.h>
#include "camera.h"
#include "obj.h"
#include "windowhandling.h"
#include <stdbool.h>
#include <stdio.h>

 bool keyDown[256];
Camera cameradefault[2] = {
    { 0, 0, 1, 0, 0, 1.3 },
    {  0, 0, 1, 0, 0, 1.3 }
};
float panx=0,pany=0,camerax,cameray,cameraz,camerapitch,camerayaw,camerafov;
float camerazoom = 1,fx,fy,fz,px,py,pz;

void camera(int cam){
    camerax = cameradefault[cam].x;
    cameray = cameradefault[cam].y;
    cameraz = cameradefault[cam].z;
    camerapitch = cameradefault[cam].pitch;
    camerayaw = cameradefault[cam].yaw;
    camerafov = cameradefault[cam].fov;
	objtocamera();
}



