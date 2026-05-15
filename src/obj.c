#include <windows.h>
#include<windows.h>
#include "windowhandling.h"
#include "graphics.h"
#include "obj.h"
#include "camera.h"
#include <stdio.h>
#include<stdbool.h>
#include<math.h>


void openfile(void);
Trianglebuffer* triangle_buffer = NULL;
Trianglebuffer* camera_buffer = NULL;
Trianglebuffer* render_buffer = NULL;
Vertex* vertex_buffer = NULL;
Triangleindex* triangle_index_buffer = NULL;
Normal* normal_buffer = NULL;
int vcount = 0 , tcount = 0 , ncount = 0;
bool ortho = true;
bool camchange = false;
float fov = 6;

void openfile(){
    FILE* file = fopen("deer.obj","r");
    char line[128];
    while(fgets(line,sizeof(line),file)){
        if(line[0] == 'v' && line[1]== ' ') vcount++;
        if(line[0] == 'f') tcount++;
        if(line[0] == 'v' && line[1]== 'n') ncount++;
    }
    triangle_index_buffer = malloc(sizeof(Triangleindex)*tcount);
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
             sscanf(line,"f %d//%d %d//%d %d//%d",&triangle_index_buffer[tcounter].v1 , &triangle_index_buffer[tcounter].nx,
                    &triangle_index_buffer[tcounter].v2,&triangle_index_buffer[tcounter].ny,
                    &triangle_index_buffer[tcounter].v3,&triangle_index_buffer[tcounter].nz);
            
            tcounter++;
          }
    }
    fclose(file);
}

 void loadobj(){
    openfile();
    triangle_buffer = malloc(sizeof(Trianglebuffer)* tcount);
	camera_buffer = malloc(sizeof(Trianglebuffer)* tcount);  
    render_buffer = malloc(sizeof(Trianglebuffer)* tcount);
    for (int i=0; i < tcount ; i++){
      Triangleindex x = triangle_index_buffer[i];
      Vertex vx1 = vertex_buffer[x.v1 - 1];
      Vertex vx2 = vertex_buffer[x.v2 - 1];
      Vertex vx3 = vertex_buffer[x.v3 - 1];
      Normal nn = normal_buffer[x.nx-1];

      triangle_buffer[i].x1 = vx1.x;
      triangle_buffer[i].y1 = vx1.y;
      triangle_buffer[i].z1 = vx1.z;
      triangle_buffer[i].nx3d = nn.x;


      triangle_buffer[i].x2 = vx2.x;
      triangle_buffer[i].y2 = vx2.y;
      triangle_buffer[i].z2 = vx2.z;
      triangle_buffer[i].ny3d = nn.y;

      
      triangle_buffer[i].x3 = vx3.x;
      triangle_buffer[i].y3 = vx3.y;
      triangle_buffer[i].z3 = vx3.z;
      triangle_buffer[i].nz3d = nn.z;}
}

void objtocamera(){
 
  memcpy(camera_buffer, triangle_buffer, sizeof(Trianglebuffer) * tcount);
  for (int i = 0; i < tcount; i++){
  rotate_translate_axis(i,camera_buffer[i].x1,camera_buffer[i].y1,camera_buffer[i].z1,
              camera_buffer[i].x2,camera_buffer[i].y2,camera_buffer[i].z2,
              camera_buffer[i].x3,camera_buffer[i].y3,camera_buffer[i].z3,
              camera_buffer[i].nx3d,camera_buffer[i].ny3d,camera_buffer[i].nz3d);
            }
  sort_obj();
}

