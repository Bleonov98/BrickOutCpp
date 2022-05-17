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

	int level = 0;
	int tick = 1;
	score = 0;

	MyMortar* myMortar = new MyMortar(&wData, 13, 2, COLS / 2 - 6, ROWS - 9, '*');
	Ball* ball = new Ball(&wData, 1, 1, myMortar->_x + myMortar->_width/2, myMortar->_y - 1, '@');

	allKnownObjects.push_back(myMortar);
	allKnownObjects.push_back(ball);

	Brick* brick;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < COLS / 8; j++)
		{
			brick = new Brick(&wData, 7, 2, 4 + (8 * j), 2 + (3 * i), '#');
			allKnownObjects.push_back(brick);
			brickList.push_back(brick);
		}
	}

	while (worldIsRun)
	{
		if (started)
		{
			//if (!ball->ballGo) {
			//	ball->ballGo = true;
			//	ball->BuildTrajectory(COLS, 11);
			//}

			if (tick % 2 == 0) {
				ball->BallMove();
			}
			myMortar->MoveMyMortar();
		}
		// move and direction settings

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


		Sleep(10);
		tick++;
	}

	if (level == 10) {
		SetPos(65, 22);
		cout << "CONGRATULATIONS! YOU WIN";
	}
	else {
		SetPos(70, 22);
		cout << "GAME OVER";
		SetPos(68, 25);
		cout << "LEVEL: " << level << "/10";
	}
	SetPos(70, 24);
	cout << "SCORE:" << score;
	SetPos(65, 28);
	cout << "PRESS ENTER TO RESTART";
	SetPos(65, 29);
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