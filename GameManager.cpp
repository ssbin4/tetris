#include <iostream>
#include <Windows.h>
#include <ctime>
#include <cstdlib>
#include <stdio.h>
#include "Tetrimino.h"
#include "Block.h"
#include "GameManager.h"

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

using namespace std;
Tetrimino* initShadow(Tetrimino* current, Block* (*playField)[21]);
bool isLineComplete(int y, Block* (*playField)[21]);
GameManager::GameManager()
{
	speed = 1.0f;
	tetNum = 0;
	pts = 0;
	combo = -1;
	backToBack = false;
	shadowOn = false;
	for (int i = 0; i < 7; i++)
	{
		minoPack[i] = 0;
	}
	for (int i = 1; i <= 10; i++)
	{
		for (int j = 1; j <= 20; j++)
		{
			playField[i][j] = NULL;
			prevPlayField[i][j] = NULL;
		}
	}
	nextNum = rand() % 7;
	minoPack[nextNum] = 1;
	current = tetList[0];
}
void GameManager::makeFrame()
{
	for (int i = 1; i <= 22; i++)
	{
		gotoxy(2, i);
		cout << "▦";
		gotoxy(24, i);
		cout << "▦";
	}
	for (int i = 4; i <= 23; i += 2)
	{
		gotoxy(i, 1);
		cout << "▦";
		gotoxy(i, 22);
		cout << "▦";
	}
	for (int i = 24; i <= 34; i += 2)
	{
		gotoxy(i, 1);
		cout << "▦";
		gotoxy(i, 7);
		cout << "▦";
		gotoxy(i, 10);
		cout << "▦";
		;
	}
	for (int i = 1; i <= 10; i++)
	{
		gotoxy(36, i);
		cout << "▦";
	}
	gotoxy(26, 8);
	cout << "Score";
	gotoxy(26, 9);
	cout << pts;
}
void GameManager::playGame() //한 블럭당 플레이 진행할 함수 
{
	roundFinish = false; //라운드 끝날 상태 저장할 변수 false로 초기화  
	printField(); //현재 필드 출력  
	makeTetrimino(); //테트리미노 랜덤하게 생성  
	clock_t curKey[3]; //키입력과 관련하여 시간 저장할 변수 curKey,prevKey 배열 생성 
	clock_t prevKey[3];
	clock_t curTime = clock(); //블록 떨어지는 속도 관련한 시간 저장할 변수 curTime,prevTime 초기화 
	clock_t prevTime = clock();
	FLOAT deltaTime; //블록 떨어지는 속도 관련하여 시간 변화량 저장할 변수 
	FLOAT timeElapsed = 0.0f; //블록 떨어지는 속도 관련하여 누적 시간 저장할 변수 
	updateField(); //이전 게임판과 비교하여 변화된 부분 지우기 
	current->display(); //현재 테트리미노 화면에 출력 
	Tetrimino* shadow = initShadow(current, playField); //그림자 초기화 
	shadow->initField(playField); //그림자 위치 초기화 
	int keyM = 0, keyW = 0, keyX = 0; //회전 입력과 관련한 변수 모두 0으로 초기화 
	bool ifRotate = false;
	while (!roundFinish) //라운드 끝나지 않은 동안 while문 실행 
	{
		for (int i = 0; i < 3; i++) //curKey,prevKey 값 초기화 
		{
			curKey[i] = clock();
			prevKey[i] = curKey[i];
		}
		while (1) //블록 떨어지기 전까지 while문 실행 
		{
			curTime = clock(); //현재 시간 curTime에 저장  
			deltaTime = static_cast<float>(curTime - prevTime) / (CLOCKS_PER_SEC); //이전 시간과의 변화량을 초로 환산하여 deltaTime에 저장  
			prevTime = curTime; //이전 시간 저장 
			timeElapsed += deltaTime; //누적 시간 계산 
			for (int i = 0; i < 3; i++) //curKey 값 모두 현재 시간으로 초기화  
			{
				curKey[i] = clock();
			}
			if (GetAsyncKeyState(0x4D) & 0x8000 && !keyM) //그림자 끄고 켜기
			{
				keyM = 1; //한 번 눌렀으면 keyM값 변경하여 연속 입력 막기 
				if (shadowOn) //그림자 켜져있었으면 그림자 끄기 
				{
					shadowOn = false; //그림자 끈 상태로 저장  
					shadow->deleteShadow(current); //그림자 화면에서 지우기  
				}
				else //그림자 꺼져 있었으면 그림자 켜기 
				{
					shadowOn = true; //그림자 켠 상태로 저장 
				}
			}
			else if (GetAsyncKeyState(0x4D) == 0) //M키 누르고 있지 않으면 keyM값 0으로 변경 
			{
				keyM = 0;
			}
			if (shadowOn) //그림자 켜져 있는 상태라면 그림자 출력 
			{
				shadow->makeShadow(current, playField);
				current->display();
			}
			if (GetAsyncKeyState(0x41) & 0x8000 && current && (curKey[0] - prevKey[0] >= 100) && current->move("left", playField)) //a키, 왼쪽 이동
			{
				prevKey[0] = curKey[0]; //키 입력시간 저장  
				current->display(); //현재 테트리미노 출력 
				ifRotate = false;
			}
			if (GetAsyncKeyState(0x44) & 0x8001 && current && (curKey[1] - prevKey[1] >= 100) && current->move("right", playField)) //d키, 오른쪽 이동
			{
				prevKey[1] = curKey[1]; //키 입력시간 저장  
				current->display(); //현재 테트리미노 출력  
				ifRotate = false;
			}
			if (GetAsyncKeyState(0x53) & 0x8001 && current && (curKey[2] - prevKey[2] >= 100) && current->move("down", playField)) //s키, 밑으로 이동
			{
				prevKey[2] = curKey[2]; //키 입력시간 저장  
				current->display(); //현재 테트리미노 출력 
				ifRotate = false;
			}
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) //스페이스바 : 미노 바로 내리기
			{
				current->goBottom(playField, false); //현재 테트리미노 가장 밑으로 내리기  
				current->display(); //현재 테트리미노 출력  
				roundFinish = true; //roundFinish 값 true로 변경 후 while문 빠져나감 
				ifRotate = false;
				break;
			}
			if (GetAsyncKeyState(0x57) & 0x8000 && !keyW) //w키 : 시계방향 회전
			{
				keyW = 1; //한 번 눌렀으면 keyW값 변경하여 연속 입력 막기 
				current->rotate("w", playField); //현재 테트리미노 회전 
				current->display(); //현재 테트리미노 출력  
				ifRotate = true;
			}
			else if (GetAsyncKeyState(0x57) == 0) //w키 누르고 있지 않으면 keyW값 0으로 변경 
			{
				keyW = 0;
			}
			if (GetAsyncKeyState(0x58) & 0x8000 && !keyX) //x키 : 반시계방향 회전
			{
				keyX = 1; //한 번 눌렀으면 keyX값 변경하여 연속 입력 막기 
				current->rotate("x", playField); //현재 테트리미노 회전 
				current->display(); //현재 테트리미노 출력   
				ifRotate = true;
			}
			else if (GetAsyncKeyState(0x58) == 0)  //x키 누르고 있지 않으면 keyX값 0으로 변경 
			{
				keyX = 0;
			}
			if (timeElapsed > speed) //일정 시간 지나면 while문 break하여 새 block 떨어지도록 함  
			{
				timeElapsed = 0.0f; //누적 시간 다시 초기화 
				break;
			}
		}
		if (current->move("down", playField)) //블록 낙하 
		{
			current->display(); //현재 테트리미노 출력  
			ifRotate = false;
		}
		else //더 이상 낙하할 수 없으면 라운드 종료 
		{
			roundFinish = true;
		}
	}
	if (shadowOn) //그림자 켜져 있었다면 화면에서 지우기  
	{
		shadow->deleteShadow(current);
	}
	current->fillField(playField); //현재 테트리미노 playfield에 반영  
	updateField();
	delete shadow; //동적 할당 해제  
	deleteLine(ifRotate); //날릴 줄 있는지 검사  
	updateField(); //이전 상태와 비교화여 화면에서 변경된 값 지우기  
	Sleep(200); //sleep 함수 통해 라운드마다 지연 시간 둠 
}
void GameManager::updateField()
{
	for (int i = 1; i <= 10; i++)
	{
		for (int j = 1; j <= 20; j++)
		{
			if (playField[i][j] != prevPlayField[i][j])
			{
				gotoxy(2 * i + 2, j + 1);
				cout << "  ";
				prevPlayField[i][j] = playField[i][j]; //업데이트
			}
		}
	}
}
void GameManager::printField() const
{
	for (int i = 0; i < tetNum; i++)
	{
		tetList[i]->display();
	}
}
void GameManager::makeTetrimino()
{
	switch (nextNum)
	{
	case 0:
		tetList[tetNum++] = new Tetrimino_I();
		break;
	case 1:
		tetList[tetNum++] = new Tetrimino_O();
		break;
	case 2:
		tetList[tetNum++] = new Tetrimino_T();
		break;
	case 3:
		tetList[tetNum++] = new Tetrimino_L();
		break;
	case 4:
		tetList[tetNum++] = new Tetrimino_J();
		break;
	case 5:
		tetList[tetNum++] = new Tetrimino_S();
		break;
	case 6:
		tetList[tetNum++] = new Tetrimino_Z();
		break;
	}
	current = tetList[tetNum - 1];
	current->initField(playField);
	int num;
	while (1)
	{
		num = rand() % 7;
		if (!minoPack[num]) //새로운 num
		{
			minoPack[num] = 1;
			nextNum = num;
			break;
		}
	}
	switch (nextNum) //print next Mino
	{
	case 0:
	{
		Tetrimino_I temp;
		temp.displayNext();
		break;
	}
	case 1:
	{
		Tetrimino_O temp;
		temp.displayNext();
		break;
	}
	case 2:
	{
		Tetrimino_T temp;
		temp.displayNext();
		break;
	}
	case 3:
	{
		Tetrimino_L temp;
		temp.displayNext();
		break;
	}
	case 4:
	{
		Tetrimino_J temp;
		temp.displayNext();
		break;
	}
	case 5:
	{
		Tetrimino_S temp;
		temp.displayNext();
		break;
	}
	case 6:
	{
		Tetrimino_Z temp;
		temp.displayNext();
		break;
	}
	}
	for (int i = 0; i < 7; i++) //refresh할지 검사
	{
		if (!minoPack[i])
		{
			return;
		}
	}
	for (int i = 0; i < 7; i++) //refresh minopack
	{
		minoPack[i] = 0;
	}
}
bool isLineComplete(int y, Block* (*playField)[21])
{
	for (int x = 1; x <= 10; x++)
	{
		if (!playField[x][y])
		{
			return false;
		}
	}
	return true;
}
void GameManager::rearrange(int y)
{
	for (int x = 1; x <= 10; x++)
	{
		playField[x][y] = NULL; //playField 정보 NULL로 저장
	}
	for (int i = 0; i < tetNum; i++) //block 삭제
	{
		tetList[i]->deleteBlock(y);
		if (!tetList[i]->getNum()) //block 다사라지면 삭제
		{
			Tetrimino* del = tetList[i];
			for (int j = i; j < tetNum - 1; j++) //배열 앞당기기
			{
				tetList[j] = tetList[j + 1];
			}
			delete del;
			tetNum--;
			i--; //loop 다시 돌도록 함
		}
	}
	for (int i = 1; i <= 10; i++) //x축마다 실행
	{
		for (int j = y- 1; j >= 1; j--) //y축좌표 del[0]-1부터 1씩 내리기
		{
			if (!playField[i][j])
			{
				continue;
			}
			else
			{
				playField[i][j]->setY(playField[i][j]->getY() + 1);
				playField[i][j + 1] = playField[i][j];
				playField[i][j] = NULL;
			}
		}
	}
}
void GameManager::deleteLine(bool ifRotate)
{
	int tspin;
	if (ifRotate)
	{
		tspin = current->detectTspin(playField);
	}
	else
	{
		tspin = 0;
	}
	int num = 0;
	for (int y = 20; y >= 1; y--)
	{
		if (isLineComplete(y, playField))
		{
			num++;
			rearrange(y);
			y++; //다시 검사하도록
		}
	}
	switch (num)
	{
	case 0:
		combo = -1;
		return;
	case 1:
	{
		if (tspin == 2) //tspin single
		{
			pts += 500;
		}
		else
		{
			pts += 100;
		}
		break;
	}
	case 2:
	{
		if (tspin) //tspin double
		{
			pts += 1000;
		}
		else
		{
			pts += 300;
		}
		break;
	}
	case 3:
	{
		if (tspin) //tspin triple
		{
			pts += 1500;
		}
		else
		{
			pts += 500;
		}
		break;
	}
	case 4:
		pts += 1000;
		break;
	}
	combo++;
	if (combo >= 2)
	{
		pts += 100 * (combo - 1);
	}
	if (num >= 1) //백투백 점검
	{
		if (num == 4 || tspin) //테트리스 or 티스핀 성공
		{
			if (backToBack) //백투백 상황이면 점수 200점 추가
			{
				pts += 200;
			}
			backToBack = true;
		}
		else //위의 상황 아니면 백투백 끊김
		{
			backToBack = false;
		}
	}
	gotoxy(26, 9);
	cout << pts;
	//속도 조절
	speed = 0.5f / float(1 + 0.1 * (pts / 1000));
}

