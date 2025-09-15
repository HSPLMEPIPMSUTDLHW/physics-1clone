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
float frequency = 1;
float amplitude = 100;
float launchAngle;
Vector2 birdPos;
 

void update()
{
    dt = 1.0f / TARGET_FPS;
    time += dt;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {          
        X = GetMousePosition().x;
        Y = GetMousePosition().y;
      //  launchAngle
    }
    else
    {
  
        X = startX;
        Y = startY;
    }
    birdPos.x = startX - X;
    birdPos.y = startY - Y;
   // launchAngle = atan((birdPos.y / birdPos.x));
     launchAngle = atan((birdPos.y / birdPos.x));
 //   X = X + (-sin(time * frequency)) * frequency * amplitude * dt;
//    Y = Y + (cos(time * frequency)) * frequency * amplitude * dt;


}
float Hyp(float x, float y)
{
    return sqrt((x * x) + (y * y));
}
void draw()
{
    BeginDrawing();
    ClearBackground(DARKBLUE);
    DrawText("Michael Hatzitolios 101419422 world!", 10, 740, 20, LIGHTGRAY);


    
    GuiSliderBar(Rectangle{ 60, 5, 1000, 10 }, "Time", TextFormat("%.2f", time), &time, 0, 240);
    //Postion is based on the starting postion
    DrawText(TextFormat("X: %.f", birdPos.x), GetScreenWidth() - 140, 10, 20, LIGHTGRAY);
    DrawText(TextFormat("Y: %.f", birdPos.y), GetScreenWidth() - 140, 40, 20, LIGHTGRAY);
    DrawText(TextFormat("Hyp: %.f", Hyp(birdPos.x, birdPos.y)), GetScreenWidth() - 140, 70, 20, LIGHTGRAY);
 //   DrawText(TextFormat("X: %.f", X), GetScreenWidth() - 140, 100, 20, LIGHTGRAY);
    DrawText(TextFormat("Angle: %.f", launchAngle*(180/PI)), GetScreenWidth() - 140, 130, 20, LIGHTGRAY);
    //sin
    DrawCircle(X, Y, 25, RED);
    DrawCircle(startX,startY, 5, GREEN);
    DrawLineV({ X,Y }, { startX,startY }, WHITE);
    DrawLineV({ startX,startY }, { startX +birdPos.x,startY + birdPos.y }, RED);
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

