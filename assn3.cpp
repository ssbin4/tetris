#include <iostream>
#include <stdlib.h>
#include <Windows.h>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include "Block.h"
#include "Tetrimino.h"
#include "GameManager.h"

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

using namespace std;

void CursorView(char show);
int main()
{
	srand((unsigned)time(NULL));
	char input;
	ofstream writeFile;
	writeFile.open("score.txt");
	int pts[100]; //점수 저장 배열
	int writeNum = 0; //저장할 횟수
	string trash;
	while (1)
	{
		system("cls");
		GameManager tetris;
		tetris.makeFrame();
		CursorView(0);
		while (!tetris.isGameFinish())
		{
			tetris.playGame();
		}
		tetris.deleteAll();
		system("cls");
		getline(cin, trash);
		cout << "Do you want to save your score? (y/n)";
		cin >> input;
		if (input == 'y')
		{
			pts[writeNum++] = tetris.getPts();
			for (int i = writeNum - 1; i >= 0; i--)
			{
				writeFile << pts[i]<<endl;
			}
		}
		cout << "Do you want to restart? (y/n)";
		cin >> input;
		if (input == 'n')
		{
			break;
		}
	}
	return 0;
}

void CursorView(char show)//커서숨기기
{
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}