#include "World.h"

void World::SetPos(int x, int y)
{
	sprintf_s(coord, "%s%d;%dH", CSI, y, x);
	printf(coord);
}

void World::HotKeys()
{
	while (!GetKeyState(VK_SPACE)) {}

	started = true;

	while (worldIsRun) {
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			worldIsRun = false;
		}
		if (GetAsyncKeyState(0x50)) {
			pause = !pause;
		}
	}
}

void World::DrawInfo()
{
	SetPos(17, 52);
	cout << score;
	SetPos(17, 53);
	cout << level;
}

void World::DrawArea()
{
	// Set console code page to UTF-8 so console known how to interpret string data
	SetConsoleOutputCP(CP_UTF8);

	// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(IDR_TEXT1), L"TEXT");

	if (hResource)
	{
		HGLOBAL hLoadedResource = LoadResource(hInstance, hResource);

		if (hLoadedResource)
		{
			LPCSTR area = (LPCSTR)LockResource(hLoadedResource);

			if (area)
			{
				DWORD dwResourceSize = SizeofResource(hInstance, hResource);

				if (0 != dwResourceSize)
				{
					for (int i = 0; i < strnlen(area, 8915); i++) {
						std::cout << area[i];
					}
				}
			}
		}
	}

	setvbuf(stdout, NULL, _IONBF, 0);
}

void World::CreateWorld() {

	term.Terminal();  // Set virtual terminal settings
	term.SetScreenSize();

	printf(CSI "?1049h"); // enable alt buffer
	printf(CSI "?25l"); // hide cursor blinking

	/*DrawTitle();*/
	DrawArea();
}

