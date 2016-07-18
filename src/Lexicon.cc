#include "Lexicon.h"

// --------------------------------------------Lexicon--------------------------------------------

const vector<char> Lexicon :: OPERATOR_SYMBOL ({
							'+', '-', '*', '<', '>', '&', '.',
							'@', '/', ':', '=', '~', '|', '$',
							'!', '#', '%', '^', '_', '[', ']',
							'{', '}', '"', '`', '?'
						});
const vector<char> Lexicon :: PUNCTIONS ({'(', ')', ';', ','});
const vector<char> Lexicon :: SPACES ({' ', '\t', '\n'});
const vector<char> Lexicon :: ESCAPES ({'t', 'n', '\\', '\''});
const vector<string> Lexicon :: KEYWORDS ({"let", "in", "fn", "where", "aug", "or", "not", "gr", "ge", "ls", "le","eq", "ne", "within", "and", "rec"});
const vector<string> Lexicon :: BOOLS ({"true", "false"});
const vector<string> Lexicon :: NILS ({"nil"});
const vector<string> Lexicon :: DUMMYS ({"dummy"});

Lexicon :: Lexicon(const char* fileName): fileName(fileName), lineNum(1), colNum(0) {
	file.open(fileName);
	nextChar = '\0';
	nextToken = getNextToken();
}

Lexicon :: ~Lexicon () {
	file.close();
	destroyAST(root);
	root = NULL;
}

void Lexicon :: destroyAST(ASTNode* node) {
	if (node == NULL) {
		return;
	}
	destroyAST(node->left);
	destroyAST(node->right);
	delete node;
}

void Lexicon :: print() {
	char c;
	if (file.is_open()) {
		while (file.get(c)) {
			cout << c;
		}
	}
}

void Lexicon :: reopen() {
	lineNum = 1;
	colNum = 0;
	file.close();
	file.open(fileName);
}

bool Lexicon :: belongsTo(char ch, const vector<char> category) {
	for (char item : category) {
		if (item == ch){
			return true;
		}
	}
	return false;
}

bool Lexicon :: belongsTo(string ch, const vector<string> category) {
	for (string item : category) {
		if (item == ch){
			return true;
		}
	}
	return false;
}

bool Lexicon :: hasNext() {
	while (belongsTo(file.peek(), SPACES)){
		colNum++;
		if (file.peek() == '\n') {
			lineNum++;
			colNum = 0;
		}
		// eat up spaces
		file.get(nextChar);
	}
	if (file.peek() >= 0) {
		return true;
	} else {
		if (reachedEOF) {
			return false;
		} else {
			reachedEOF = true;
			return true;
		}
	}
}

Token Lexicon :: getNextToken() {
/*	if (nextChar == '\n'){
		colNum = 0;
		lineNum++;
	}*/
	if (reachedEOF) {
		return Token(lineNum, colNum, Token::ENDOFFILE, "^EOF");
	}
	file.get(nextChar);
	colNum++;
	if (belongsTo(nextChar, SPACES)) {
		return getNextToken();
	} else if (belongsTo(nextChar, PUNCTIONS)) {
		return Token(lineNum, colNum, Token::PUNCTION, string(1, nextChar));
	} else if (isalpha(nextChar)) {
		return startId(lineNum, colNum, string(1, nextChar));
	} else if (isdigit(nextChar)) {
		return startInt(lineNum, colNum, string(1, nextChar));
	} else if (nextChar == '\'') {
		return startString(lineNum, colNum, string(1, nextChar));
	} else if (nextChar == '/') {
		char nextNextChar = file.peek();
		if (nextNextChar == '/') {
			// eat up this line
			char peek = '\0';
			do {
				peek = file.peek();
				file.get(nextChar);
				colNum++;
			} while (nextChar != '\n' && peek != -1);
			if (peek == -1) {
				reachedEOF = true;
				return Token(lineNum, colNum, Token::ENDOFFILE, "^EOF");
			} else {
				lineNum++;
				colNum = 0;
				hasNext();
				return getNextToken();
			}
			
		} else {
			return startOperator(lineNum, colNum, "/");
		}
	} else if (belongsTo(nextChar, OPERATOR_SYMBOL)) {
		return startOperator(lineNum, colNum, string(1, nextChar));
	} else {
		errorPrint("Unknown character " + string(1, nextChar), lineNum, colNum);
		exit(0);
	}
}

