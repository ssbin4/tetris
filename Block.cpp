#include <iostream>
#include "Block.h"

using namespace std;

#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define cRED "\x1b[31m"
#define cGREEN "\x1b[32m"
#define cYELLOW "\x1b[33m"
#define cBLUE "\x1b[34m"
#define cMAGENTA "\x1b[35m"
#define cCYAN "\x1b[36m"
#define cWHITE "\x1b[37m"
#define cRESET "\x1b[0m"

Block::Block()
{
	x = 0;
	y = 0;
}
Block::Block(int x_, int y_, Color color_)
{
	x = x_;
	y = y_;
	color = color_;
	msg ="■";
}
Block::Block(int x_, int y_, Color color_, string msg_)
{
	x = x_;
	y = y_;
	color = color_;
	msg = msg_;
}


Block Block::operator+(const Block &pt)
{
	return Block(x + pt.x, y + pt.y, color, msg);
}	
void Block::display()
{
	gotoxy(this->x, this->y);
	if (color == RED) {cout<< cRED << msg << cRESET;}
	else if (color == GREEN) {cout<< cGREEN << msg << cRESET;}
	else if (color == YELLOW) {cout<< cYELLOW << msg << cRESET;}
	else if (color == BLUE) {cout<< cBLUE << msg << cRESET;}
	else if (color == MAGENTA) {cout<< cMAGENTA << msg << cRESET;}
	else if (color == CYAN) {cout<< cCYAN << msg << cRESET;}
	else {cout<< cWHITE << msg << cRESET;}
}