void World::RunWorld(bool& restart)
{
	srand(time(NULL));
	CreateWorld();
	
	thread hotKeys([&]
		{ HotKeys(); }
	);

	level = 1;
	score = 0;
	int tick = 1;
	

	MyMortar* myMortar = new MyMortar(&wData, 7, 2, COLS / 2 - 6, ROWS - 9, '*');
	Ball* ball = new Ball(&wData, 1, 1, myMortar->_x + myMortar->_width/2, myMortar->_y - 1, '@');
	Bonus* bonusLife = new Bonus;
	Bonus* bonusSize = new Bonus;

	allKnownObjects.push_back(myMortar);
	allKnownObjects.push_back(ball);

	Brick* brick;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < COLS / 8; j++)
		{
			brick = new Brick(&wData, 7, 2, 4 + (8 * j), 2 + (4 * i), '#');
			allKnownObjects.push_back(brick);
			brickList.push_back(brick);
		}
	}

	while (worldIsRun)
	{
		if (started)
		{
			if (!bonusList.empty()) {
				for (int i = 0; i < bonusList.size(); i++)
				{
					bonusList[i]->BonusGo(bonusList, i);
				}
			}

			ball->BallMove();
			ball->ChangeDirection();

			if (ball->_y == ROWS - 6) {

			myMortar->DeathMortar(worldIsRun);
						
			for (int Lifeheight = 0; Lifeheight < 4; Lifeheight++) {

				SetPos(42 + (10 * myMortar->lifes), 51 + Lifeheight);

				for (int Lifewidth = 0; Lifewidth < 5; Lifewidth++) {
					cout << " ";
				}
			}

			Sleep(2000);

			ball->LEFT_BOTTOM = 0;
			ball->LEFT_TOP = 0;
			ball->RIGHT_TOP = 1;
			ball->RIGHT_BOTTOM = 0;
				
			ball->_x = myMortar->_x + myMortar->_width / 2;
			ball->_y = myMortar->_y - 1;
						}

			myMortar->MoveMyMortar();
		}
		// move and direction settings

		// ------------------------------(1)
		if (bonusList.empty()) {
			if (score % 1200 == 0 && score > 0) {
				int randBrickNum = rand() % brickList.size() - 1;
				bonusLife = new Bonus(&wData, 1, 1, brickList[randBrickNum]->_x + brickList[randBrickNum]->_width / 2, brickList[randBrickNum]->_y + 2, 'L');

				bonusList.push_back(bonusLife);
				allKnownObjects.push_back(bonusLife);
			}

			else if (score % 1500 == 0 && score > 0) {
				int randBrickNum = rand() % brickList.size() - 1;
				bonusSize = new Bonus(&wData, 1, 1, brickList[randBrickNum]->_x + brickList[randBrickNum]->_width / 2, brickList[randBrickNum]->_y + 2, 'S');

				bonusList.push_back(bonusSize);
				allKnownObjects.push_back(bonusSize);
			}
		}
		//bonusLife && bonusSize settings(2)

		if (!bonusList.empty()) {
			for (int height = 0; height < myMortar->_height; height++)
			{
				for (int width = 0; width < myMortar->_width; width++)
				{
					if ((myMortar->_x + width == bonusLife->_x) && (myMortar->_y + height == bonusLife->_y) && (myMortar->lifes < 3)) {

						if (myMortar->lifes == 1) {
							SetPos(52, 51);
							cout << "@@ @@";
							SetPos(52, 52);
							cout << "@ @ @";
							SetPos(52, 53);
							cout << " @ @ ";
							SetPos(52, 54);
							cout <<  " @ ";
						}

						if (myMortar->lifes == 2) {
							SetPos(62, 51);
							cout << "@@ @@";
							SetPos(62, 52);
							cout << "@ @ @";
							SetPos(62, 53);
							cout << " @ @ ";
							SetPos(62, 54);
							cout << " @ ";
						}

						myMortar->lifes++;
					}
					if ((myMortar->_x + width == bonusSize->_x) && (myMortar->_y + height == bonusSize->_y)) {
						myMortar->_width += 2;
					}
				}
			}
		}
		//Bonus collision and functional

   		for (int i = 0; i < brickList.size(); i++)
		{
			bool collisionBreak = 0;
			for (int height = 0; height < brickList[i]->_height; height++)
			{
				for (int width = 0; width < brickList[i]->_width; width++)
				{
					if ((ball->RIGHT == brickList[i]->_x + width) && (ball->_y == brickList[i]->_y + height)) {

						if (ball->RIGHT_TOP) {
							ball->RIGHT_TOP = 0;
							ball->LEFT_TOP = 1;
						}
						else if (ball->RIGHT_BOTTOM) {
							ball->RIGHT_BOTTOM = 0;
							ball->LEFT_BOTTOM = 1;
						}

						brickList[i]->BrickHit(brickList, i);

						collisionBreak = 1;
						break;
					}

					else if ((ball->LEFT == brickList[i]->_x + width) && (ball->_y == brickList[i]->_y + height)) {

						if (ball->LEFT_TOP) {
							ball->LEFT_TOP = 0;
							ball->RIGHT_TOP = 1;
						}
						else if (ball->LEFT_BOTTOM) {
							ball->LEFT_BOTTOM = 0;
							ball->RIGHT_BOTTOM = 1;
						}

						brickList[i]->BrickHit(brickList, i);

						collisionBreak = 1;
						break;
					}

					else if ((ball->_x == brickList[i]->_x + width) && (ball->TOP == brickList[i]->_y + height)) {
												
						if (ball->RIGHT_TOP) {
							ball->RIGHT_TOP = 0;
							ball->RIGHT_BOTTOM = 1;
						}
						else if (ball->LEFT_TOP) {
							ball->LEFT_TOP = 0;
							ball->LEFT_BOTTOM = 1;
						}

						brickList[i]->BrickHit(brickList, i);

						collisionBreak = 1;
						break;
					}

					else if ((ball->_x == brickList[i]->_x + width) && (ball->BOT == brickList[i]->_y + height)) {

						if (ball->RIGHT_BOTTOM) {
							ball->RIGHT_BOTTOM = 0;
							ball->RIGHT_TOP = 1;
						}
						else if (ball->LEFT_BOTTOM) {
							ball->LEFT_BOTTOM = 0;
							ball->LEFT_TOP = 1;
						}

						brickList[i]->BrickHit(brickList, i);

						collisionBreak = 1;
						break;
					}
				}
				if (collisionBreak) break;
			}
			if (collisionBreak) {
				score += 30;
				break;
			}
		}
		// brick destroy settings and ball collision

		for (int height = 0; height < myMortar->_height; height++)
		{
			for (int width = 0; width < myMortar->_width; width++)
			{
				if ((ball->_x == myMortar->_x + width) && (ball->BOT == myMortar->_y + height)) {
					if (ball->RIGHT_BOTTOM) {
						ball->RIGHT_BOTTOM = 0;
						ball->RIGHT_TOP = 1;
					}
					else if (ball->LEFT_BOTTOM) {
						ball->LEFT_BOTTOM = 0;
						ball->LEFT_TOP = 1;
					}
				}
			}
		}
		// myMortar collision 

		for (int i = 0; i < allKnownObjects.size(); i++)
		{
			if (allKnownObjects[i]->death) {
				allKnownObjects.erase(allKnownObjects.begin() + i);
			}
		}
		// delete object from vect if obj destroyed

		for (int i = 0; i < allKnownObjects.size(); i++)
		{
			allKnownObjects[i]->DrawObject();
		}
		// Draw all object

		for (int y = 0; y < ROWS; y++)
		{
			for (int x = 0; x < COLS; x++)
			{
				if (prevBuf[y][x] == wData.vBuf[y][x])
				{
					continue;
				}
				SetPos(x, y);
				cout << wData.vBuf[y][x];
			}
		}

		memcpy(prevBuf, wData.vBuf, ROWS * COLS);
		// double buffering output function

		DrawInfo();

		Sleep(30);
		tick++;

		if (level == 3) {
			worldIsRun = 0;
		}
	}

	if (level == 3) {
		SetPos(65, 22);
		cout << "CONGRATULATIONS! YOU WIN";
	}
	else {
		SetPos(70, 22);
		cout << "GAME OVER!";
		SetPos(70, 25);
		cout << "LEVEL: " << level << "/5";
	}
	SetPos(70, 24);
	cout << "SCORE: " << score;
	SetPos(65, 27);
	cout << "PRESS ENTER TO RESTART";
	SetPos(68, 28);
	cout << "PRESS ESC TO EXIT";

	bool pressed = false;

	do {
		if (GetAsyncKeyState(VK_RETURN)) {
			restart = true;
			pressed = true;
		}
		else if (GetAsyncKeyState(VK_ESCAPE)) {
			restart = false;
			pressed = true;
		}
	} while (!pressed);

	hotKeys.join();

	printf(CSI "?1049l"); // enable main buffer
}
