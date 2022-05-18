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
		if (_x <= 149 - _width) {
			EraseObject();
			_x += speed;
		}
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		if (_x > 1) {
			EraseObject();
			_x -= speed;
		}
	}
}

void MyMortar::DeathMortar(bool& worldIsRun)
{
	lifes--;

	if (lifes <= 0) {
		death = true;
		worldIsRun = 0;
	}
}

void Brick::BrickHit(vector<Brick*> &brickList, int i)
{
	this->strength--;

	if (this->strength == 0) {
		death = true;
		brickList.erase(brickList.begin() + i);
		EraseObject();
	}
}

void Brick::BonusFireBall()
{
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
