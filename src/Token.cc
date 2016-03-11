#include "Token.h"

// --------------------------------------------Token--------------------------------------------

const vector<string> Token :: TYPE_NAMES ({"IDENTIFIER", "STRING", "BOOL", "NIL", "DUMMY", "INTEGER", "OPERATOR", "PUNCTION", "KEYWORD", "EOF"});

Token :: Token() {}

Token :: Token(int sL, int sC, int type, string content) {
	startLine = sL;
	startCol = sC;
	this->type = type;
	this->content = content;
}

Token :: ~Token() {}

void Token :: print() {
	cout << "LineNum: " << setw(4) << startLine << " ColNum: " << setw(4) << startCol << " Type:" << setw(11) << TYPE_NAMES[type] << " Token: " << content << endl;
}

string Token :: toString() {
	return "<" + TYPE_NAMES[type] + ":" + content + ">";
}


bool Token :: equals(int type, string content) {
	return type == this->type && content == this->content;
}
