#include "World.h"

void World::SetPos(int x, int y)
{
	sprintf_s(coord, "%s%d;%dH", CSI, y, x);
	printf(coord);
}

void World::HotKeys()
{
	while (worldIsRun) {
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			worldIsRun = false;
		}
		if (GetKeyState(0x50)) {
			pause = !pause;
		}
		if (GetAsyncKeyState(VK_SPACE)) {
			started = true;
		}
		this_thread::sleep_for(milliseconds(10));
	}
}

void World::DrawInfo()
{
	SetPos(17, 52);
	cout << score;
	SetPos(17, 53);
	cout << level + 1;
}

void World::DrawTitle() {

	PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);

	HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(IDR_TEXT2), L"TEXT");

	if (hResource)
	{
		HGLOBAL hLoadedResource = LoadResource(hInstance, hResource);

		if (hLoadedResource)
		{
			LPCSTR title = (LPCSTR)LockResource(hLoadedResource);

			if (title)
			{
				DWORD dwResourceSize = SizeofResource(hInstance, hResource);

				if (0 != dwResourceSize)
				{
					int j = 0;
					int k = 1;
					for (int i = 0; i < 55; i++)
					{
						for (; j < 150.95 * k; j++)
						{
							cout << title[j];
						}
						k++;
						Sleep(20);
					}
				}
			}
		}
	}

	while (!GetAsyncKeyState(VK_RETURN)) {
		SetPos(67, 42);
		cout << "PRESS ENTER TO START";
		Sleep(650);
		SetPos(67, 42);
		cout << "                    ";
		Sleep(650);
	}

	PlaySound(NULL, 0, 0);

	system("cls");
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
					for (int i = 0; i < strnlen(area, 9500); i++) {
						cout << area[i];
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

	DrawTitle();
	DrawArea();
}

