#pragma once
#include "Buffer.h"

class GameObject
{
public:
	int _width, _height, _x, _y;
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

	MyMortar(wd* wData, int width, int height, int x, int y, char symbol) : GameObject(wData, width, height, x, y, symbol) {};

	void MoveMyMortar();
};






class Ball : public GameObject 
{
public:
	bool ballGo = false;

	Ball(wd* wData, int width, int height, int x, int y, char symbol) : GameObject(wData, width, height, x, y, symbol) {};

	void BallMove();

	//void BuildTrajectory(int x, int y);

private:

	//vector <pair<int, int>> traj;

};






class Brick : public GameObject
{
public:

	Brick(wd* wData, int width, int height, int x, int y, char symbol) : GameObject(wData, width, height, x, y, symbol) {};

	void brickHit();
	
private: 

	int strength = rand() % 3;

};