@echo off
gcc src/main.c src/windowhandling.c src/graphics.c src/camera.c -I src -o app -lgdi32
pause
