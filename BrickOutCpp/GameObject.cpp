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
		if (_x <= 147 - _width) {
			EraseObject();
			_x += speed;
		}
	}
	if (GetAsyncKeyState(VK_LEFT)) {
		if (_x > 3) {
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

void Brick::DrawObject()
{
	if (strength == 1) {
		_symbol = '-';
	}
	else if (strength == 2) {
		_symbol = '=';
	}
	else if (strength == 3) {
		_symbol = '#';
	}

	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			_wData->vBuf[_y + i][_x + j] = _symbol;
		}
	}
}

void Brick::BrickHit(vector<Brick*> &brickList, int i)
{
	this->strength--;

	if (this->strength > 0) {
		PlaySound(MAKEINTRESOURCE(IDR_WAVE3), NULL, SND_RESOURCE | SND_ASYNC);
	}

	if (this->strength == 0) {
		PlaySound(MAKEINTRESOURCE(IDR_WAVE4), NULL, SND_RESOURCE | SND_ASYNC);
		death = true;
		brickList.erase(brickList.begin() + i);
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

	if (RIGHT_TOP && RIGHT >= 149) {
		RIGHT_TOP = 0;
		LEFT_TOP = 1;
	}

	if (RIGHT_TOP && TOP <= 1) {
		RIGHT_TOP = 0;
		RIGHT_BOTTOM = 1;
	}

	if (RIGHT_BOTTOM && RIGHT >= 149) {
		RIGHT_BOTTOM = 0;
		LEFT_BOTTOM = 1;
	}

	if (LEFT_TOP && LEFT <= 1) {
		LEFT_TOP = 0;
		RIGHT_TOP = 1;
	}

	if (LEFT_TOP && TOP <= 1) {
		LEFT_TOP = 0;
		LEFT_BOTTOM = 1;
	}

	if (LEFT_BOTTOM && LEFT <= 1) {
		LEFT_BOTTOM = 0;
		RIGHT_BOTTOM = 1;
	}
}

void Bonus::BonusGo(vector<Bonus*>& bonusList, int i)
{
	EraseObject();
	_y++;

	if (_y == ROWS - 7) {
		death = true;
		bonusList.erase(bonusList.begin() + i);
		EraseObject();
	}
}
