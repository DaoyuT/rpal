#include "header.h"

using namespace std;

class Token {
public:
	static const int IDENTIFIER = 0;
	static const int STRING = 1;
	static const int BOOL = 2;
	static const int NIL = 3;
	static const int DUMMY = 4;
	static const int INTEGER = 5;
	static const int OPERATOR = 6;
	static const int PUNCTION = 7;
	static const int KEYWORD = 8;
	static const int ENDOFFILE = 9;
    static const vector<string> TYPE_NAMES;

	int startLine;
	int startCol;
	int type;
	string content;
	Token();
	Token(int sL, int sC, int type, string content);
	~Token();
	void print();
	string toString();
	bool equals(int type, string content);
};