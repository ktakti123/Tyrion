#include <windows.h>
#include "camera.h"
#include "windowhandling.h"
#include <stdbool.h>
#include <stdio.h>

 bool keyDown[256];
Camera cameradefault[2] = {
    { 0, 0, 4, 0, 0, 11.3 },
    {  0, 0, 4, 0, 0, 11.3 }
};
float camerax,cameray,cameraz,camerapitch,camerayaw,camerafov;

void camera(int cam){
    camerax = cameradefault[cam].x;
    cameray = cameradefault[cam].y;
    cameraz = cameradefault[cam].z;
    camerapitch = cameradefault[cam].pitch;
    camerayaw = cameradefault[cam].yaw;
    camerafov = cameradefault[cam].fov;
}