Token Lexicon :: startId(int sL, int sC, string content) {
	char ch = file.peek();
	if (isalpha(ch) || isdigit(ch) || ch == '_'){
		file.get(nextChar);
		colNum++;
		return startId(sL, sC, content + ch);
	} else {
		if (belongsTo(content, KEYWORDS)) {
			return Token(sL, sC, Token::KEYWORD, content);
		} else if (belongsTo(content, BOOLS)) {
			return Token(sL, sC, Token::BOOL, content);
		} else if (belongsTo(content, NILS)) {
			return Token(sL, sC, Token::NIL, content);
		} else if (belongsTo(content, DUMMYS)){
			return Token(sL, sC, Token::DUMMY, content);
		} else {
			return Token(sL, sC, Token::IDENTIFIER, content);
		}
	}
}

Token Lexicon :: startInt(int sL, int sC, string content) {
	char ch = file.peek();
	if (isdigit(ch)){
		file.get(nextChar);
		colNum++;
		return startInt(sL, sC, content + ch);
	} else {
		return Token(sL, sC, Token::INTEGER, content);
	}
}

Token Lexicon :: startOperator(int sL, int sC, string content) {
	char ch = file.peek();
	if (belongsTo(ch, OPERATOR_SYMBOL)){
		file.get(nextChar);
		colNum++;
		return startOperator(sL, sC, content + ch);
	} else {
		return Token(sL, sC, Token::OPERATOR, content);
	}
}

Token Lexicon :: startString(int sL, int sC, string content) {
	char ch = file.peek();
	if (isalpha(ch) || isdigit(ch) || belongsTo(ch, PUNCTIONS) || belongsTo(ch, OPERATOR_SYMBOL) || ch == ' '){
		file.get(nextChar);
		colNum++;
		return startString(sL, sC, content + ch);
	} else if (ch == '\\'){
		file.get(nextChar);
		colNum++;
		return backSlash(sL, sC, content + ch);
	} else if (ch == '\'') {
		file.get(nextChar);
		colNum++;
		return Token(sL, sC, Token::STRING, content + ch);
	} else if (ch == '\n') {
		errorPrint("Missing terminating \' character.", sL, sC);
		exit(0);
	} else {
		errorPrint("Illegal char in string.", lineNum, colNum + 1);
		exit(0);
	}
}

Token Lexicon :: backSlash(int sL, int sC, string content) {
	char ch = file.peek();
	if (belongsTo(ch, ESCAPES)) {
		file.get(nextChar);
		colNum++;
		return startString(sL, sC, content + ch);
	} else {
		errorPrint("Unknown escape sequence.", lineNum, colNum + 1);
		exit(0);
	}
}

void Lexicon :: errorPrint(string msg,int sL, int sC) {
	cout << "Error: " << msg << ": Line " << sL << ", col " << sC << endl;
	ifstream fileCopy (fileName);
	if (fileCopy.is_open()) {
		string line;
		for (int i = 0; i < sL; i++){
			getline(fileCopy, line);
		}
		cout << "├─ " << line << endl;
		cout << "└─ " << string(sC - 1, ' ') << '^' << endl; 
	}
	printErrorTree();
}

