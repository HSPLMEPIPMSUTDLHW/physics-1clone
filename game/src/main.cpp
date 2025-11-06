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
float dt = 1.0f / TARGET_FPS;
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

enum shapetype {
    CIRCLE,
    SQUARE,
    HALFSPACE
};


class physbody
{

public:
    Vector2 pos;
    Vector2 vel;
    Vector2 drag;
    float mass;
    bool isStatic;
    shapetype shapeType;
    std::string name = "obj";
    Color color = GREEN;

    virtual void draw() {}
    Vector2 getPos()
    {
        return pos;
    }
    shapetype getType()
    {
        return shapeType;
    }
    bool isObjStatic()
    {
        return isStatic;
    }



};

class physCircle : public physbody
{
public:
    float radius;
    physCircle()
    {
        shapeType = CIRCLE;
        isStatic = false;
    };
 
    void draw() override
    {
        DrawCircle(pos.x, pos.y, radius, color);

        //Draw velocity
        DrawLineEx(pos, pos + vel, 1, color);
    }
    float getRad()
    {
        return radius;
    }
};

class physhalfspace : public physbody
{
private:
    float rotation = 0;
    Vector2 normal = { 0,-1 };
public:
    physhalfspace()
    {
        shapeType = HALFSPACE;
       // shapeType = SQUARE; //get fucked i dont want to deal with you rn
        color = RED;
        isStatic = true;
    }
 

    void draw() override 
    { 
        DrawCircle(pos.x, pos.y, 8, color);

        DrawLineEx(pos, pos + normal*30, 1, color);
        Vector2 parallelToSurface = Vector2Rotate(normal, 90 * DEG2RAD);
        DrawLineEx(pos- parallelToSurface* InitialWidth, pos + parallelToSurface * InitialWidth, 1, color);
    }
    
    float getRotation()
    {
        return rotation;
    }        
    Vector2 getNormal()
    {
        return normal;
    }
    void setRotation(float r)
    {
        rotation = r;   
        normal = Vector2Rotate({ 0,-1 }, rotation * DEG2RAD);
    }
    Vector2 getplane()
    {
        Vector2 parallelToSurface = Vector2Rotate(normal, rotation * DEG2RAD);
        return { parallelToSurface};
    }
};

class physBox : public physbody
{
public:
    Vector2 size;

    //   void draw() {}

};

float Hyp(float x, float y)
{
    return sqrt((x * x) + (y * y));
};
float Hyp(Vector2 a)
{
    return sqrt((a.x * a.x) + (a.y * a.y));
};
float DotProduct(Vector2 a, Vector2 b)
{
    return ((a.x * b.x) + (a.y * b.y));
};
float AngleBetweenVectors(Vector2 a, Vector2 b)
{
    return acos((DotProduct(a, b) / (Hyp(a) * Hyp(b))));
};

Vector2 launchVel(float X, float Y, float angle, float hyp)
{
    float x = cos(angle) * hyp;
    float y = sin(angle) * hyp;

    return { X + x,Y + y };
};

class physicSim
{
private:
    unsigned int objcount = 0;
public:
    std::vector<physbody*> physobjects; // All objects in physics simulation
    Vector2 accelerationGravity = { 0, 0 };

    void add(physbody* newObject) // Add to physics simulation
    {
        newObject->name = std::to_string(objcount);
        physobjects.push_back(newObject);
        objcount++;
    }


    void update()
    {

        for (int i = 0; i < physobjects.size(); i++)
        {
            //vel = change in position / time, therefore     change in position = vel * time 
            if (!physobjects[i]->isObjStatic())
            {

         
            physobjects[i]->pos = physobjects[i]->pos + physobjects[i]->vel * dt;
            //accel = deltaV / time (change in velocity over time) therefore     deltaV = accel * time
            physobjects[i]->vel = physobjects[i]->vel + accelerationGravity * dt;
            physobjects[i]->color = GREEN; //
            }
        }
        checkCollisions();
        

    }