bool GameManager::isGameFinish()
{
	for (int i = 0; i < tetNum; i++)
	{
		if (tetList[i]->isGameOver())
		{
			return true;
		}
	}
	return false;
}

void GameManager::deleteAll()
{
	for (int i = 0; i < tetNum; i++)
	{
		delete tetList[i];
	}
}

Tetrimino* initShadow(Tetrimino* current, Block* (*playField)[21])
{
	char alphabet = current->getAlphabet();
	Tetrimino* shadow;
	switch (alphabet)
	{
	case 'I':
		shadow = new Tetrimino_I();
		shadow->goBottom(playField, true);
		return shadow;
	case 'O':
		shadow = new Tetrimino_O();
		shadow->goBottom(playField, true);
		return shadow;
	case 'T':
		shadow = new Tetrimino_T();
		shadow->goBottom(playField, true);
		return shadow;
	case 'L':
		shadow = new Tetrimino_L();
		shadow->goBottom(playField, true);
		return shadow;
	case 'J':
		shadow = new Tetrimino_J();
		shadow->goBottom(playField, true);
		return shadow;
	case 'S':
		shadow = new Tetrimino_S();
		shadow->goBottom(playField, true);
		return shadow;
	case 'Z':
		shadow = new Tetrimino_Z();
		shadow->goBottom(playField, true);
		return shadow;
	}
}