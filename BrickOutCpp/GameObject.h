#pragma once
#include "Buffer.h"

class GameObject
{
public:
	int _width, _height, _x, _y, _color;
	bool death = false;
	char16_t _symbol;

	GameObject() {};

	GameObject(wd* wData, int width, int height, int x, int y, int color, char16_t symbol) {
		_width = width, _height = height, _x = x, _y = y, _symbol = symbol, _color = color;
		_wData = wData;
	};

	virtual void DrawObject();

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

	int lifes = 3, speed = 2;

	MyMortar(wd* wData, int width, int height, int x, int y, int color, char16_t symbol) : GameObject(wData, width, height, x, y, color, symbol) {
	};

	void MoveMyMortar();

	void DeathMortar(bool &worldIsRun);

};





class Ball : public GameObject 
{
public:

	int LEFT = _x - 1, TOP = _y - 1, RIGHT = _x + 1, BOT = _y + 1;

	bool RIGHT_TOP = 1, LEFT_TOP = 0, LEFT_BOTTOM = 0, RIGHT_BOTTOM = 0;

	Ball(wd* wData, int width, int height, int x, int y, int color, char16_t symbol) : GameObject(wData, width, height, x, y, color, symbol) {};

	void BallMove();

	void ChangeDirection();

};





class Brick : public GameObject
{
public:

	Brick(wd* wData, int width, int height, int x, int y, int color, char16_t symbol) : GameObject(wData, width, height, x, y, color, symbol) {
	};

	void DrawObject();

	void BrickHit(vector<Brick*> &brickList, int i);
	
private: 

	int strength = 1 + rand() % 3;

};





class Bonus : public GameObject
{
public:

	bool alreadyDone = false;

	Bonus() {};

	Bonus(wd* wData, int width, int height, int x, int y, int color, char16_t symbol) : GameObject(wData, width, height, x, y, color, symbol) {};

	void BonusGo(vector<Bonus*> &bonusList, int i);
};