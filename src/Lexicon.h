#include "Token.h"
#include "ASTNodes.h"

using namespace std;

class Lexicon {
public:
	static const vector<char> OPERATOR_SYMBOL;
	static const vector<char> PUNCTIONS;
	static const vector<char> SPACES;
	static const vector<char> ESCAPES;
	static const vector<string> KEYWORDS;
	static const vector<string> BOOLS; 
	static const vector<string> NILS; 
	static const vector<string> DUMMYS; 

	string fileName;
    ifstream file;
    int lineNum;
    int colNum;
    char nextChar;
    ASTNode* root = NULL;
    stack<ASTNode*> stackOfNodes;
    Token nextToken;
    bool reachedEOF = false;
    int debugMode = 0;
    set<int> STTreeTypes;
    set<string> STTreeContents;

	Lexicon(const char* fileName);
	~Lexicon();
	void print();
	void reopen();
	bool belongsTo(char ch, const vector<char> category);
	bool belongsTo(string ch, const vector<string> category);
	bool hasNext();
	Token getNextToken();
	Token startId(int sL, int sC, string content);
	Token startInt(int sL, int sC, string content);
	Token startString(int sL, int sC, string content);
	Token backSlash(int sL, int sC, string content);
	Token startOperator(int sL, int sC, string content);

	// tree
	void buildTree(string name, int numOfCh);
	void printTree();
	void printErrorTree();
	void printTreeHelper(ASTNode* node, int level);
	void standardizeTree();
	void standardizeHelper(ASTNode* node);
	void standardizeLet(ASTNode* node);
	void standardizeWhere(ASTNode* node);
	void standardizeMultiParams(ASTNode* node);
	void standardizeWithin(ASTNode* node);
	void standardizeAt(ASTNode* node);
	void standardizeAnd(ASTNode* node);
	void standardizeRec(ASTNode* node);
	void checkNode(ASTNode* node, string name);
	void destroyAST(ASTNode* node);

	// procedure
	void startParse();
	void read(int type, string content, string calledBy);
	void procE();
	void procEw();
	void procT();
	void procTa();
	void procTc();
	void procB();
	void procBt();
	void procBs();
	void procBp();
	void procA();
	void procAt();
	void procAf();
	void procAp();
	void procR();
	void procRn();
	void procD();
	void procDa();
	void procDr();
	void procDb();
	void procVb();
	void procVl();
	void procVl(int n);

	void errorPrint(string msg,int sL, int sC);
	void setDebugMode(int i);
};