#ifndef POINT_H
#define POINT_H
#include <string>

enum Color {
	RED,
	GREEN,
	YELLOW,
	BLUE,
	MAGENTA,
	CYAN,
	WHITE,
};

using namespace std;

class Block {
	private:
		int x;
		int y;
		string msg;
		Color color;

	public :
		Block();
		Block(int, int, Color);
		Block(int, int, Color, string);
		int getX() const {return x/2-1;}
		int getY() const {return y-1;}
		string getMsg() const {return msg;}
		Color getColor() const {return color;}
		void setX(int x_) {x = 2*x_+2;}
		void setY(int y_) {y = y_+1;}
		void setMsg(string msg_) {msg = msg_;}
		void setColor(Color color_) {color = color_;}
		Block operator+(const Block &pt);
		void display();
		~Block() {};
};

#endif