void Lexicon :: buildTree(string name, int numOfCh) {
	if (stackOfNodes.size() < numOfCh) {
		cout << "Error: Trying to get " << numOfCh << " nodes in stack size of " << stackOfNodes.size() << "." << endl;
		exit(0);
	}
	ASTNode* p = stackOfNodes.top();
	stackOfNodes.pop();
	while (numOfCh > 1) {
		ASTNode* elderBro = stackOfNodes.top();
		stackOfNodes.pop();
		elderBro->right = p;
		//p->parent = elderBro;
		p = elderBro;
		numOfCh--;
	}
	ASTNode_NT* newNT = new ASTNode_NT(name);
	newNT->left = p;
	//p->parent = newNT;
	stackOfNodes.push(newNT);
}

void Lexicon :: printTree() {
	printTreeHelper(root, 0);
}

void Lexicon :: printErrorTree() {
	stack<ASTNode*> copy = stackOfNodes;
	cout << "Printing Error Tree..." << endl;
	while (copy.size()) {
		cout << "-------------------------" << endl;
		ASTNode* p = copy.top();
		copy.pop();
		printTreeHelper(p, 0);
	}
	cout << "-------------------------" << endl;
}

void Lexicon :: printTreeHelper(ASTNode* node, int level) {
	if (node == NULL) {
		return;
	}
	cout << string(level, '.') << node->toString() + " " << endl;
	STTreeTypes.insert(node->getType());
	if (node->getType() != 0 && node->getType() != 1) {
		STTreeContents.insert(node->getContent());
	}
	printTreeHelper(node->left, level + 1);
	printTreeHelper(node->right, level);
}

// --------------------------------------------Standardize--------------------------------------------

void Lexicon :: standardizeTree() {
	standardizeHelper(root);
}

void Lexicon :: checkNode(ASTNode* node, string name) {
	if (node->toString() != name) {
		cout << "Wrong node while standardizing: \"" << name << "\" expected, but \"" << node->toString() << "\" found."<< endl;
		exit(0);
	}
}

void Lexicon :: standardizeHelper(ASTNode* node) {
	if (node == NULL) {
		return;
	}
	standardizeHelper(node->left);
	standardizeHelper(node->right);
	if (node->toString() == "let") {
		standardizeLet(node);
	} else if (node->toString() == "where") {
		standardizeWhere(node);
	} else if (node->toString() == "function_form") {
		node->setContent("=");
		standardizeMultiParams(node);
	} else if (node->toString() == "lambda") {
		standardizeMultiParams(node);
	} else if (node->toString() == "within") {
		standardizeWithin(node);
	} else if (node->toString() == "@") {
		standardizeAt(node);
	} else if (node->toString() == "and") {
		standardizeAnd(node);
	} else if (node->toString() == "rec") {
		standardizeRec(node);
	} 
}

void Lexicon :: standardizeLet(ASTNode* node) {
	node->setContent("gamma");
	ASTNode* equalNode = node->left;
	ASTNode* X = equalNode->left;
	ASTNode* P = equalNode->right;
	ASTNode* E = X->right;
	checkNode(equalNode, "=");
	ASTNode* newLambda = new ASTNode_NT("lambda");
	node->left = newLambda;
	newLambda->left = X;
	newLambda->right = E;
	X->right = P;
	delete equalNode;
}

void Lexicon :: standardizeWhere(ASTNode* node) {
	node->setContent("gamma");
	ASTNode* P = node->left;
	ASTNode* equalNode = P->right;
	ASTNode* X = equalNode->left;
	ASTNode* E = X->right; 
	checkNode(equalNode, "=");
	ASTNode* newLambda = new ASTNode_NT("lambda");
	node->left = newLambda;
	newLambda->left = X;
	newLambda->right = E;
	X->right = P;
	P->right = NULL;
	delete equalNode;
}

void Lexicon :: standardizeMultiParams(ASTNode* node) {
	ASTNode* pre = node->left;		// function name 		first parameter
	ASTNode* curr = pre->right;		// first parameter 		second parameter or body
	while (curr->right != NULL) {
		ASTNode* newLambda = new ASTNode_NT("lambda");
		pre->right = newLambda;
		newLambda->left = curr;
		pre = curr;
		curr = curr->right;
	}
}

