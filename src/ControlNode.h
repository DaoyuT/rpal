#include "header.h"

using namespace std;

class ControlNode {
public:
	static const int NT = -1;
	static const int IDENTIFIER = 0;
	static const int STRING = 1;
	static const int BOOL = 2;
	static const int NIL = 3;
	static const int DUMMY = 4;
	static const int INTEGER = 5;
	static const int LAMBDA = 6;
	static const int TAU = 7;
	static const int BETA = 8;
	static const int ENV = 9;
	static const vector<string> typeNames;

	int type;
	string contentStr;
	int contentInt;

	ControlNode(int _type, string _contentStr, int _contentInt);
	~ControlNode();
	string toString();
};