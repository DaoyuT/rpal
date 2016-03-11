#include "ASTNodes.h"

/*
ASTNode :: ASTNode() {}

// delete whole tree ???
ASTNode :: ~ASTNode() {}*/

// --------------------------------------------ASTNode_NT--------------------------------------------

ASTNode_NT :: ASTNode_NT (string name): name(name) {}

ASTNode_NT :: ~ASTNode_NT() {}

string ASTNode_NT :: toString() {
	return name;
}

// --------------------------------------------ASTNode_ID--------------------------------------------

const vector<string> ASTNode_ID :: names ({"ID", "STR", "BOOL", "NIL", "DUMMY"});

ASTNode_ID :: ASTNode_ID(int type, string content): type(type), content(content) {}

ASTNode_ID :: ~ASTNode_ID() {}

string ASTNode_ID :: toString() {
	if (type <= ASTNode_ID::STRING) {
		return "<" + names[type] + ":" + content + ">" ;
	} else {
		return "<" + content + ">" ;
	}
}

// --------------------------------------------ASTNode_INT--------------------------------------------

ASTNode_INT :: ASTNode_INT(int content): content(content) {}

ASTNode_INT :: ~ASTNode_INT() {}

string ASTNode_INT :: toString() {
	return "<" + name + ":" + to_string(content) + ">" ;
}