void Lexicon :: standardizeWithin(ASTNode* node) {
	node->setContent("=");
	ASTNode* equalNode1 = node->left;
	ASTNode* equalNode2 = equalNode1->right;
	ASTNode* X1 = equalNode1->left;
	ASTNode* E1 = X1->right;
	ASTNode* X2 = equalNode2->left;
	ASTNode* E2 = X2->right;
	checkNode(equalNode1, "=");
	checkNode(equalNode2, "=");
	ASTNode* newLambda = new ASTNode_NT("lambda");
	ASTNode* newGamma = new ASTNode_NT("gamma");
	node->left = X2;
	X2->right = newGamma;
	newGamma->left = newLambda;
	newLambda->left = X1;
	newLambda->right = E1;
	X1->right = E2;
	delete equalNode1;
	delete equalNode2;
}

void Lexicon :: standardizeAt(ASTNode* node) {
	node->setContent("gamma");
	ASTNode* E1 = node->left;
	ASTNode* N = E1->right;
	ASTNode* E2 = N->right;
	ASTNode* newGamma = new ASTNode_NT("gamma");
	node->left = newGamma;
	newGamma->left = N;
	newGamma->right = E2;
	N->right = E1;
	E1->right = NULL;
}

void Lexicon :: standardizeAnd(ASTNode* node) {
	node->setContent("=");
	ASTNode* equalNode = node->left;
	checkNode(equalNode, "=");
	ASTNode* X = equalNode->left;
	ASTNode* E = X->right;
	ASTNode* nextEqualNode = equalNode->right;
	ASTNode* newComma = new ASTNode_NT(",");
	ASTNode* newTau = new ASTNode_NT("tau");
	node->left = newComma;
	newComma->right = newTau;
	newComma->left = X;
	newTau->left = E;
	while (nextEqualNode != NULL) {
		delete equalNode;
		checkNode(nextEqualNode, "=");
		ASTNode* nextX = nextEqualNode->left;
		ASTNode* nextE = nextX->right;
		X->right = nextX;
		E->right = nextE;
		X = nextX;
		E = nextE;
		X->right = NULL;
		E->right = NULL;
		equalNode = nextEqualNode;
		nextEqualNode = nextEqualNode->right;
	}
}

void Lexicon :: standardizeRec(ASTNode* node) {
	node->setContent("=");
	ASTNode* equalNode = node->left;
	checkNode(equalNode, "=");
	ASTNode* X = equalNode->left;
	ASTNode* E = X->right;
	ASTNode* newGamma = new ASTNode_NT("gamma");
	ASTNode* newLambda = new ASTNode_NT("lambda");
	ASTNode* YStar = new ASTNode_NT("YStar");
	ASTNode* copyOfX = new ASTNode_ID(X->getType(), X->getContent());
	node->left = X;
	X->right = newGamma;
	newGamma->left = YStar;
	YStar->right = newLambda;
	newLambda->left = copyOfX;
	copyOfX->right = E;
	delete equalNode;
}

// --------------------------------------------Procedures--------------------------------------------

void Lexicon :: setDebugMode(int i) {
	debugMode = i;
}

