#include "header.h"
#include "StackNode.h"


using namespace std;

class EnvironmentNode {
public:
	int envNodeNum;
	map<string, StackNode*> valueTable;
	EnvironmentNode* parent;
	
	EnvironmentNode(int _envNodeNum, EnvironmentNode* _parent);
	~EnvironmentNode();
};