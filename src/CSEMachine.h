#include "ASTNodes.h"
#include "EnvironmentNode.h"

using namespace std;

class CSEMachine {
public:
	map<int, vector<ControlNode*>> controlStructure;
	vector<ControlNode*> controler;
	vector<StackNode*> stack;
	vector<EnvironmentNode*> environment;
	vector<int> envNumStack;


	CSEMachine(ASTNode* root);
	~CSEMachine();
	void buildControlStructure(ASTNode* node, int number);
	void addControlNode(ControlNode* cs, int number);
	void printControlStructure();
	void printControlStructure2();
	void runMachine();
	void loadControlerWithDelta(int deltaNumber);
	void loadControlerWithEnv(int envNumber);
	void loadStackWithEnv(int envNumber);
	void checkType(StackNode* cs, int expected);
	void checkType(StackNode* cs, int expected1, int expected2);
	void checkType(ControlNode* cs, int expected);
	void buildEnvironment(int currEnvNum, int newEnvNum, string paramStr, StackNode* param);
	StackNode* findID(string id, EnvironmentNode* env);
	void loadEnvironmentZero();
	void print(StackNode* printValue);
	vector<string> split(string str, char delimiter);
	void printStack();
	void printControler();
	void printEnv(int i);
	void printEnv();
};