void Lexicon :: read(int type, string content, string calledBy) {
	if (type <= Token::INTEGER) {
		if (nextToken.type != type) {
			cout << "Error: In " << calledBy << "(): Expected token type <" << Token::TYPE_NAMES[type] << ">, but " << nextToken.toString() << " found." << endl;
			errorPrint("Unexpected token", nextToken.startLine, nextToken.startCol);
			exit(0); 
		}
		ASTNode* node;
		if (nextToken.type == Token::INTEGER) {
			node = new ASTNode_INT(stoi(nextToken.content));
		} else {
			node = new ASTNode_ID(nextToken.type, nextToken.content);
		}
		stackOfNodes.push(node);
		if (debugMode) {
			cout << "Terminal pushed into stack:" << endl;
			nextToken.print();
		}
	} else {
		if (nextToken.type != type || nextToken.content.compare(content) != 0) {
			cout << "Error: In " << calledBy << "(): Expected token <" << Token::TYPE_NAMES[type] << ":" << content << ">, but " << nextToken.toString() << " found." << endl;
			errorPrint("Unexpected token", nextToken.startLine, nextToken.startCol);
			exit(0); 
		}
		if (debugMode) {
			cout << "Non-terminal read:" << endl;
			nextToken.print();
		}
	}
	if (hasNext()) {
		nextToken = getNextToken();
	}
	// EOF????
}

void Lexicon :: startParse() {
	procE();
	if (stackOfNodes.size() != 1) {
		cout << "Error: Failed to get the only root." << endl;
		exit(0);
	}
	root = stackOfNodes.top();
	stackOfNodes.pop();
}

// multi children lambda
void Lexicon :: procE() {
	string procName = "E";
	if (nextToken.equals(Token::KEYWORD, "let")) {
		read(Token::KEYWORD, "let", procName);
		procD();
		read(Token::KEYWORD, "in", procName);
		procE();
		buildTree("let", 2);
	} else if (nextToken.equals(Token::KEYWORD,"fn")) {
		read(Token::KEYWORD, "fn", procName);
		int n = 1;
		do {
			procVb();
			n++;
		} while (nextToken.type == Token::IDENTIFIER || nextToken.equals(Token::PUNCTION, "("));
		read(Token::OPERATOR, ".", procName);
		procE();
		buildTree("lambda", n);
	} else {
		procEw();
	}
}

void Lexicon :: procEw() {
	string procName = "Ew";
	procT();
	if (nextToken.equals(Token::KEYWORD, "where")) {
		read(Token::KEYWORD, "where", procName);
		procDr();
		buildTree("where", 2);
	}
}

// multi children tau
void Lexicon :: procT() {
	string procName = "T";
	procTa();
	int n = 1;

	while (nextToken.equals(Token::PUNCTION, ",")) {
		read(Token::PUNCTION, ",", procName);
		procTa();
		n++;
	}
	if (n > 1) {
		buildTree("tau", n);
	}
}

// left associative aug
void Lexicon :: procTa() {
	string procName = "Ta";
	procTc();
	while (nextToken.equals(Token::KEYWORD, "aug")) {
		read(Token::KEYWORD, "aug", procName);
		procTc();
		buildTree("aug", 2);
	}
}

void Lexicon :: procTc() {
	string procName = "Tc";
	procB();
	if (nextToken.equals(Token::OPERATOR, "->")) {
		read(Token::OPERATOR, "->", procName);
		procTc();
		read(Token::OPERATOR, "|", procName);
		procTc();
		buildTree("->", 3);
	}
}

// left associative or
void Lexicon :: procB() {
	string procName = "B";
	procBt();
	while (nextToken.equals(Token::KEYWORD, "or")) {
		read(Token::KEYWORD, "or", procName);
		procBt();
		buildTree("or", 2);
	}
}

// left associative &
void Lexicon :: procBt() {
	string procName = "Bt";
	procBs();
	while (nextToken.equals(Token::OPERATOR, "&")) {
		read(Token::OPERATOR, "&", procName);
		procBs();
		buildTree("&", 2);
	}
}

// single child not
void Lexicon :: procBs() {
	string procName = "Bs";
	if (nextToken.equals(Token::KEYWORD, "not")) {
		read(Token::KEYWORD, "not", procName);
		procBp();
		buildTree("not", 1);
	} else {
		procBp();
	}
}

