#include "ControlNode.h"

// --------------------------------------------ControlNode--------------------------------------------

const vector<string> ControlNode :: typeNames ({"NT", "ID", "STR", "BOOL", "NIL", "DUMMY", "INTEGER", "LAMBDA", "TAU", "BETA", "ENV"});

ControlNode :: ControlNode(int _type, string _contentStr, int _contentInt): type(_type), contentStr(_contentStr), contentInt(_contentInt){

}

ControlNode :: ~ControlNode() {
	
}

string ControlNode :: toString() {
	return "<" + typeNames[type + 1] + ":" + contentStr + ";" + to_string(contentInt) + ">";
}
