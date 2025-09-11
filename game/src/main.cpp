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
float X = 500;
float Y = 500;
float frequency = 1;
float amplitude = 100;

void update()
{
    dt = 1.0f / TARGET_FPS;
    time += dt;
    X = X + (-sin(time * frequency)) * frequency * amplitude * dt;
    Y = Y + (cos(time * frequency)) * frequency * amplitude * dt;
    
}
void draw()
{
    BeginDrawing();
    ClearBackground(DARKBLUE);
    DrawText("Michael Hatzitolios 101419422 world!", 10, 740, 20, LIGHTGRAY);


    
    GuiSliderBar(Rectangle{ 60, 5, 1000, 10 }, "Time", TextFormat("%.2f", time), &time, 0, 240);
    DrawText(TextFormat("T: %.f", time), GetScreenWidth() - 140, 10, 30, LIGHTGRAY);

    DrawCircle(X, Y, 50, RED);
    DrawCircle(500+cos(time * frequency)* frequency* amplitude, 500 + (-sin(time * frequency)) * frequency * amplitude, 50, GREEN);


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
