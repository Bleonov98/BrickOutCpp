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

void Brick::brickHit()
{
	strength--;

	if (strength == 0) {

	}
}

//void Ball::BuildTrajectory(int x, int y)
//{
//	int dx = abs(x - _x);
//	int dy = abs(y - _y);
//
//	int error = dx - dy;
//
//	int X = _x;
//	int Y = _y;
//	int dirY = y - _y;
//	int dirX = x - _x;
//
//	dirY = (dirY > 0) ? 1 : -1;
//	dirX = (dirX > 0) ? 1 : -1;
//
//	while (X != x || Y != y)
//	{
//		traj.push_back(make_pair(X, Y));
//		int error2 = error * 2;
//		if (error2 > -dy) {
//			error -= dy;
//			X += dirX;
//		}
//		if (error2 < dx) {
//			error += dx;
//			Y += dirY;
//		}
//	}
//}

void Ball::BallMove()
{
	EraseObject();
	_x++;
	_y--;

	//if (traj.empty()) {

	//}
}