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
		cout << "��";
		gotoxy(24, i);
		cout << "��";
	}
	for (int i = 4; i <= 23; i += 2)
	{
		gotoxy(i, 1);
		cout << "��";
		gotoxy(i, 22);
		cout << "��";
	}
	for (int i = 24; i <= 34; i += 2)
	{
		gotoxy(i, 1);
		cout << "��";
		gotoxy(i, 7);
		cout << "��";
		gotoxy(i, 10);
		cout << "��";
		;
	}
	for (int i = 1; i <= 10; i++)
	{
		gotoxy(36, i);
		cout << "��";
	}
	gotoxy(26, 8);
	cout << "Score";
	gotoxy(26, 9);
	cout << pts;
}
void GameManager::playGame() //�� ���� �÷��� ������ �Լ� 
{
	roundFinish = false; //���� ���� ���� ������ ���� false�� �ʱ�ȭ  
	printField(); //���� �ʵ� ���  
	makeTetrimino(); //��Ʈ���̳� �����ϰ� ����  
	clock_t curKey[3]; //Ű�Է°� �����Ͽ� �ð� ������ ���� curKey,prevKey �迭 ���� 
	clock_t prevKey[3];
	clock_t curTime = clock(); //��� �������� �ӵ� ������ �ð� ������ ���� curTime,prevTime �ʱ�ȭ 
	clock_t prevTime = clock();
	FLOAT deltaTime; //��� �������� �ӵ� �����Ͽ� �ð� ��ȭ�� ������ ���� 
	FLOAT timeElapsed = 0.0f; //��� �������� �ӵ� �����Ͽ� ���� �ð� ������ ���� 
	updateField(); //���� �����ǰ� ���Ͽ� ��ȭ�� �κ� ����� 
	current->display(); //���� ��Ʈ���̳� ȭ�鿡 ��� 
	Tetrimino* shadow = initShadow(current, playField); //�׸��� �ʱ�ȭ 
	shadow->initField(playField); //�׸��� ��ġ �ʱ�ȭ 
	int keyM = 0, keyW = 0, keyX = 0; //ȸ�� �Է°� ������ ���� ��� 0���� �ʱ�ȭ 
	bool ifRotate = false;
	while (!roundFinish) //���� ������ ���� ���� while�� ���� 
	{
		for (int i = 0; i < 3; i++) //curKey,prevKey �� �ʱ�ȭ 
		{
			curKey[i] = clock();
			prevKey[i] = curKey[i];
		}
		while (1) //��� �������� ������ while�� ���� 
		{
			curTime = clock(); //���� �ð� curTime�� ����  
			deltaTime = static_cast<float>(curTime - prevTime) / (CLOCKS_PER_SEC); //���� �ð����� ��ȭ���� �ʷ� ȯ���Ͽ� deltaTime�� ����  
			prevTime = curTime; //���� �ð� ���� 
			timeElapsed += deltaTime; //���� �ð� ��� 
			for (int i = 0; i < 3; i++) //curKey �� ��� ���� �ð����� �ʱ�ȭ  
			{
				curKey[i] = clock();
			}
			if (GetAsyncKeyState(0x4D) & 0x8000 && !keyM) //�׸��� ���� �ѱ�
			{
				keyM = 1; //�� �� �������� keyM�� �����Ͽ� ���� �Է� ���� 
				if (shadowOn) //�׸��� �����־����� �׸��� ���� 
				{
					shadowOn = false; //�׸��� �� ���·� ����  
					shadow->deleteShadow(current); //�׸��� ȭ�鿡�� �����  
				}
				else //�׸��� ���� �־����� �׸��� �ѱ� 
				{
					shadowOn = true; //�׸��� �� ���·� ���� 
				}
			}
			else if (GetAsyncKeyState(0x4D) == 0) //MŰ ������ ���� ������ keyM�� 0���� ���� 
			{
				keyM = 0;
			}
			if (shadowOn) //�׸��� ���� �ִ� ���¶�� �׸��� ��� 
			{
				shadow->makeShadow(current, playField);
				current->display();
			}
			if (GetAsyncKeyState(0x41) & 0x8000 && current && (curKey[0] - prevKey[0] >= 100) && current->move("left", playField)) //aŰ, ���� �̵�
			{
				prevKey[0] = curKey[0]; //Ű �Է½ð� ����  
				current->display(); //���� ��Ʈ���̳� ��� 
				ifRotate = false;
			}
			if (GetAsyncKeyState(0x44) & 0x8001 && current && (curKey[1] - prevKey[1] >= 100) && current->move("right", playField)) //dŰ, ������ �̵�
			{
				prevKey[1] = curKey[1]; //Ű �Է½ð� ����  
				current->display(); //���� ��Ʈ���̳� ���  
				ifRotate = false;
			}
			if (GetAsyncKeyState(0x53) & 0x8001 && current && (curKey[2] - prevKey[2] >= 100) && current->move("down", playField)) //sŰ, ������ �̵�
			{
				prevKey[2] = curKey[2]; //Ű �Է½ð� ����  
				current->display(); //���� ��Ʈ���̳� ��� 
				ifRotate = false;
			}
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) //�����̽��� : �̳� �ٷ� ������
			{
				current->goBottom(playField, false); //���� ��Ʈ���̳� ���� ������ ������  
				current->display(); //���� ��Ʈ���̳� ���  
				roundFinish = true; //roundFinish �� true�� ���� �� while�� �������� 
				ifRotate = false;
				break;
			}
			if (GetAsyncKeyState(0x57) & 0x8000 && !keyW) //wŰ : �ð���� ȸ��
			{
				keyW = 1; //�� �� �������� keyW�� �����Ͽ� ���� �Է� ���� 
				current->rotate("w", playField); //���� ��Ʈ���̳� ȸ�� 
				current->display(); //���� ��Ʈ���̳� ���  
				ifRotate = true;
			}
			else if (GetAsyncKeyState(0x57) == 0) //wŰ ������ ���� ������ keyW�� 0���� ���� 
			{
				keyW = 0;
			}
			if (GetAsyncKeyState(0x58) & 0x8000 && !keyX) //xŰ : �ݽð���� ȸ��
			{
				keyX = 1; //�� �� �������� keyX�� �����Ͽ� ���� �Է� ���� 
				current->rotate("x", playField); //���� ��Ʈ���̳� ȸ�� 
				current->display(); //���� ��Ʈ���̳� ���   
				ifRotate = true;
			}
			else if (GetAsyncKeyState(0x58) == 0)  //xŰ ������ ���� ������ keyX�� 0���� ���� 
			{
				keyX = 0;
			}
			if (timeElapsed > speed) //���� �ð� ������ while�� break�Ͽ� �� block ���������� ��  
			{
				timeElapsed = 0.0f; //���� �ð� �ٽ� �ʱ�ȭ 
				break;
			}
		}
		if (current->move("down", playField)) //��� ���� 
		{
			current->display(); //���� ��Ʈ���̳� ���  
			ifRotate = false;
		}
		else //�� �̻� ������ �� ������ ���� ���� 
		{
			roundFinish = true;
		}
	}
	if (shadowOn) //�׸��� ���� �־��ٸ� ȭ�鿡�� �����  
	{
		shadow->deleteShadow(current);
	}
	current->fillField(playField); //���� ��Ʈ���̳� playfield�� �ݿ�  
	updateField();
	delete shadow; //���� �Ҵ� ����  
	deleteLine(ifRotate); //���� �� �ִ��� �˻�  
	updateField(); //���� ���¿� ��ȭ�� ȭ�鿡�� ����� �� �����  
	Sleep(200); //sleep �Լ� ���� ���帶�� ���� �ð� �� 
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
				prevPlayField[i][j] = playField[i][j]; //������Ʈ
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
		if (!minoPack[num]) //���ο� num
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
	for (int i = 0; i < 7; i++) //refresh���� �˻�
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
		playField[x][y] = NULL; //playField ���� NULL�� ����
	}
	for (int i = 0; i < tetNum; i++) //block ����
	{
		tetList[i]->deleteBlock(y);
		if (!tetList[i]->getNum()) //block �ٻ������ ����
		{
			Tetrimino* del = tetList[i];
			for (int j = i; j < tetNum - 1; j++) //�迭 �մ���
			{
				tetList[j] = tetList[j + 1];
			}
			delete del;
			tetNum--;
			i--; //loop �ٽ� ������ ��
		}
	}
	for (int i = 1; i <= 10; i++) //x�ึ�� ����
	{
		for (int j = y- 1; j >= 1; j--) //y����ǥ del[0]-1���� 1�� ������
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
			y++; //�ٽ� �˻��ϵ���
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
	if (num >= 1) //������ ����
	{
		if (num == 4 || tspin) //��Ʈ���� or Ƽ���� ����
		{
			if (backToBack) //������ ��Ȳ�̸� ���� 200�� �߰�
			{
				pts += 200;
			}
			backToBack = true;
		}
		else //���� ��Ȳ �ƴϸ� ������ ����
		{
			backToBack = false;
		}
	}
	gotoxy(26, 9);
	cout << pts;
	//�ӵ� ����
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