void World::RunWorld(bool& restart)
{
	srand(time(NULL));
	CreateWorld();
	
	thread hotKeys([&]
		{ HotKeys(); }
	);

	level = 0;
	score = 0;
	int tick = 1;
	
	MyMortar* myMortar = new MyMortar(&wData, 7, 2, COLS / 2 - 6, ROWS - 9, FOREGROUND_GREEN | FOREGROUND_RED, u'*');
	Ball* ball = new Ball(&wData, 1, 1, myMortar->_x + myMortar->_width/2, myMortar->_y - 1, FOREGROUND_RED, u'@');
	Bonus* bonusLife = new Bonus;
	Bonus* bonusSize = new Bonus;
	Bonus* penaltySize = new Bonus;

	allKnownObjects.push_back(myMortar);
	allKnownObjects.push_back(ball);

	Brick* brick;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < COLS / 8; j++)
		{
			brick = new Brick(&wData, 7, 2, 4 + (8 * j), 15 + (8 * i), FOREGROUND_GREEN, u'#');
			allKnownObjects.push_back(brick);
			brickList.push_back(brick);
		}
	}

	while (worldIsRun)
	{
		if (pause) {
			SetPos(COLS / 2, ROWS / 2);
			cout << "PAUSED";
			while (pause) {

			}
			SetPos(COLS / 2, ROWS / 2);
			cout << "      ";
		}

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

			PlaySound(MAKEINTRESOURCE(IDR_WAVE5), NULL, SND_RESOURCE | SND_ASYNC);

			Sleep(2000);

			ball->LEFT_BOTTOM = 0;
			ball->LEFT_TOP = 0;
			ball->RIGHT_TOP = 1;
			ball->RIGHT_BOTTOM = 0;
				
			ball->_x = myMortar->_x + myMortar->_width / 2;
			ball->_y = myMortar->_y - 1;

			started = false;
			}
		}

		myMortar->MoveMyMortar();

		if (!started) {
			ball->EraseObject();
			ball->_x = myMortar->_x + myMortar->_width / 2;
			ball->_y = myMortar->_y - 1;
		}
		// move and direction settings
		

		if (brickList.empty()) {
			ball->EraseObject();

			started = false;

			level++;

			if (level == 1) {
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < COLS / 8; j++)
					{
						brick = new Brick(&wData, 7, 2, 4 + (8 * j), 2 + (15 * i), FOREGROUND_GREEN, '#');
						allKnownObjects.push_back(brick);
						brickList.push_back(brick);
					}
				}
			}

			else if (level == 2) {
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < COLS / 8; j++)
					{
						brick = new Brick(&wData, 7, 2, 4 + (8 * j), 2 + (4 * i), FOREGROUND_GREEN, '#');
						allKnownObjects.push_back(brick);
						brickList.push_back(brick);
					}
				}
			}

			else if (level == 3) {
				break;
			}
		}

		// ------------------------------(1)
		if ((bonusList.empty()) && (!brickList.empty())) {
			if ((score % 1200 == 0) && (score > 0) && (myMortar->lifes <= 2)) {
				PlaySound(MAKEINTRESOURCE(IDR_WAVE6), NULL, SND_RESOURCE | SND_ASYNC);

				int randBrickNum = rand() % (brickList.size() - 1);

				bonusLife = new Bonus(&wData, 1, 1, brickList[randBrickNum]->_x + brickList[randBrickNum]->_width / 2, brickList[randBrickNum]->_y + 2, FOREGROUND_RED, 'L');

				bonusList.push_back(bonusLife);
				allKnownObjects.push_back(bonusLife);

				score += 30;
			}

			if (score % 450 == 0 && score > 0) {
				PlaySound(MAKEINTRESOURCE(IDR_WAVE6), NULL, SND_RESOURCE | SND_ASYNC);

				if (myMortar->_width <= 15) {
					int randBrickNum = rand() % (brickList.size() - 1);

					bonusSize = new Bonus(&wData, 1, 1, brickList[randBrickNum]->_x + brickList[randBrickNum]->_width / 2, brickList[randBrickNum]->_y + 2, FOREGROUND_RED, '+');

					bonusList.push_back(bonusSize);
					allKnownObjects.push_back(bonusSize);

					score += 30;
				}

				else if (myMortar->_width > 15) {
					int randBrickNum = rand() % (brickList.size() - 1);

					penaltySize = new Bonus(&wData, 1, 1, brickList[randBrickNum]->_x + brickList[randBrickNum]->_width / 2, brickList[randBrickNum]->_y + 2, FOREGROUND_RED, '-');

					bonusList.push_back(penaltySize);
					allKnownObjects.push_back(penaltySize);

					score += 30;
				}
			}
		}
		//bonusLife && bonusSize settings(2)

		if (!bonusList.empty()) {

		bool breakBonus = false;

			for (int height = 0; height < myMortar->_height; height++) {

				for (int width = 0; width < myMortar->_width; width++) {

					if ((myMortar->_x + width == bonusLife->_x) && (myMortar->_y + height == bonusLife->_y) && (myMortar->lifes < 3) && (!bonusLife->alreadyDone)) {

						PlaySound(MAKEINTRESOURCE(IDR_WAVE7), NULL, SND_RESOURCE | SND_ASYNC);

							if (myMortar->lifes == 1) {
								SetPos(52, 51);
								cout << "@@ @@";
								SetPos(52, 52);
								cout << "@ @ @";
								SetPos(52, 53);
								cout << " @ @ ";
								SetPos(52, 54);
								cout << "  @  ";

								score += 30;
								myMortar->lifes++;

								bonusLife->death = 1;
								bonusLife->alreadyDone = 1;

								breakBonus = 1;
								break;
							}

							else if (myMortar->lifes == 2) {
								SetPos(62, 51);
								cout << "@@ @@";
								SetPos(62, 52);
								cout << "@ @ @";
								SetPos(62, 53);
								cout << " @ @ ";
								SetPos(62, 54);
								cout << "  @  ";

								score += 30;
								myMortar->lifes++;

								bonusLife->death = 1;
								bonusLife->alreadyDone = 1;

								breakBonus = 1;
								break;
							}
						}

					if ((myMortar->_x + width == bonusSize->_x) && (myMortar->_y + height == bonusSize->_y) && (!bonusSize->alreadyDone)) {
						PlaySound(MAKEINTRESOURCE(IDR_WAVE7), NULL, SND_RESOURCE | SND_ASYNC);

						myMortar->_width += 1;

						bonusSize->death = 1;
						bonusSize->alreadyDone = 1;
						bonusSize->EraseObject();

						score += 30;

						breakBonus = 1;
						break;
					}

					if ((myMortar->_x + width == penaltySize->_x) && (myMortar->_y + height == penaltySize->_y) && (!penaltySize->alreadyDone)) {
						PlaySound(MAKEINTRESOURCE(IDR_WAVE7), NULL, SND_RESOURCE | SND_ASYNC);

						myMortar->EraseObject();
						myMortar->_width -= 6;

						penaltySize->death = 1;
						penaltySize->alreadyDone = 1;
						penaltySize->EraseObject();

						score += 30;

						breakBonus = 1;
						break;
					}
				
					if (breakBonus) break;
				}
				if (breakBonus) break;
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

					if (started) {
						PlaySound(MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_RESOURCE | SND_ASYNC);
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
				if (prevBuf[y][x] != wData.vBuf[y][x])
				{
					COORD coord;

					coord.X = x;
					coord.Y = y;

					prevBuf[y][x] = wData.vBuf[y][x];

					SetPos(x, y);

					if ((prevBuf[y][x] >> 8) == FOREGROUND_GREEN) {
						printf(CSI "22;32m");
					}
					else if ((prevBuf[y][x] >> 8) == FOREGROUND_RED) {
						printf(CSI "22;31m");
					}
					else if ((prevBuf[y][x] >> 8) == (FOREGROUND_RED | FOREGROUND_GREEN)) {
						printf(CSI "22;33m");
					}
					/*else if ((prevBuf[y][x] >> 8) == () {
						printf(CSI "22;31m");
					}*/

					cout << char(prevBuf[y][x]);

					printf(CSI "1;0m");
				}
			}
		}
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
		cout << "LEVEL: " << level + 1 << "/3";
	}
	SetPos(70, 24);
	cout << "SCORE: " << score;
	SetPos(65, 27);
	cout << "PRESS ENTER TO RESTART";
	SetPos(68, 28);
	cout << "PRESS ESC TO EXIT";

	pressed = false;
	restart = false;

	while (!pressed) {
		if (GetAsyncKeyState(VK_RETURN)) {
			restart = true;
			pressed = true;
		}
		else if (GetAsyncKeyState(VK_ESCAPE)) {
			restart = false;
			pressed = true;
		}
	}

	hotKeys.join();

	printf(CSI "?1049l"); 
}
