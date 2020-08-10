#pragma once

class GameManager
{
private:
	float speed;
	int key;
	int tetNum;
	int pts;
	bool roundFinish;
	bool shadowOn;
	int minoPack[7];
	int nextNum;
	int combo;
	bool backToBack;
	Tetrimino* current;
	Tetrimino* shadow;
	Tetrimino* tetList[500];
	Block* playField[11][21];
	Block* prevPlayField[11][21];
public:
	GameManager();
	void playGame();
	void makeFrame();
	void updateField();
	void printField() const;
	void makeTetrimino();
	void deleteLine(bool ifRotate);
	bool isGameFinish();
	int getPts(){ return pts; };
	void deleteAll();
	void rearrange(int y);
};