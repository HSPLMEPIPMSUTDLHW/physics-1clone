/*
This project uses the Raylib framework to provide us functionality for math, graphics, GUI, input etc.
See documentation here: https://www.raylib.com/, and examples here: https://www.raylib.com/examples.html
*/

#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "game.h"
#include <iostream>
#include <string>
#include <vector>
const unsigned int TARGET_FPS = 50;
float dt = 1.0f/TARGET_FPS;
float mytime = 0;
float startX = 100;
float startY = InitialHeight - 100;
float X = startX;
float Y = startY;
//float frequency = 1;
//float amplitude = 100;
float launchAngle;
float launchSpeed;
Vector2 birdPos;




class physbody
{
   
public:
    Vector2 pos;
    Vector2 vel;
    Vector2 drag;
    float mass;
    float radius;
    std::string name = "obj";
    Color color = RED;

    void draw()
    {
        DrawCircle(pos.x, pos.y, radius, color);

     //   DrawText(name.c_str(), pos.x, pos.y, radius * 2, LIGHTGRAY);

        //Draw velocity
        DrawLineEx(pos, pos + vel, 1, color);
    }
};

class physicSim
{
private:
    unsigned int objcount = 0;
public:
    std::vector<physbody> physobjects; // All objects in physics simulation
    Vector2 accelerationGravity = { 0, 9 };

    void add(physbody newObject) // Add to physics simulation
    {
        newObject.name = std::to_string(objcount);
        physobjects.push_back(newObject);
        objcount++;
    }

  
    void update()
    {
        for (int i = 0; i < physobjects.size(); i++)
        {
            //vel = change in position / time, therefore     change in position = vel * time 
            physobjects[i].pos = physobjects[i].pos + physobjects[i].vel * dt;
            //accel = deltaV / time (change in velocity over time) therefore     deltaV = accel * time
            physobjects[i].vel = physobjects[i].vel + accelerationGravity * dt;
        }
    }
};

physicSim simulation;

void update()
{
    dt = 1.0f / TARGET_FPS;
    mytime += dt;
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {          
        launchAngle = 0;
        X = startX;
        Y = startY;
        launchSpeed = 0;
        simulation.accelerationGravity.y = 9;
    }
    if (IsKeyPressed(KEY_SPACE))
    {

        physbody bird;
     
        bird.pos = { X, (float)GetScreenHeight() - 100 };
        bird.vel = { launchSpeed * (float)cos(launchAngle), -launchSpeed * (float)sin(launchAngle) };
        bird.radius = 10;
        simulation.add(bird);
        std::cout << "Birds: " << simulation.physobjects.size() << std::endl;
    }

    birdPos.x = startX - X;
    birdPos.y = startY - Y;

    simulation.update();

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
    GuiSliderBar(Rectangle{ 60, 130, 1000, 10 }, "Gravity", TextFormat("%.1f", simulation.accelerationGravity.y), &simulation.accelerationGravity.y, -500, 500);
   
    DrawText(TextFormat("Angle: %.1f", -launchAngle * (180 / PI)), GetScreenWidth() - 140, 10, 20, LIGHTGRAY);;
    DrawText(TextFormat("X: %.1f", birdPos.x), GetScreenWidth() - 140, 40, 20, LIGHTGRAY);
    DrawText(TextFormat("Y: %.1f", birdPos.y), GetScreenWidth() - 140, 70, 20, LIGHTGRAY);
    DrawText(TextFormat("Speed: %.1f", launchSpeed), GetScreenWidth() - 140, 100, 20, LIGHTGRAY);
   // DrawText(TextFormat("Hyp: %.f", Hyp(birdPos.x, birdPos.y)), GetScreenWidth() - 140, 70, 20, LIGHTGRAY);
  
  
      
  //  DrawCircle(X, Y, 25, RED);
    //marks starting position
    DrawCircle(X, Y, 5, GREEN);
 
   
    DrawLineV({X,Y}, launchVel(X,Y,-launchAngle, launchSpeed), RED);
    for (int i = 0; i < simulation.physobjects.size(); i++)
    {
     //   std::cout << "Birds" << std::endl;
        simulation.physobjects[i].draw();
    }

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

