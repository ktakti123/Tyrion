#include<windows.h>
#include "windowhandling.h"
#include "graphics.h"
#include "obj.h"
#include "camera.h"
#include <stdio.h>
#include<stdbool.h>
#include<math.h>




void graphics_fill_screen(BYTE red, BYTE green, BYTE blue){
    // Fill the memory with the desired color
    for (int y = 0; y < GameResHeight; ++y)
    {
        for (int x = 0; x < GameResWidth; ++x)
        {
            // Calculate the index for the pixel in the memory buffer
            int pixelIndex = (y * GameResWidth + x) * 4; // 4 bytes per pixel for 32 BPP
            BYTE *pixel = (BYTE *)gDrawingSurface.memory + pixelIndex;

            // Set the pixel color
            pixel[0] = blue;  // Blue component
            pixel[1] = green; // Green component
            pixel[2] = red;   // Red component
            pixel[3] = 0;     // Alpha component (not used)
        }
    }
}
void set_point(int x , int y, BYTE red, BYTE green, BYTE blue){
    if(x <GameResWidth && y < GameResHeight){
            int pixelIndex = (y * GameResWidth + x) * 4;
            BYTE *pixel = (BYTE *)gDrawingSurface.memory + pixelIndex;
            pixel[0] = blue;  // Blue component
            pixel[1] = green; // Green component
            pixel[2] = red;   // Red component
            pixel[3] = 0;     // Alpha component (not used)
    }
}
void draw_line(int x1 , int y1 , int x2 , int y2 ,byte r,byte g, byte b){
  int sx = x2-x1;
  int sy = y2-y1;
  int dx = abs(sx);
  int dy = abs(sy);
  int k ; 
  int xn = x1;
  int yn = y1;
 
  if (dx>=dy)
   k = 2 * sy - sx;
  else
  k = 2 * sx - sy;
  int dg = (dx>dy)? dx:dy;
  for (int i = 0 ; i <dg ; i++)
  {
   if (k>0){
      if(dx>=dy){
         k = k+ 2*dy - 2*dx;
      }
      else{
         k = k + 2*dx - 2*dy;
      }
      if (sx < 0)
      xn = xn-1;
      else
      xn = xn+1;

      if(sy<0)
      yn = yn-1;
      else
      yn = yn+1;
      set_point(xn,yn,r,g,b);

   }
   else{
      if (dx>=dy)
      k = k+2*dy;
      else
      k = k + 2 *dx;
      if (dx>=dy){
         if (sx>=0)
            xn = xn+1;
         else
            xn = xn-1;
         }
         else{
            if (sy>=0)
            yn = yn+1;
            else
            yn = yn-1;
         }
         set_point(xn,yn,r,g,b);
      }

   }
}
void draw_triangle (int x0,int y0,int x1,int y1,int x2 , int y2 , byte r , byte g ,byte b){
     if ((x0 == x1 && y0 == y1) || (x1 == x2 && y1 == y2) || (x0 == x2 && y0 == y2)) 
    {
        return;
    }
    if (x0 < 0 || x0 >= GameResWidth ||x1 < 0 || x1 >= GameResWidth ||x2 < 0 || x2 >= GameResWidth ||
      y0 < 0 || y0 >= GameResHeight ||y1 < 0 || y1 >= GameResHeight ||y2 < 0 || y2 >= GameResHeight) 
     {
    return;
     }
    int area = (x1 - x0) * (y2 - y0) - (x2 - x0) * (y1 - y0);
    if (area == 0) {
        return;
    }
    
    if(y0>y1){swap(x0,x1); swap(y0,y1);}
    if(y1>y2){swap(x1,x2); swap(y1,y2);}
    if(y0>y1){swap(x0,x1); swap(y0,y1);}

    
     int loop;
     int x = x0;
     int dx = x1-x0;
     int dy = y1-y0;
     int abs_dx = abs(dx);
     int abs_dy = abs(dy);
     int step;

     int lx = x0;
     int ldx = x2-x0;
     int ldy = y2-y0;
     int labs_dx = abs(ldx);
     int labs_dy = abs(ldy);
     int lstep;

     if(y0==y1){
        x = x0;
        dx = x2-x0;
        dy = y2-y0;
        abs_dx = abs(dx);
        abs_dy = abs(dy);


        lx = x1;
        ldx = x2-x1;
        ldy = y2-y1;
        labs_dx = abs(ldx);
        labs_dy = abs(ldy);


     }

      if (ldx>0)
        lstep = 1;
     else
        lstep = -1;


     
     if (dx>0)
        step = 1;
     else
        step = -1;
    int lerror = labs_dx/2;    
    int error = abs_dx/2;

    if(y0 != y1 && y1 != y2 && y0 != y2)
        loop = y1;
    else
        loop = y2;

    for (int y = y0; y <= loop; y++)
    {
        int min = x , max = lx;
        if(min>max){swap(min,max)};
        for(int d = min; d <= max; d++){
            set_point(d,y,r,g,b);
        }
        error = error + abs_dx;
        lerror = lerror + labs_dx;

        while(error>= abs_dy){
            x=x+step;
            error = error-abs_dy;
        }
         while(lerror>= labs_dy){
            lx=lx+lstep;
            lerror = lerror-labs_dy;
        }
    }
    if(y0 != y1 && y1 != y2 && y0 != y2){
     draw_triangle(x1,y1,lx,y1,x2, y2 ,r,g,b);
    }
}
