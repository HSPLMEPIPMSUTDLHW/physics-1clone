#include "raylib.h"
#include "raymath.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "game.h"
#include <vector>
using namespace std;
class body
{
private:
	Vector2 pos;
	Vector2 vel;
	Vector2 drag;
	float mass;
public:
	std::vector<body> physobjects;
};