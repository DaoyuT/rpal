#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

const vector<char> operator_symbol ({
							'+', '-', '*', '<', '>', '&', '.',
							'@', '/', ':', '=', '~', '|', '$',
							'!', '#', '%', '^', '_', '[', ']',
							'{', '}', '"', '`', '?'
							});
const vector<char> punctions ({'(', ')', ';', ','});
const vector<char> spaces ({' ', '\t', '\n'});
const vector<char> escapes ({'t', 'n', '\\', '\''});
const vector<string> TYPE_NAMES ({"NULL", "IDENTIFIER", "INTEGER", "OPERATOR", "STRING", "PUNCTION"});
	

class Token {
public:
	const static int IDENTIFIER = 1;
	const static int INTEGER = 2;
	const static int OPERATOR = 3;
	const static int STRING = 4;
	const static int PUNCTION = 5;
	int startLine;
	int startCol;
	int type;
	string content;
	Token(int sL, int sC, int type, string content);
	~Token();
	void print();
};

class Lexicon {
public:
	string fileName;
    ifstream file;
    int lineNum ;
    int colNum;
    char nextChar;

	Lexicon(const char* fileName);
	~Lexicon();
	void print();
	void reopen();
	bool belongsTo(char ch, vector<char> category);
	bool hasNext();
	Token nextToken();
	Token startId(int sL, int sC, string content);
	Token startInt(int sL, int sC, string content);
	Token startString(int sL, int sC, string content);
	Token backSlash(int sL, int sC, string content);
	Token startOperator(int sL, int sC, string content);
	void errorPrint(string msg,int sL, int sC);
};

