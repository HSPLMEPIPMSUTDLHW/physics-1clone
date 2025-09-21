/*
This project uses the Raylib framework to provide us functionality for math, graphics, GUI, input etc.
See documentation here: https://www.raylib.com/, and examples here: https://www.raylib.com/examples.html
*/

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "game.h"

const unsigned int TARGET_FPS = 50;
float dt = 1.0f/TARGET_FPS;
float time = 0;
float startX = 500;
float startY = 500;
float X = 500;
float Y = 500;
//float frequency = 1;
//float amplitude = 100;
float launchAngle;
float launchSpeed;
Vector2 birdPos;
 

void update()
{
    dt = 1.0f / TARGET_FPS;
    time += dt;
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {          
        launchAngle = 0;
        X = startX;
        Y = startY;
        launchSpeed = 0;
    }
    birdPos.x = startX - X;
    birdPos.y = startY - Y;



}
float Hyp(float x, float y)
{
    return sqrt((x * x) + (y * y));
}
Vector2 launchVel(float X, float Y,float angle, float hyp)
{
    float x = cos(angle) * hyp;
    float y = sin(angle) * hyp;
   
    return { X+x,Y+y };
}
void draw()
{
    BeginDrawing();
    ClearBackground(DARKBLUE);
    GuiSliderBar(Rectangle{ 60, 10, 1000, 10 }, "Angle", TextFormat("%.1f", launchAngle), &launchAngle, -PI, PI);
    GuiSliderBar(Rectangle{ 60, 40, 1000, 10 }, "X Pos", TextFormat("%.1f", X), &X, 0, InitialWidth);
    GuiSliderBar(Rectangle{ 60, 70, 1000, 10 }, "Y Pos", TextFormat("%.1f", Y), &Y, 0, InitialHeight);
    GuiSliderBar(Rectangle{ 60, 100, 1000, 10 }, "Speed", TextFormat("%.1f", launchSpeed), &launchSpeed, 0, 500);
   
    DrawText(TextFormat("Angle: %.1f", -launchAngle * (180 / PI)), GetScreenWidth() - 140, 10, 20, LIGHTGRAY);;
    DrawText(TextFormat("X: %.1f", birdPos.x), GetScreenWidth() - 140, 40, 20, LIGHTGRAY);
    DrawText(TextFormat("Y: %.1f", birdPos.y), GetScreenWidth() - 140, 70, 20, LIGHTGRAY);
    DrawText(TextFormat("Speed: %.1f", launchSpeed), GetScreenWidth() - 140, 100, 20, LIGHTGRAY);
   // DrawText(TextFormat("Hyp: %.f", Hyp(birdPos.x, birdPos.y)), GetScreenWidth() - 140, 70, 20, LIGHTGRAY);
  
  
      
    DrawCircle(X, Y, 25, RED);
    //marks starting position
    DrawCircle(startX, startY, 5, GREEN);
 
   
    DrawLineV({X,Y}, launchVel(X,Y,launchAngle, launchSpeed), RED);
    EndDrawing();
}
int main()
{
    InitWindow(InitialWidth, InitialHeight, "Michael Hatzitolios 101419422 Game2005");
    SetTargetFPS(TARGET_FPS);

    while (!WindowShouldClose())
    {
        update();
        draw();
        

    }

    CloseWindow();
    return 0;
}