    bool CircleCircleCollision(physCircle* a, physCircle* b)
    {
        // calcualtes the distance between the two circles
        // then checks if thhe distance is smaller then the radius' of the circles
        // if the distance is smaller then it returns true
        Vector2 d = a->getPos() - b->getPos();
        if (Hyp(d) < (a->getRad() + b->getRad()))
        {
            return true;

        }
        return false;
    }

    bool CirclePlaneCollision(physCircle* a, physhalfspace* b)
    {
        float viper = DotProduct(a->getPos() - b->getPos(), b->getNormal());
        Vector2 projection = b->getNormal() * viper;
        DrawLineEx(a->getPos(), a->getPos() - projection, 1, WHITE);

        std::cout << "angle is " << RAD2DEG * AngleBetweenVectors(a->getPos()-b->getPos(), b->getNormal()) << std::endl;
        if (DotProduct(a->getPos() - b->getPos(), b->getNormal())>0)
        {
           
            return true;

        }
        return false;
    }

    void checkCollisions()
    {
        //    std::cout << "Birds: " << std::endl;
        for (int i = 0; i < physobjects.size(); i++)
        {
          
            if (physobjects[i]->getType() == CIRCLE)
            {
                physbody* objA = physobjects[i];
                physCircle* circleA = (physCircle*)objA;
                for (int j = 1 + i; j < physobjects.size(); j++)
                {

                    if (physobjects[j]->getType() == CIRCLE)
                    {
;
                        // objA->color = GREEN;
                        physbody* objB = physobjects[j];
                        physCircle* circleB = (physCircle*)objB;

                        if (CircleCircleCollision(circleA, circleB))
                        {
                            //   std::cout << "Birds collided " << std::endl;
                            objA->color = RED;
                            objB->color = RED;
                            float rads = circleA->getRad()+ circleB->getRad();
                            Vector2 A2B = circleA->getPos() - circleB->getPos();
                            float ABD = Hyp(circleA->getPos() - circleB->getPos());
                            float overlap = rads - ABD;
                            if (ABD == 0)
                            {
                                std::cout << "NO TELEPORTONG" << std::endl;
                                A2B = { 1, 0 };
                                ABD = 1;
                            }
                            Vector2 NormalizedA2B = A2B / ABD;
                         //   std::cout << "RadiusA: " << circleA->getRad() << " RadiusB: " << circleB->getRad() << " Radiust: " << rads << std::endl;
                          //  std::cout << "Distance: " << ABD << " Overlap: " << rads - ABD << std::endl;
                       
                            Vector2 mtv = NormalizedA2B * overlap;
                       
                            if (overlap > 0)
                            {
                             
                              // std::cout << "A2B: " << A2B.x << " " << A2B.y << std::endl;
                              // std::cout << "ABD: " << ABD << std::endl;
                              // std::cout << "NormalizedA2B: " << NormalizedA2B.x << " " << NormalizedA2B.y << std::endl;
                              // std::cout << "mtv: " << mtv.x << " " << mtv.y << std::endl;
                              // std::cout << "PosA: " << circleA->getPos().x << " " << circleA->getPos().y << " PosB: " << circleB->getPos().x << " " << circleB->getPos().y << std::endl;
                                objA->pos += mtv * 0.5;
                                objB->pos -= mtv * 0.5;
                              //  std::cout << "PosA: " << circleA->getPos().x << " "<< circleA->getPos().y<< " PosB: " << circleB->getPos().x << " " << circleB->getPos().y << std::endl;
                            }

                         
                                
                        }

                    }
                    else if (physobjects[j]->getType() == HALFSPACE)
                    {
                        physbody* objB = physobjects[j];
                        physhalfspace* spaceB = (physhalfspace*)objB;
                        if (CirclePlaneCollision(circleA, spaceB))
                        {
                            
                            objA->color = RED;
                        }
                    }
                }
            }
            else if (physobjects[i]->getType() == HALFSPACE)
                {
                    physbody* objA = physobjects[i];
                    physhalfspace* spaceA = (physhalfspace*)objA;
                    for (int j = 1 + i; j < physobjects.size(); j++)
                    {

                        if (physobjects[j]->getType() == CIRCLE)
                        {
                            
                            // objA->color = GREEN;
                            physbody* objB = physobjects[j];
                            physCircle* circleB = (physCircle*)objB;

                            if (CirclePlaneCollision(circleB, spaceA))
                            {
                                //   std::cout << "Birds collided " << std::endl;
                                objB->color = RED;
                            }

                        }
                    }
                }
            
        }
    }
};


