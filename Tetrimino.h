#pragma once
#include "Block.h"

struct Pos
{
	int x;
	int y;
};

class Tetrimino
{
protected:
	Block* blocks[4];
	char alphabet;
	int num;
	int rotationState; 

public:
	Tetrimino();
	int getNum() { return num; };
	char getAlphabet() { return alphabet; };
	int getRotationState() { return rotationState; };
	void display() const;
	void displayNext() const;
	void initField(Block*(*playField)[21]);
	bool move(string direction, Block* (*playField)[21]);
	void goBottom(Block* (*playField)[21],bool shadow);
	void rotate(string direction, Block* (*playField)[21]);
	void cleanScreen(Pos* posArray, bool isNext);
	void fillField(Block* (*playField)[21]);
	void deleteBlock(int y);
	void makeShadow(Tetrimino* current, Block* (*playField)[21]);
	void  deleteShadow(Tetrimino* current);
	virtual int getFirstRow() =0;
	virtual int getFirstColumn()=0;
	int detectTspin(Block* (*playField)[21]);
	bool isGameOver();
	~Tetrimino();
};

class Tetrimino_I : public Tetrimino
{
public:
	Tetrimino_I();
	virtual int getFirstRow();
	virtual int getFirstColumn();
};
class Tetrimino_O : public Tetrimino
{
public:
	Tetrimino_O();
	virtual int getFirstRow() {return 0;};
	virtual int getFirstColumn() { return 0; };
};
class Tetrimino_T : public Tetrimino
{
public:
	Tetrimino_T();
	virtual int getFirstRow();
	virtual int getFirstColumn();
};
class Tetrimino_L : public Tetrimino
{
public:
	Tetrimino_L();
	virtual int getFirstRow();
	virtual int getFirstColumn();
};
class Tetrimino_J : public Tetrimino
{
public:
	Tetrimino_J();
	virtual int getFirstRow();
	virtual int getFirstColumn();
};
class Tetrimino_S : public Tetrimino
{
public:
	Tetrimino_S();
	virtual int getFirstRow();
	virtual int getFirstColumn();
};
class Tetrimino_Z : public Tetrimino
{
public:
	Tetrimino_Z();
	virtual int getFirstRow();
	virtual int getFirstColumn();
};
