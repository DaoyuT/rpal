#include "header.h"

using namespace std;

class ASTNode {
public:
	ASTNode* left = NULL;
	ASTNode* right = NULL;

	virtual string toString() = 0;
};

class ASTNode_NT: public ASTNode {
public:
	string name;

	ASTNode_NT(string name);
	~ASTNode_NT();
	string toString();
};

class ASTNode_ID: public ASTNode {
public:
	static const int IDENTIFIER = 0;
	static const int STRING = 1;
	static const int BOOL = 2;
	static const int NIL = 3;
	static const int DUMMY = 4;
	static const vector<string> names;

	int type;
	string content;

	ASTNode_ID(int type, string content);
	~ASTNode_ID();
	string toString();
};

class ASTNode_INT: public ASTNode {
public:
	const string name = "INT";
	int content;

	ASTNode_INT(int content);
	~ASTNode_INT();
	string toString();
};