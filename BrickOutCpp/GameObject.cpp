#include "GameObject.h"

void GameObject::DrawObject()
{
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			_wData->vBuf[_y + i][_x + j] = _symbol;
		}
	}
}

void GameObject::EraseObject()
{
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			_wData->vBuf[_y + i][_x + j] = ' ';
		}
	}
}

void MyMortar::MoveMyMortar()
{
	if (GetAsyncKeyState(VK_RIGHT)) {
		if (_x <= 148 - _width) {
			EraseObject();
			_x++;
		}
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		if (_x > 3) {
			EraseObject();
			_x--;
		}
	}
}

void MyMortar::DeathMortar()
{
	lifes--;

	if (lifes <= 0) {
		death = true;
	}
}

void Brick::BrickHit()
{
	strength--;

	if (strength <= 0) {
		death = true;
		EraseObject();
	}
}

void Ball::BallMove()
{
	EraseObject();

	if (RIGHT_TOP) {
		_x++;
		_y--;
	}

	if (LEFT_TOP) {
		_x--;
		_y--;
	}

	if (RIGHT_BOTTOM) {
		_x++;
		_y++;
	}

	if (LEFT_BOTTOM) {
		_x--;
		_y++;
	}
}

void Ball::ChangeDirection()
{
	LEFT = _x - 1, TOP = _y - 1, RIGHT = _x + 1, BOT = _y + 1;

	if (RIGHT_TOP && RIGHT == 150) {
		RIGHT_TOP = 0;
		LEFT_TOP = 1;
	}

	if (RIGHT_TOP && TOP == 0) {
		RIGHT_TOP = 0;
		RIGHT_BOTTOM = 1;
	}

	if (RIGHT_BOTTOM && RIGHT == 150) {
		RIGHT_BOTTOM = 0;
		LEFT_BOTTOM = 1;
	}

	if (LEFT_TOP && LEFT == 0) {
		LEFT_TOP = 0;
		RIGHT_TOP = 1;
	}

	if (LEFT_TOP && TOP == 0) {
		LEFT_TOP = 0;
		LEFT_BOTTOM = 1;
	}

	if (LEFT_BOTTOM && LEFT == 0) {
		LEFT_BOTTOM = 0;
		RIGHT_BOTTOM = 1;
	}
}
