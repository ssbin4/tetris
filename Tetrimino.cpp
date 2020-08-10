#include <iostream>
#include "Tetrimino.h"
#include "Block.h"
#include <Windows.h>

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

using namespace std;

bool isCrashed(Pos* nextPos, Block* (*playField)[21]);

int findMin(int a, int b, int c);
int findMin(int a, int b);
Pos getWallkick(Tetrimino* current, string direction, int test);

Tetrimino::Tetrimino()
{
	num = 4;
	rotationState = 0;
}
void Tetrimino::display() const
{
	for (int i = 0; i < num; i++)
	{
		if (blocks[i]->getY() < 1)
		{
			continue;
		}
		blocks[i]->display();
	}
}
void Tetrimino::displayNext() const
{
	for (int j = 2; j <= 6; j++)
	{
		for (int i = 26; i <= 34; i += 2)
		{
			gotoxy(i, j);
			cout << "  ";
		}
	}
	Block* copy[4];
	for (int i = 0; i < 4; i++)
	{
		copy[i] = blocks[i];
		copy[i]->setX(blocks[i]->getX() + 9);
		copy[i]->setY(blocks[i]->getY() + 2);
		copy[i]->display();
	}
}
void Tetrimino::initField(Block* (*playField)[21])
{
	for (int i = 0; i < 4; i++) //겹치면 2칸 위로 올림
	{
		if (playField[blocks[i]->getX()][blocks[i]->getY()]) //채워져 있는 칸일때
		{
			break;
		}
		if (i == 3)
		{
			return;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		blocks[i]->setY(blocks[i]->getY() - 2);
	}
}
void Tetrimino::fillField(Block* (*playField)[21])
{
	for (int i = 0; i < 4; i++)
	{
		if (blocks[i]->getY() < 1) //y<1인 블록은 무시
		{
			continue;
		}
		playField[blocks[i]->getX()][blocks[i]->getY()] = blocks[i];
	}
}
void Tetrimino::deleteBlock(int y)
{
	for (int i = 0; i < num; i++)
	{
		if (blocks[i]->getY() == y) //삭제할 줄일 때
		{
			Block* del = blocks[i];
			for (int j = i; j < num - 1; j++) //배열 앞당기기
			{
				blocks[j] = blocks[j + 1];
			}
			blocks[num - 1] = NULL;
			delete del;
			num--;
			i--; //loop 다시 돌도록 함
		}
	}
}
Tetrimino_I::Tetrimino_I()
{
	alphabet = 'I';
	for (int i = 0; i < 4; i++)
	{
		blocks[i] = new Block(10 + 2 * i, 2, CYAN);
	}
}
Tetrimino_O::Tetrimino_O()
{
	alphabet = 'O';
	for (int i = 0; i < 2; i++)
	{
		blocks[i] = new Block(12 + 2 * i, 2, YELLOW);
		blocks[i + 2] = new Block(12 + 2 * i, 3, YELLOW);
	}
}
Tetrimino_T::Tetrimino_T()
{
	alphabet = 'T';
	blocks[0] = new Block(12, 2, MAGENTA);
	for (int i = 1; i < 4; i++)
	{
		blocks[i] = new Block(8 + 2 * i, 3, MAGENTA);
	}
}
Tetrimino_L::Tetrimino_L()
{
	alphabet = 'L';
	blocks[0] = new Block(14, 2, WHITE);
	for (int i = 1; i < 4; i++)
	{
		blocks[i] = new Block(8 + 2 * i, 3, WHITE);
	}

}
Tetrimino_J::Tetrimino_J()
{
	alphabet = 'J';
	blocks[0] = new Block(10, 2, BLUE);
	for (int i = 1; i < 4; i++)
	{
		blocks[i] = new Block(8 + 2 * i, 3, BLUE);
	}

}
Tetrimino_S::Tetrimino_S()
{
	alphabet = 'S';
	blocks[0] = new Block(12, 2, GREEN);
	blocks[1] = new Block(14, 2, GREEN);
	blocks[2] = new Block(10, 3, GREEN);
	blocks[3] = new Block(12, 3, GREEN);
}
Tetrimino_Z::Tetrimino_Z()
{
	alphabet = 'Z';
	blocks[0] = new Block(10, 2, RED);
blocks[1] = new Block(12, 2, RED);
blocks[2] = new Block(12, 3, RED);
blocks[3] = new Block(14, 3, RED);
}
bool Tetrimino::move(string direction, Block* (*playField)[21])
{
	Pos nextPos[4];
	if (direction == "down")
	{
		for (int i = 0; i < 4; i++)
		{
			nextPos[i].x = blocks[i]->getX();
			nextPos[i].y = blocks[i]->getY() + 1;
		}
	}
	else if (direction == "left")
	{
		for (int i = 0; i < 4; i++)
		{
			nextPos[i].x = blocks[i]->getX()-1;
			nextPos[i].y = blocks[i]->getY();
		}
	}
	else if (direction == "right")
	{
		for (int i = 0; i < 4; i++)
		{
			nextPos[i].x = blocks[i]->getX()+1;
			nextPos[i].y = blocks[i]->getY();
		}
	}
	if (isCrashed(nextPos, playField)) //when crash occurs
	{
		return false;
	}
	cleanScreen(nextPos,true);
	for (int i = 0; i < 4; i++) //setting new value
	{
		blocks[i]->setX(nextPos[i].x);
		blocks[i]->setY(nextPos[i].y);
	}
	return true;
}

void Tetrimino::goBottom(Block* (*playField)[21],bool shadow)
{
	if (!shadow)
	{
		for (int i = 0; i < 4; i++)
		{
			if (blocks[i]->getY() < 1)
			{
				continue;
			}
			gotoxy(2 * blocks[i]->getX() + 2, blocks[i]->getY() + 1);
			cout << "  ";
		}
	}
	while (1)
	{
		for (int i = 0; i < 4; i++)
		{
			if (playField[blocks[i]->getX()][blocks[i]->getY()+1] || blocks[i]->getY() + 1 > 20)
			{
				return;
			}
		}
		for (int i = 0; i < 4; i++)
		{
			blocks[i]->setY(blocks[i]->getY() + 1);
		}
	}
}

void Tetrimino::makeShadow(Tetrimino* current, Block* (*playField)[21])
{
	Pos prevPos[4];
	for (int i = 0; i < 4; i++)
	{
		prevPos[i].x = this->blocks[i]->getX();
		prevPos[i].y = this->blocks[i]->getY();
		blocks[i]->setX(current->blocks[i]->getX());
		blocks[i]->setY(current->blocks[i]->getY());
		blocks[i]->setColor(current->blocks[i]->getColor());
		this->blocks[i]->setMsg("□");
	}
	this->goBottom(playField,true);
	cleanScreen(prevPos, false);
	for (int i = 0; i < 4; i++)
	{
		int j;
		for (j = 0; j < 4; j++)
		{
			if (this->blocks[i]->getX() == current->blocks[j]->getX() && this->blocks[i]->getY() == current->blocks[j]->getY())
			{
				break;
			}
		}
		if (blocks[i]->getY() < 1)
		{
			continue;
		}
		if (j == 4)
		{
			blocks[i]->display();
		}
	}
}
void Tetrimino::deleteShadow(Tetrimino* current)
{
	for (int i = 0; i <4; i++)
	{
		if (blocks[i]->getY() < 1)
		{
			continue;
		}
		int j;
		for (j= 0; j < 4; j++)
		{
			if (blocks[i]->getX() == current->blocks[j]->getX() && blocks[i]->getY() == current->blocks[j]->getY())
			{
				break;
			}
		}
		if (j != 4)
		{
			continue;
		}
		gotoxy(2 * blocks[i]->getX() + 2, blocks[i]->getY() + 1);
		cout << "  ";
	}
}
void Tetrimino::rotate(string direction, Block* (*playField)[21])
{
	if (alphabet == 'O') //O미노는 회전 x
	{
		return;
	}
	Pos nextPos[4];
	for (int i = 0; i < 4; i++)
	{
		if (direction == "w")
		{
			if (alphabet == 'I') //I는 4x4 배열로 생각하여 회전
			{
				nextPos[i].x = 3 - blocks[i]->getY() + getFirstRow() + getFirstColumn();
				nextPos[i].y = blocks[i]->getX() + getFirstRow() - getFirstColumn();
			}
			else //다른 경우는 3x3 배열로 생각하여 회전
			{
				nextPos[i].x = 2 - blocks[i]->getY() + getFirstRow() + getFirstColumn();
				nextPos[i].y = blocks[i]->getX() + getFirstRow() - getFirstColumn();
			}
		}
		else if (direction == "x")
		{
			if (alphabet == 'I') //I는 4x4 배열로 생각하여 회전
			{
				nextPos[i].x = blocks[i]->getY() - getFirstRow() + getFirstColumn();
				nextPos[i].y = 3 - blocks[i]->getX() + getFirstRow() + getFirstColumn();
			}
			else //다른 경우는 3x3 배열로 생각하여 회전
			{
				nextPos[i].x = blocks[i]->getY() - getFirstRow() + getFirstColumn();
				nextPos[i].y = 2 - blocks[i]->getX() + getFirstRow() + getFirstColumn();
			}
		}
	}
	Pos wallkick = { 0,0 };
	for (int i = 1; i <= 5; i++)
	{
		wallkick = getWallkick(this, direction, i);
		for (int j = 0; j < 4; j++)
		{
			nextPos[j].x+= wallkick.x;
			nextPos[j].y -= wallkick.y;
		}
		if (isCrashed(nextPos, playField))
		{
			for (int j = 0; j < 4; j++)
			{
				nextPos[j].x -= wallkick.x;
				nextPos[j].y += wallkick.y;
			}
			continue;
		}
		for (int j = 0; j < 4; j++) //block 지우기
		{
			if (blocks[j]->getY() < 1)
			{
				continue;
			}
			gotoxy(2 * blocks[j]->getX() + 2, blocks[j]->getY() + 1);
			cout << "  ";
		}
		if (direction == "w")
		{
			rotationState++;
			if (rotationState == 4)
			{
				rotationState = 0;
			}
		}
		else if (direction == "x")
		{
			rotationState--;
			if (rotationState == -1)
			{
				rotationState = 3;
			}
		}
		for (int j = 0; j < 4; j++)
		{
			blocks[j]->setX(nextPos[j].x);
			blocks[j]->setY(nextPos[j].y);
		}
		return;
	}
}
int Tetrimino_I::getFirstRow()
{
	switch (rotationState)
	{
	case 0:
		return blocks[0]->getY() - 1;
	case 1:
	case 3:
		return findMin(blocks[0]->getY(),blocks[3]->getY());
	case 2:
		return blocks[0]->getY() - 2;
	}
	return 0;
}
int Tetrimino_I::getFirstColumn()
{
	switch (rotationState)
	{
	case 0:
	case 2:
		return findMin(blocks[0]->getX(),blocks[3]->getX());
	case 1:
		return blocks[0]->getX()-2;
	case 3:
		return blocks[0]->getX()-1;
	}
	return 0;
}
int Tetrimino_T::getFirstRow()
{
	switch (rotationState)
	{
	case 0:
	case 1:
	case 3:
		return findMin(blocks[0]->getY(),blocks[1]->getY(),blocks[3]->getY());
	case 2:
		return blocks[1]->getY() - 1;
	}
	return 0;
}
int Tetrimino_T::getFirstColumn()
{
	switch (rotationState)
	{
	case 0:
	case 2:
	case 3:
		return findMin(blocks[0]->getX(),blocks[1]->getX(),blocks[3]->getX());
	case 1:
		return blocks[1]->getX() - 1;
	}
	return 0;
}
int Tetrimino_L::getFirstRow()
{
	switch (rotationState)
	{
	case 0:
	case 1:
	case 3:
		return findMin(blocks[0]->getY(), blocks[1]->getY(), blocks[3]->getY());
	case 2:
		return blocks[1]->getY() - 1;
	}
	return 0;
}
int Tetrimino_L::getFirstColumn()
{
	switch (rotationState)
	{
	case 0:
	case 2:
	case 3:
		return findMin(blocks[0]->getX(), blocks[1]->getX(), blocks[3]->getX());
	case 1:
		return blocks[1]->getX() - 1;
	}
	return 0;
}
int Tetrimino_J::getFirstRow()
{
	switch (rotationState)
	{
	case 0:
	case 1:
	case 3:
		return findMin(blocks[0]->getY(), blocks[1]->getY(), blocks[3]->getY());
	case 2:
		return blocks[1]->getY() - 1;
	}
	return 0;
}
int Tetrimino_J::getFirstColumn()
{
	switch (rotationState)
	{
	case 0:
	case 2:
	case 3:
		return findMin(blocks[0]->getX(), blocks[1]->getX(), blocks[3]->getX());
	case 1:
		return blocks[1]->getX() - 1;
	}
	return 0;
}
int Tetrimino_S::getFirstRow()
{
	switch (rotationState)
	{
	case 0:
	case 1:
	case 3:
		return findMin(blocks[1]->getY(),blocks[2]->getY());
	case 2:
		return blocks[2]->getY() - 1;
	}
	return 0;
}
int Tetrimino_S::getFirstColumn()
{
	switch (rotationState)
	{
	case 0:
	case 2:
	case 3:
		return findMin(blocks[1]->getX(), blocks[2]->getX());
	case 1:
		return blocks[2]->getX() - 1;
	}
	return 0;
}
int Tetrimino_Z::getFirstRow()
{
	switch (rotationState)
	{
	case 0:
	case 1:
	case 3:
		return findMin(blocks[0]->getY(), blocks[3]->getY());
	case 2:
		return blocks[2]->getY() - 1;
	}
	return 0;
}
int Tetrimino_Z::getFirstColumn()
{
	switch (rotationState)
	{
	case 0:
	case 2:
	case 3:
		return findMin(blocks[0]->getX(), blocks[3]->getX());
	case 1:
		return blocks[2]->getX() - 1;
	}
	return 0;
}

int findMin(int a, int b)
{
	if (a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
}
int findMin(int a, int b, int c)
{
	int min = findMin(a, b);
	if (min < c)
	{
		return min;
	}
	else
	{
		return c;
	}
}

Pos getWallkick(Tetrimino* current, string direction, int test)
{
	char alphabet = current->getAlphabet();
	int rotationState = current->getRotationState();
	Pos cand1[4]{ {-2,0},{1,0},{-2,-1},{1,2} };
	Pos cand2[4] = { {-1,0},{-1,1},{0,-2},{-1,-2} };
	Pos cand3[4] = { {-1,0 }, { 2,0 }, { -1,2 }, { 2,-1 }};
	Pos change;
	if (test == 1)
	{
		change = { 0,0 };
		return change;
	}
	if (alphabet == 'I'&&direction=="w")
	{
		switch (rotationState)
		{
		case 0:
			return cand1[test - 2]; //test는 2,3,4,5 cand배열은 0,1,2,3
		case 1:
		{
			return cand3[test - 2];
		}
		case 2:
		{
			change.x = -cand1[test - 2].x;
			change.y = -cand1[test - 2].y;
			return change;
		}
		case 3:
		{
			change.x = -cand3[test - 2].x;
			change.y = -cand3[test - 2].y;
			return change;
		}
		}
	}
	else if (alphabet == 'I' && direction == "x")
	{
		switch (rotationState)
		{
		case 0:
			return cand3[test - 2];
		case 1:
		{
			change.x = -cand1[test - 2].x;
			change.y = -cand1[test - 2].y;
			return change;
		}
		case 2:
		{
			change.x = -cand3[test - 2].x;
			change.y = -cand3[test - 2].y;
			return change;
		}
		case 3:
		{
			return cand1[test - 2];
		}
		}
	}
	else if(alphabet!='I'&&direction=="w")
	{
		switch (rotationState)
		{
		case 0:
			return cand2[test - 2]; //test는 2,3,4,5 cand배열은 0,1,2,3
		case 1:
		{
			change.x = -cand2[test - 2].x;
			change.y = -cand2[test - 2].y;
			return change;
		}
		case 2:
		{
			change.x = -cand2[test - 2].x;
			change.y = cand2[test - 2].y;
			return change;
		}
		case 3:
		{
			change.x = cand2[test - 2].x;
			change.y = -cand2[test - 2].y;
			return change;
		}
		}
	}
	else if (alphabet != 'I' && direction == "x")
	{
		switch (rotationState)
		{
		case 0:
		{
			change.x = -cand2[test - 2].x;
			change.y = cand2[test - 2].y;
			return change;
		}
		case 1:
		{
			change.x = -cand2[test - 2].x;
			change.y = -cand2[test - 2].y;
			return change;
		}
		case 2:
		{
			return cand2[test - 2];
		}
		case 3:
		{
			change.x = cand2[test - 2].x;
			change.y = -cand2[test - 2].y;
			return change;
		}
		}
	}
}

bool Tetrimino::isGameOver()
{
	for (int i = 0; i < num; i++)
	{
		if (blocks[i]->getY() < 1)
		{
			return true;
		}
	}
	return false;
}

Tetrimino::~Tetrimino()
{
	for (int i = 0; i < num; i++)
	{
		delete blocks[i];
	}
}

bool isCrashed(Pos* nextPos, Block* (*playField)[21])
{
	int x, y;
	for (int i = 0; i < 4; i++)
	{
		x = nextPos[i].x;
		y = nextPos[i].y;
		if (x < 1 || x>10 || y > 20)
		{
			return true;
		}
		if (y < 1)
		{
			continue;
		}
		if (playField[x][y])
		{
			return true;
		}
	}
	return false;
}
void Tetrimino::cleanScreen(Pos* posArray,bool isNext) //움직이는 방향과 겹치지 않는 부분만 지움
{
	if (isNext)
	{
		for (int i = 0; i < 4; i++)
		{
			int j;
			for (j = 0; j < 4; j++)
			{
				if (blocks[i]->getX() == posArray[j].x && blocks[i]->getY() == posArray[j].y)
				{
					break;
				}
			}
			if (blocks[i]->getY() < 1 || j != 4)
			{
				continue;
			}
			gotoxy(2 * blocks[i]->getX() + 2, blocks[i]->getY() + 1);
			cout << "  ";
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			int j;
			for (j = 0; j < 4; j++)
			{
				if (posArray[i].x== blocks[j]->getX() && posArray[i].y==blocks[j]->getY())
				{
					break;
				}
			}
			if (posArray[i].y<1||j != 4)
			{
				continue;
			}
			gotoxy(2 * posArray[i].x+ 2,posArray[i].y + 1);
			cout << "  ";
		}
	}
}

int Tetrimino::detectTspin(Block* (*playField)[21])
{
	if (alphabet != 'T')
	{
		return 0;
	}
	Pos posArray[4];
	posArray[0].x = blocks[2]->getX() - 1;
	posArray[0].y = blocks[2]->getY() - 1;
	posArray[1].x = blocks[2]->getX() - 1;
	posArray[1].y = blocks[2]->getY() + 1;
	posArray[2].x = blocks[2]->getX() + 1;
	posArray[2].y = blocks[2]->getY() - 1;
	posArray[3].x = blocks[2]->getX() + 1;
	posArray[3].y = blocks[2]->getY() + 1;
	int filled = 0;
	for (int i = 0; i < 4; i++)
	{
		if (playField[posArray[i].x][posArray[i].y]) //블록 차있는 경우
		{
			filled++;
		}
		else if (posArray[i].x == 0 || posArray[i].x == 11 || posArray[i].y == 21) //벽인 경우
		{
			filled++;
		}
	}
	if (filled<3 )
	{
		return 0; //0: tspin 아닐 때
	}
	switch (rotationState)
	{
	case 0:
	{
		if (blocks[2]->getY() == 20 || playField[blocks[2]->getX()][blocks[2]->getY() + 1])
		{
			return 1; //1: mini
		}
		else
		{
			return 2;
		}
	}
	case 1:
	{
		if (blocks[2]->getX() == 1 || playField[blocks[2]->getX() - 1][blocks[2]->getY()])
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	case 2:
	{
		if (playField[blocks[2]->getX()][blocks[2]->getY() - 1])
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	case 3:
	{
		if (blocks[2]->getX() == 20 || playField[blocks[2]->getX() + 1][blocks[2]->getY()])
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	}
}