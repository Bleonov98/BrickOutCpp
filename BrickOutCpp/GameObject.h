#pragma once
#include "Buffer.h"

class GameObject
{
public:
	int _width, _height, _x, _y;
	bool death = false;
	char _symbol;

	GameObject(wd* wData, int width, int height, int x, int y, char symbol) {
		_width = width, _height = height, _x = x, _y = y, _symbol = symbol;
		_wData = wData;
	};

	void DrawObject();

	void EraseObject();

	virtual ~GameObject() {
		delete this;
	};

protected:

	wd* _wData;
};





class MyMortar : public GameObject 
{
public:
	int lifes = 3;
	float speed = 2;

	MyMortar(wd* wData, int width, int height, int x, int y, char symbol) : GameObject(wData, width, height, x, y, symbol) {};

	void MoveMyMortar();

	void DeathMortar(bool &worldIsRun);
};






class Ball : public GameObject 
{
public:

	int LEFT = _x - 1, TOP = _y - 1, RIGHT = _x + 1, BOT = _y + 1;

	bool RIGHT_TOP = 1, LEFT_TOP = 0, LEFT_BOTTOM = 0, RIGHT_BOTTOM = 0;

	Ball(wd* wData, int width, int height, int x, int y, char symbol) : GameObject(wData, width, height, x, y, symbol) {};

	void BallMove();

	void ChangeDirection();

};






class Brick : public GameObject
{
public:

	Brick(wd* wData, int width, int height, int x, int y, char symbol) : GameObject(wData, width, height, x, y, symbol) {
		if (strength == 1) {
			_symbol = '-';
		}
		else if (strength == 2) {
			_symbol = '=';
		}
		else if (strength == 3) {
			_symbol = symbol;
		}
	};

	void BrickHit(vector<Brick*> &brickList, int i);

	void BonusMortarSize();

	void BonusLife();
	
private: 

	int strength = 1 + rand() % 3;

};