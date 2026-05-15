#include <stdio.h>
#include <stdlib.h>
#include "windowhandling.h"
#include "graphics.h"
#include "camera.h"
#include "obj.h"


// Main function
int main()
{
  
  start(); // Initialize your graphical environment
  loadobj(); 
  camera(0);
  while (run)
  {                             // Main application loop
    Events();                   // Handle events (like user inputs)
    graphics_fill_screen(124, 124, 124); // Clear screen to white
     objtocamera();//set object array  based on camera
    drawobj();//draw object array
    Render(); // Render everything on screen
    
  }

  
  return 0;
}