void rotate_translate_axis(int i,float camera_buffer_x1,float camera_buffer_y1,float camera_buffer_z1,float camera_buffer_x2,float camera_buffer_y2,float camera_buffer_z2,float camera_buffer_x3,float camera_buffer_y3,float camera_buffer_z3,float tnx3d,float tny3d,float tnz3d){
    float ax1 = camera_buffer_x1 - camerax;
    float ay1 = camera_buffer_y1 - cameray;
    float az1 = camera_buffer_z1 - cameraz;
    float ax2 = camera_buffer_x2 - camerax;
    float ay2 = camera_buffer_y2 - cameray;
    float az2 = camera_buffer_z2 - cameraz;
    float ax3 = camera_buffer_x3 - camerax;
    float ay3 = camera_buffer_y3 - cameray;
    float az3 = camera_buffer_z3 - cameraz;

    

     
  float cx1,cx2,cx3,cy1,cy2,cy3,cz1,cz2,cz3,cnx3d,cny3d,cnz3d;
  float ccx1,ccx2,ccx3,ccy1,ccy2,ccy3,ccz1,ccz2,ccz3,ccnx3d,ccny3d,ccnz3d;




  //rotate y axis
  cx1 =  ax1*cos(camerayaw) + az1*sin(camerayaw);
  cz1 = -ax1*sin(camerayaw) + az1*cos(camerayaw);
  cy1 =  ay1;
  cnx3d =  tnx3d*cos(camerayaw) + tnz3d*sin(camerayaw);
  cnz3d = -tnx3d*sin(camerayaw) + tnz3d*cos(camerayaw);
  cny3d =  tny3d;


  cx2 =  ax2*cos(camerayaw) + az2*sin(camerayaw);
  cz2 = -ax2*sin(camerayaw) + az2*cos(camerayaw);
  cy2 =  ay2;

  cx3 =  ax3*cos(camerayaw) + az3*sin(camerayaw);
  cz3 = -ax3*sin(camerayaw) + az3*cos(camerayaw);
  cy3 =  ay3;


    //rotate x axis 
  ccy1 = cy1*cos(camerapitch) - cz1*sin(camerapitch);
  ccz1 = cy1*sin(camerapitch) + cz1*cos(camerapitch);
  ccx1 = cx1;
  ccnx3d = cnx3d;
  ccny3d = cny3d*cos(camerapitch) - cnz3d*sin(camerapitch);
  ccnz3d = cny3d*sin(camerapitch) + cnz3d*cos(camerapitch);

  

  ccy2 = cy2*cos(camerapitch) - cz2*sin(camerapitch);
  ccz2 = cy2*sin(camerapitch) + cz2*cos(camerapitch);
  ccx2 = cx2;

  ccy3 = cy3*cos(camerapitch) - cz3*sin(camerapitch);
  ccz3 = cy3*sin(camerapitch) + cz3*cos(camerapitch);
  ccx3 = cx3;
  
  

  

   /* ccx1 += fx * camerazoom;
    ccy1 += fy * camerazoom;
    ccz1 += fz * camerazoom;

    ccx2 += fx * camerazoom;
    ccy2 += fy * camerazoom;
    ccz2 += fz * camerazoom;

    ccx3 += fx * camerazoom;
    ccy3 += fy * camerazoom;
    ccz3 += fz * camerazoom;*/
	
	camera_buffer[i].x1 = ccx1;
    camera_buffer[i].y1 = ccy1;
    camera_buffer[i].z1 = ccz1;
	camera_buffer[i].x2 = ccx2;
    camera_buffer[i].y2 = ccy2;
    camera_buffer[i].z2 = ccz2;
	camera_buffer[i].x3 = ccx3;
    camera_buffer[i].y3 = ccy3;
    camera_buffer[i].z3 = ccz3;
	camera_buffer[i].nx3d = ccnx3d;
	camera_buffer[i].ny3d = ccny3d;
	camera_buffer[i].nz3d = ccnz3d;
	
   


  if (ortho){
  ccx1 = ccx1*cameraz*camerazoom;
  ccy1 = ccy1*cameraz*camerazoom; 

  ccx2 = ccx2*cameraz*camerazoom;
  ccy2 = ccy2*cameraz*camerazoom; 

  ccx3 = ccx3*cameraz*camerazoom;
  ccy3 = ccy3*cameraz*camerazoom; 
  }
  else{
  ccx1 = (ccx1/(1-ccz1))*camerafov*camerazoom;
  ccy1 = (ccy1/(1-ccz1))*camerafov*camerazoom; 

  ccx2 = (ccx2/(1-ccz2))*camerafov*camerazoom;
  ccy2 = (ccy2/(1-ccz2))*camerafov*camerazoom; 

  ccx3 = (ccx3/(1-ccz3))*camerafov*camerazoom;
  ccy3 = (ccy3/(1-ccz3))*camerafov*camerazoom; 


  
  }


render_buffer[i].x1 = ccx1;
render_buffer[i].x2 = ccx2;
render_buffer[i].x3 = ccx3;

render_buffer[i].y1 = ccy1;
render_buffer[i].y2 = ccy2;
render_buffer[i].y3 = ccy3;

render_buffer[i].z1 = ccz1;
render_buffer[i].z2 = ccz2;
render_buffer[i].z3 = ccz3;

render_buffer[i].nx3d = ccnx3d;
render_buffer[i].ny3d = ccny3d;
render_buffer[i].nz3d = ccnz3d;


}

void sort_obj(){
  for (int i=0; i<tcount-1; i++){
      for(int j=i+1; j<tcount; j++){
        float sum1 = render_buffer[i].z1+render_buffer[i].z2+render_buffer[i].z3;
        float sum2 = render_buffer[j].z1+render_buffer[j].z2+render_buffer[j].z3;

        if(sum1>sum2){
          Trianglebuffer temp = render_buffer[i];
          render_buffer[i] = render_buffer[j];
          render_buffer[j] = temp;
        }
      }
    }
}

void drawobj(){
   for (int i = 0; i < tcount; i++){

  float dp=(render_buffer[i].nx3d * 0.4718) + (render_buffer[i].ny3d * -0.6891) + (render_buffer[i].nz3d * 0.5500);
  int cp = (int)(((dp+1.0f)/2.0f)*255.0f);


    cnnormalize(
        render_buffer[i].x1,
        render_buffer[i].y1 ,
        render_buffer[i].x2,
        render_buffer[i].y2,
        render_buffer[i].x3,
        render_buffer[i].y3,
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