// long and boring
void Lexicon :: procBp() {
	string procName = "Bp";
	procA();
	if (nextToken.equals(Token::KEYWORD, "gr")) {
		// gr
		read(Token::KEYWORD, "gr", procName);
		procA();
		buildTree("gr", 2);
	} else if(nextToken.equals(Token::OPERATOR, ">")) {
		// gr
		read(Token::OPERATOR, ">", procName);
		procA();
		buildTree("gr", 2);
	} else if (nextToken.equals(Token::KEYWORD, "ge")){
		// ge
		read(Token::KEYWORD, "ge", procName);
		procA();
		buildTree("ge", 2);
	} else if (nextToken.equals(Token::OPERATOR, ">=")) {
		// ge
		read(Token::OPERATOR, ">=", procName);
		procA();
		buildTree("ge", 2);
	} else if (nextToken.equals(Token::KEYWORD, "ls")) {
		//ls
		read(Token::KEYWORD, "ls", procName);
		procA();
		buildTree("ls", 2);
	} else if (nextToken.equals(Token::OPERATOR, "<")) {
		//ls
		read(Token::OPERATOR, "<", procName);
		procA();
		buildTree("ls", 2);
	} else if (nextToken.equals(Token::KEYWORD, "le")) {
		//le
		read(Token::KEYWORD, "le", procName);
		procA();
		buildTree("le", 2);
	} else if (nextToken.equals(Token::OPERATOR, "<=")) {
		//le
		read(Token::OPERATOR, "<=", procName);
		procA();
		buildTree("le", 2);
	} else if (nextToken.equals(Token::KEYWORD, "eq")) {
		//eq
		read(Token::KEYWORD, "eq", procName);
		procA();
		buildTree("eq", 2);
	} else if (nextToken.equals(Token::KEYWORD, "ne")) {
		//ne
		read(Token::KEYWORD, "ne", procName);
		procA();
		buildTree("ne", 2);
	}
}

// left associative + -
// single child neg
void Lexicon :: procA() {
	string procName = "A";
	if (nextToken.equals(Token::OPERATOR, "+")) {
		read(Token::OPERATOR, "+", procName);
		procAt();
	} else if (nextToken.equals(Token::OPERATOR, "-")) {
		read(Token::OPERATOR, "-", procName);
		procAt();
		buildTree("neg", 1);
	} else {
		procAt();
		while (nextToken.equals(Token::OPERATOR, "+") || nextToken.equals(Token::OPERATOR, "-")) {
			if (nextToken.equals(Token::OPERATOR, "+")) {
				read(Token::OPERATOR, "+", procName);
				procAt();
				buildTree("+", 2);
			} else {
				read(Token::OPERATOR, "-", procName);
				procAt();
				buildTree("-", 2);
			}
		}
	}
}

// left associative * /
void Lexicon :: procAt() {
	string procName = "At";
	procAf();
	while (nextToken.equals(Token::OPERATOR, "*") || nextToken.equals(Token::OPERATOR, "/")) {
		if (nextToken.equals(Token::OPERATOR, "*")) {
			read(Token::OPERATOR, "*", procName);
			procAf();
			buildTree("*", 2);
		} else {
			read(Token::OPERATOR, "/", procName);
			procAf();
			buildTree("/", 2);
		}
	}
}

// right associative **
void Lexicon :: procAf() {
	string procName = "Af";
	procAp();
	if (nextToken.equals(Token::OPERATOR, "**")) {
		read(Token::OPERATOR, "**", procName);
		procAf();
		buildTree("**", 2);
	}
}

// left associative @ID
void Lexicon :: procAp() {
	string procName = "Ap";
	procR();
	while (nextToken.equals(Token::OPERATOR, "@")) {
		read(Token::OPERATOR, "@", procName);
		read(Token::IDENTIFIER, "", procName);
		procR();
		buildTree("@", 3);
	}
}