physicSim simulation;
physhalfspace plane;


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

        physCircle* bird = new physCircle();

        bird->pos = { X, Y };
        bird->vel = { launchSpeed * (float)cos(launchAngle), -launchSpeed * (float)sin(launchAngle) };
        bird->radius = 20;
        simulation.add(bird);
        std::cout << "Birds: Created " << simulation.physobjects.size() << std::endl;
    }

    birdPos.x = startX - X;
    birdPos.y = startY - Y;

    simulation.update();

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
   
    float planeAngle = plane.getRotation();
    GuiSliderBar(Rectangle{ 60, 160, 1000, 10 }, "Plane rotation", TextFormat("%.0f", plane.getRotation()), &planeAngle, -180, 180);
    plane.setRotation(planeAngle);
    //std::cout << plane.getRotation() << std::endl;
   
    GuiSliderBar(Rectangle{ 60, 190, 1000, 10 }, "Plane X", TextFormat("%.1f", plane.pos.x), &plane.pos.x, 0, InitialWidth);
    GuiSliderBar(Rectangle{ 60, 220, 1000, 10 }, "Plane Y", TextFormat("%.1f", plane.pos.y), &plane.pos.y, 0, InitialHeight);

    DrawText(TextFormat("Angle: %.1f", -launchAngle * (180 / PI)), GetScreenWidth() - 140, 10, 20, LIGHTGRAY);;
    DrawText(TextFormat("X: %.1f", birdPos.x), GetScreenWidth() - 140, 40, 20, LIGHTGRAY);
    DrawText(TextFormat("Y: %.1f", birdPos.y), GetScreenWidth() - 140, 70, 20, LIGHTGRAY);
    DrawText(TextFormat("Speed: %.1f", launchSpeed), GetScreenWidth() - 140, 100, 20, LIGHTGRAY);
    DrawText(TextFormat("Angle: %.1f", plane.getRotation()), GetScreenWidth() - 140, 160, 20, LIGHTGRAY);;
    DrawText(TextFormat("X: %.1f", plane.pos.x), GetScreenWidth() - 140, 190, 20, LIGHTGRAY);
    DrawText(TextFormat("Y: %.1f", plane.pos.y), GetScreenWidth() - 140, 220, 20, LIGHTGRAY);
    // DrawText(TextFormat("Hyp: %.f", Hyp(birdPos.x, birdPos.y)), GetScreenWidth() - 140, 70, 20, LIGHTGRAY);



   //  DrawCircle(X, Y, 25, RED);
     //marks starting position
    DrawCircle(X, Y, 5, GREEN);


    DrawLineV({ X,Y }, launchVel(X, Y, -launchAngle, launchSpeed), RED);
    for (int i = 0; i < simulation.physobjects.size(); i++)
    {
        //   std::cout << "Birds" << std::endl;
        simulation.physobjects[i]->draw();
    }

    EndDrawing();
}
int main()
{
    InitWindow(InitialWidth, InitialHeight, "Michael Hatzitolios 101419422 Game2005");
    SetTargetFPS(TARGET_FPS);
    plane.pos = { 500,700 };
   // physhalfspace* planepointer = &plane;
    simulation.add(&plane);

    while (!WindowShouldClose())
    {
        update();
        draw();
        plane.draw();

    }

    CloseWindow();
    return 0;
}

