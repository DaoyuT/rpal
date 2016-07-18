#include "header.h"
#include "ControlNode.h"

using namespace std;

class StackNode {
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
	static const int ETA = 8;
	static const int ENV = 9;
	static const int PRINT = 10;
	static const int ISINTEGER = 11;
	static const int ISTRUTHVALUE = 12;
	static const int ISSTRING = 13;
	static const int ISTUPLE = 14;
	static const int ISDUMMY = 15;
	static const int ORDER = 16;
	static const int STEM = 17;
	static const int STERN = 18;
	static const int CONC = 19;
	static const vector<string> typeNames;

	int type;
	string valueStr;
	int valueInt;
	int environmentNumber = -1;
	StackNode* nextElement = NULL;
	StackNode* tauNext = NULL;


	StackNode(ControlNode* cs);
	StackNode(ControlNode* cs, int _environmentNumber);
	StackNode(int _type, string _valueStr, int _valueInt);
	StackNode(int _type, string _valueStr, int _valueInt, int _environmentNumber);
	StackNode(int _type, StackNode* sn);
	~StackNode();
	string toString();
};