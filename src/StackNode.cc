#include "StackNode.h"

// --------------------------------------------StackNode--------------------------------------------

const vector<string> StackNode :: typeNames ({"NT", "ID", "STR", "BOOL", "NIL", "DUMMY", "INTEGER", "LAMBDA", "TAU", "ETA", "ENV", "PRINT", "ISINTEGER", "ISTRUTHVALUE", "ISSTRING", "ISTUPLE", "ISDUMMY", "ORDER", "STEM", "STERN", "CONC"});

StackNode :: StackNode(ControlNode* cs){
	type = cs->type;
	valueStr = cs->contentStr;
	valueInt = cs->contentInt;
}

StackNode :: StackNode(ControlNode* cs, int _environmentNumber): environmentNumber(_environmentNumber){
	type = cs->type;
	valueStr = cs->contentStr;
	valueInt = cs->contentInt;
}

StackNode :: StackNode(int _type, string _valueStr, int _valueInt): type(_type), valueStr(_valueStr), valueInt(_valueInt){

}

StackNode :: StackNode(int _type, string _valueStr, int _valueInt, int _environmentNumber): type(_type), valueStr(_valueStr), valueInt(_valueInt), environmentNumber(_environmentNumber){

}

StackNode :: StackNode(int _type, StackNode* sn): type(_type) {
	valueStr = sn->valueStr;
	valueInt = sn->valueInt;
	environmentNumber = sn->environmentNumber;
}

StackNode :: ~StackNode() {
	
}

string StackNode :: toString() {
	return "<" + typeNames[type + 1] + ":" + valueStr + ";" + to_string(valueInt) + ";" + to_string(environmentNumber) + ">";
}