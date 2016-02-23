#include "rpal-parser.h"

using namespace std;

Lexicon :: Lexicon(const char* fileName): fileName(fileName), lineNum(1), colNum(0) {
	file.open(fileName);
	nextChar = '\0';
}

Lexicon :: ~Lexicon () {
	file.close();
}

void Lexicon :: print() {
	char c;
	if(file.is_open()) {
		while(file.get(c)) {
			cout << c;
		}
	}
}

void Lexicon :: reopen() {
	file.close();
	file.open(fileName);
}

bool Lexicon :: belongsTo(char ch, vector<char> category) {
	for(char item : category) {
		if(item == ch){
			return true;
		}
	}
	return false;
}

Token Lexicon :: nextToken() {
	if(nextChar == '\n'){
		colNum = 0;
		lineNum++;
	}
	file.get(nextChar);
	colNum++;
	if(belongsTo(nextChar, spaces)) {
		return nextToken();
	} else if(belongsTo(nextChar, punctions)) {
		return Token(lineNum, colNum, Token::PUNCTION, string(1, nextChar));
	} else if(isalpha(nextChar)) {
		return startId(lineNum, colNum, string(1, nextChar));
	} else if(isdigit(nextChar)) {
		return startInt(lineNum, colNum, string(1, nextChar));
	} else if(nextChar == '\'') {
		return startString(lineNum, colNum, string(1, nextChar));
	} else if(nextChar == '/') {
		char nextNextChar = file.peek();
		if(nextNextChar == '/') {
			// eat up this line
			do {
				file.get(nextChar);
				colNum++;
			} while(nextChar != '\n');
			return nextToken();
		} else {
			return startOperator(lineNum, colNum, "/");
		}
	} else if(belongsTo(nextChar, operator_symbol)) {
		return startOperator(lineNum, colNum, string(1, nextChar));
	} else {
		errorPrint("Unknown character " + string(1, nextChar), lineNum, colNum);
		exit(0);
	}
}

Token Lexicon :: startId(int sL, int sC, string content) {
	char ch = file.peek();
	if(isalpha(ch) || isdigit(ch) || ch == '_'){
		file.get(nextChar);
		colNum++;
		return startId(sL, sC, content + ch);
	} else {
		return Token(sL, sC, Token::IDENTIFIER, content);
	}
}

Token Lexicon :: startInt(int sL, int sC, string content) {
	char ch = file.peek();
	if(isdigit(ch)){
		file.get(nextChar);
		colNum++;
		return startInt(sL, sC, content + ch);
	} else {
		return Token(sL, sC, Token::INTEGER, content);
	}
}

Token Lexicon :: startOperator(int sL, int sC, string content) {
	char ch = file.peek();
	if(belongsTo(ch, operator_symbol)){
		file.get(nextChar);
		colNum++;
		return startOperator(sL, sC, content + ch);
	} else {
		return Token(sL, sC, Token::OPERATOR, content);
	}
}

Token Lexicon :: startString(int sL, int sC, string content) {
	char ch = file.peek();
	if(isalpha(ch) || isdigit(ch) || belongsTo(ch, punctions) || belongsTo(ch, operator_symbol) || ch == ' '){
		file.get(nextChar);
		colNum++;
		return startString(sL, sC, content + ch);
	} else if(ch == '\\'){
		file.get(nextChar);
		colNum++;
		return backSlash(sL, sC, content + ch);
	} else if(ch == '\'') {
		file.get(nextChar);
		colNum++;
		return Token(sL, sC, Token::STRING, content + ch);
	} else if(ch == '\n') {
		errorPrint("Missing terminating \' character.", sL, sC);
		exit(0);
	} else {
		errorPrint("Illegal char in string.", lineNum, colNum + 1);
		exit(0);
	}
}

Token Lexicon :: backSlash(int sL, int sC, string content) {
	char ch = file.peek();
	if(belongsTo(ch, escapes)) {
		file.get(nextChar);
		colNum++;
		return startString(sL, sC, content + ch);
	} else {
		errorPrint("Unknown escape sequence.", lineNum, colNum + 1);
		exit(0);
	}
}

void Lexicon :: errorPrint(string msg,int sL, int sC) {
	cout << "Error: " << msg << " Line " << sL << ", col " << sC << endl;
	ifstream fileCopy (fileName);
	if(fileCopy.is_open()) {
		string line;
		for(int i = 0; i < sL; i++){
			getline(fileCopy, line);
		}
		cout << "├─ " << line << endl;
		cout << "└─ " << string(sC - 1, ' ') << '^' << endl; 
	}
}

bool Lexicon :: hasNext() {
	while(belongsTo(file.peek(), spaces)){
		// eat up spaces
		file.get(nextChar);
		colNum++;
	}
	return file.peek() >= 0;
}

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

int main(int argc, char *argv[]) {
	if(argc != 2) {
		cout << "got " << argc - 1 << " args, need 1 args!" << endl;
		return 1;
	}
	Lexicon lex (argv[1]);
	cout << "-----------------------------------" << endl;
	lex.print();
	lex.reopen();
	cout << "-----------------------------------" << endl;
	while(lex.hasNext()) {
		Token token = lex.nextToken();
		token.print();
	}
}