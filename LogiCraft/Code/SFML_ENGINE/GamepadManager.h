#pragma once
#include "Tools.h"
#include "Steam/isteaminput.h"


enum GamepadInput
{
	A, B, X, Y, LB, RB, LT, RT, SELECT, START, UP, DOWN, LEFT, RIGHT
};





static class GamepadManager
{
public:
	GamepadManager();
	~GamepadManager();

	GamepadInput getInput(unsigned char index);

};