// left associative gamma
void Lexicon :: procR() {
	string procName = "R";
	procRn();
	while (nextToken.equals(Token::PUNCTION, "(") || nextToken.type <= Token::INTEGER) {
		procRn();
		buildTree("gamma", 2);
	}
}

void Lexicon :: procRn() {
	string procName = "Rn";
	if (nextToken.equals(Token::PUNCTION, "(")) {
		read(Token::PUNCTION, "(", procName);
		procE();
		read(Token::PUNCTION, ")", procName);
	} else {
		if (nextToken.type > Token::INTEGER) {
			cout << "Error: A terminal expected, but non-terminal " << nextToken.toString() << " found" << endl;
			exit(0);
		}
		read(nextToken.type, "", procName);
	}
}

// right associative within
void Lexicon :: procD() {
	string procName = "D";
	procDa();
	if (nextToken.equals(Token::KEYWORD, "within")) {
		read(Token::KEYWORD, "within", procName);
		procD();
		buildTree("within", 2);
	}
}

// multi children and
void Lexicon :: procDa() {
	string procName = "Da";
	procDr();
	int n = 1;
	while (nextToken.equals(Token::KEYWORD, "and")) {
		read(Token::KEYWORD, "and", procName);
		procDr();
		n++;
	}
	if (n > 1) {
		buildTree("and", n);
	}
}

// single child rec
void Lexicon :: procDr() {
	string procName = "Dr";
	if (nextToken.equals(Token::KEYWORD, "rec")) {
		read(Token::KEYWORD, "rec", procName);
		procDb();
		buildTree("rec", 1);
	} else {
		procDb();
	}
}

// most tricky one
// multi children fcn_form
void Lexicon :: procDb() {
	string procName = "Db";
	if (nextToken.equals(Token::PUNCTION, "(")) {
		read(Token::PUNCTION, "(", procName);
		procD();
		read(Token::PUNCTION, ")", procName);
	} else {
		read(Token::IDENTIFIER, "", procName);
		if (nextToken.equals(Token::PUNCTION, ",")) {
			// the first id under node"," has been read
			procVl(1);
			read(Token::OPERATOR, "=", procName);
			procE();
			buildTree("=", 2);
		} else if (nextToken.equals(Token::OPERATOR, "=")) {
			read(Token::OPERATOR, "=", procName);
			procE();
			buildTree("=", 2);
		} else {
			procVb();
			int n = 3;
			while (nextToken.equals(Token::PUNCTION, "(") || nextToken.type == Token::IDENTIFIER) {
				procVb();
				n++;
			}
			read(Token::OPERATOR, "=", procName);
			procE();
			buildTree("function_form", n);
		}
	}
}

void Lexicon :: procVb() {
	string procName = "Vb";
	if (nextToken.type == Token::IDENTIFIER) {
		read(Token::IDENTIFIER, "", procName);
	} else {
		read(Token::PUNCTION, "(", procName);
		if (nextToken.equals(Token::PUNCTION, ")")) {
			read(Token::PUNCTION, ")", procName);
			// ???????
			// read(Token::NIL, "()", procName);
		} else {
			procVl();
			read(Token::PUNCTION, ")", procName);
		}
	}
}

// multi children ,
void Lexicon :: procVl() {
	string procName = "Vl";
	read(Token::IDENTIFIER, "", procName);
	int n = 1;
	while (nextToken.equals(Token::PUNCTION, ",")) {
		read(Token::PUNCTION, ",", procName);
		read(Token::IDENTIFIER, "", procName);
		n++;
	}
	if (n > 1) {
		buildTree(",", n);
	}
}

// overload procVl(), before enter, n IDs has been read
void Lexicon :: procVl(int n) {
	string procName = "Vl";
	while (nextToken.equals(Token::PUNCTION, ",")) {
		read(Token::PUNCTION, ",", procName);
		read(Token::IDENTIFIER, "", procName);
		n++;
	}
	buildTree(",", n);
}