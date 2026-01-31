#include<windows.h>
#include "windowhandling.h"
#include "graphics.h"
#include "camera.h"
#include <stdio.h>
#include<stdbool.h>
#include<math.h>

Trianglebuffer* triangle3d_buffer = NULL;
Trianglebuffer* camerabuffer = NULL;
Vertex* vertex_buffer = NULL;
Triangleindex* triangle_buffer = NULL;
Normal* normal_buffer = NULL;
int vcount = 0 , tcount = 0 , ncount = 0;
bool ortho = false;
float fov = 6;


void fill_screen(BYTE red, BYTE green, BYTE blue){
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
void openfile(){
    FILE* file = fopen("deer.obj","r");
    char line[128];
    while(fgets(line,sizeof(line),file)){
        if(line[0] == 'v' && line[1]== ' ') vcount++;
        if(line[0] == 'f') tcount++;
        if(line[0] == 'v' && line[1]== 'n') ncount++;
    }
    triangle_buffer = malloc(sizeof(Triangleindex)*tcount);
    vertex_buffer = malloc(sizeof(Vertex)*vcount);
    normal_buffer = malloc(sizeof(Normal)*ncount);
    fclose(file);
    file = fopen("deer.obj","r");
    int vcounter = 0,tcounter = 0,ncounter = 0;
    while(fgets(line,sizeof(line),file)){
          if(line[0] == 'v'&& line[1]==' '){
            sscanf(line,"v %f %f %f",&vertex_buffer[vcounter].x ,&vertex_buffer[vcounter].y,&vertex_buffer[vcounter].z);
            vcounter++;
          }
          if(line[0] == 'v'&& line[1]=='n'){
            sscanf(line,"vn %f %f %f",&normal_buffer[ncounter].x ,&normal_buffer[ncounter].y,&normal_buffer[ncounter].z);
            ncounter++;
          }
          if(line[0] == 'f'){
             sscanf(line,"f %d//%d %d//%d %d//%d",&triangle_buffer[tcounter].v1 , &triangle_buffer[tcounter].nx,
                    &triangle_buffer[tcounter].v2,&triangle_buffer[tcounter].ny,
                    &triangle_buffer[tcounter].v3,&triangle_buffer[tcounter].nz);
            
            tcounter++;
          }
    }
    fclose(file);
}
void loadobj(){
    openfile();
    triangle3d_buffer = malloc(sizeof(Trianglebuffer)* tcount);
    camerabuffer = malloc(sizeof(Trianglebuffer)* tcount);
    for (int i=0; i < tcount ; i++){
      Triangleindex x = triangle_buffer[i];
      Vertex vx1 = vertex_buffer[x.v1 - 1];
      Vertex vx2 = vertex_buffer[x.v2 - 1];
      Vertex vx3 = vertex_buffer[x.v3 - 1];
      Normal nn = normal_buffer[x.nx-1];

      triangle3d_buffer[i].x1 = vx1.x;
      triangle3d_buffer[i].y1 = vx1.y;
      triangle3d_buffer[i].z1 = vx1.z;
      triangle3d_buffer[i].nx3d = nn.x;


      triangle3d_buffer[i].x2 = vx2.x;
      triangle3d_buffer[i].y2 = vx2.y;
      triangle3d_buffer[i].z2 = vx2.z;
      triangle3d_buffer[i].ny3d = nn.y;

      
      triangle3d_buffer[i].x3 = vx3.x;
      triangle3d_buffer[i].y3 = vx3.y;
      triangle3d_buffer[i].z3 = vx3.z;
      triangle3d_buffer[i].nz3d = nn.z;}
}

void sort_obj(){
  for (int i=0; i<tcount-1; i++){
      for(int j=i+1; j<tcount; j++){
        float sum1 = camerabuffer[i].z1+camerabuffer[i].z2+camerabuffer[i].z3;
        float sum2 = camerabuffer[j].z1+camerabuffer[j].z2+camerabuffer[j].z3;

        if(sum1>sum2){
          Trianglebuffer temp = camerabuffer[i];
          camerabuffer[i] = camerabuffer[j];
          camerabuffer[j] = temp;
        }
      }
    }
}

void drawobj(){
   for (int i = 0; i < tcount; i++){

  float dp=(camerabuffer[i].nx3d * 0.4718) + (camerabuffer[i].ny3d * -0.6891) + (camerabuffer[i].nz3d * 0.5500);
  int cp = (int)(((dp+1.0f)/2.0f)*255.0f);


    cnnormalize(
        camerabuffer[i].x1,
        camerabuffer[i].y1 ,
        camerabuffer[i].x2,
        camerabuffer[i].y2,
        camerabuffer[i].x3,
        camerabuffer[i].y3,
        cp);
    }
}


void cnnormalize(float x0,float y0,float x1,float y1,float x2 , float y2 , byte cp){ 
      x0 = ((x0+1.0f) * 0.5f)*GameResWidth;
      x1 = ((x1+1.0f) * 0.5f)*GameResWidth;
      x2 = ((x2+1.0f) * 0.5f)*GameResWidth;
      y0 = (1.0f - ((y0 + 1.0f) * 0.5f)) * GameResHeight;
      y1 = (1.0f - ((y1 + 1.0f) * 0.5f)) * GameResHeight;
      y2 = (1.0f - ((y2 + 1.0f) * 0.5f)) * GameResHeight;
      
      draw_triangle(x0,y0,x1,y1,x2,y2,cp,cp,cp);

}

void objtocamera(){
    
 for (int i = 0; i < tcount; i++){
 rotate_translate_axis(i,triangle3d_buffer[i].x1,triangle3d_buffer[i].y1,triangle3d_buffer[i].z1,
              triangle3d_buffer[i].x2,triangle3d_buffer[i].y2,triangle3d_buffer[i].z2,
              triangle3d_buffer[i].x3,triangle3d_buffer[i].y3,triangle3d_buffer[i].z3,
              triangle3d_buffer[i].nx3d,triangle3d_buffer[i].ny3d,triangle3d_buffer[i].nz3d);
            }
  sort_obj();
}


void rotate_translate_axis(int i,float tx1,float ty1,float tz1,float tx2,float ty2,float tz2,float tx3,float ty3,float tz3,float tnx3d,float tny3d,float tnz3d){
    float ax1 = tx1 - camerax;
    float ay1 = ty1 - cameray;
    float az1 = tz1 - cameraz;
    float ax2 = tx2 - camerax;
    float ay2 = ty2 - cameray;
    float az2 = tz2 - cameraz;
    float ax3 = tx3 - camerax;
    float ay3 = ty3 - cameray;
    float az3 = tz3 - cameraz;

  float cx1,cx2,cx3,cy1,cy2,cy3,cz1,cz2,cz3,cnx3d,cny3d,cnz3d;
  float ccx1,ccx2,ccx3,ccy1,ccy2,ccy3,ccz1,ccz2,ccz3,ccnx3d,ccny3d,ccnz3d;

  //rotate x axis 
  cy1 = ay1*cos(camerapitch) - az1*sin(camerapitch);
  cz1 = ay1*sin(camerapitch) + az1*cos(camerapitch);
  cx1 = ax1;
  cnx3d = tnx3d;
  cny3d = tny3d*cos(camerapitch) - tnz3d*sin(camerapitch);
  cnz3d = tny3d*sin(camerapitch) + tnz3d*cos(camerapitch);

  

  cy2 = ay2*cos(camerapitch) - az2*sin(camerapitch);
  cz2 = ay2*sin(camerapitch) + az2*cos(camerapitch);
  cx2 = ax2;

  cy3 = ay3*cos(camerapitch) - az3*sin(camerapitch);
  cz3 = ay3*sin(camerapitch) + az3*cos(camerapitch);
  cx3 = ax3;
  


  //rotate y axis
  ccx1 =  cx1*cos(camerayaw) + cz1*sin(camerayaw);
  ccz1 = -cx1*sin(camerayaw) + cz1*cos(camerayaw);
  ccy1 =  cy1;
  ccnx3d =  cnx3d*cos(camerayaw) + cnz3d*sin(camerayaw);
  ccnz3d = -cnx3d*sin(camerayaw) + cnz3d*cos(camerayaw);
  ccny3d =  cny3d;


  ccx2 =  cx2*cos(camerayaw) + cz2*sin(camerayaw);
  ccz2 = -cx2*sin(camerayaw) + cz2*cos(camerayaw);
  ccy2 =  cy2;

  ccx3 =  cx3*cos(camerayaw) + cz3*sin(camerayaw);
  ccz3 = -cx3*sin(camerayaw) + cz3*cos(camerayaw);
  ccy3 =  cy3;

  //translate
  ccx1 = ccx1+camerax;
  ccy1 = ccy1+cameray; 
  ccz1 = ccz1+cameraz;
  ccx2 = ccx2+camerax;
  ccy2 = ccy2+cameray; 
  ccz2 = ccz2+cameraz;
  ccx3 = ccx3+camerax;
  ccy3 = ccy3+cameray; 
  ccz3 = ccz3+cameraz;


  if (ortho){
  ccx1 = ccx1*cameraz;
  ccy1 = ccy1*cameraz; 

  ccx2 = ccx2*cameraz;
  ccy2 = ccy2*cameraz; 

  ccx3 = ccx3*cameraz;
  ccy3 = ccy3*cameraz; 
  }
  else{
  ccx1 = (ccx1/(camerafov-ccz1))*cameraz;
  ccy1 = (ccy1/(camerafov-ccz1))*cameraz; 

  ccx2 = (ccx2/(camerafov-ccz2))*cameraz;
  ccy2 = (ccy2/(camerafov-ccz2))*cameraz; 

  ccx3 = (ccx3/(camerafov-ccz3))*cameraz;
  ccy3 = (ccy3/(camerafov-ccz3))*cameraz; 
  }


camerabuffer[i].x1 = ccx1;
camerabuffer[i].x2 = ccx2;
camerabuffer[i].x3 = ccx3;

camerabuffer[i].y1 = ccy1;
camerabuffer[i].y2 = ccy2;
camerabuffer[i].y3 = ccy3;

camerabuffer[i].z1 = ccz1;
camerabuffer[i].z2 = ccz2;
camerabuffer[i].z3 = ccz3;

camerabuffer[i].nx3d = ccnx3d;
camerabuffer[i].ny3d = ccny3d;
camerabuffer[i].nz3d = ccnz3d;


}
