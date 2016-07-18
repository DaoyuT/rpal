#include "Lexicon.h"
#include "CSEMachine.h"

using namespace std;

int main(int argc, char *argv[]) {
	if (argc == 2) {
		Lexicon lex (argv[1]);
		lex.startParse();
		lex.standardizeTree();
    	CSEMachine cseMachine (lex.root);
    	cseMachine.printControlStructure();
    	cseMachine.runMachine();
	}else if (argc == 3) {
	string option(argv[1]);
	if (option.compare("-ast") == 0) {
		//lex.setDebugMode(1);
		Lexicon lex (argv[2]);
		lex.startParse();
		lex.printTree();
		cout << "-----------------standardizeTree-------------------" << endl;
		lex.standardizeTree();
		lex.printTree();
		/*vector<int> a(10);
		vector<int> b;
		a.push_back(20);
		b = a;
		a.push_back(30);
		for (int i : b) {
			cout << i << endl;
		}
		for (int i : a) {
			cout << i << endl;
		}
		vector<int> c ;
		c.push_back(1);
		for (int i : c) {
			cout << i << endl;
		}
		map<int, vector<int>> m;
		m.insert(pair<int,vector<int>>(0,a));
		m.at(0).push_back(4);
		for (int i : m.at(0)) {
			cout << i << endl;
		}
		set<int>::iterator it;
		for (it=lex.STTreeTypes.begin(); it!=lex.STTreeTypes.end(); ++it){
    		cout << ' ' << *it;
		}
    	cout << "\n";
		set<string>::iterator it2;
		for (it2=lex.STTreeContents.begin(); it2!=lex.STTreeContents.end(); ++it2){
    		cout << ' ' << *it2;
		}
    	cout << "\n";*/
/*    	map<int, vector<ASTNode*>> m;
    	vector<ASTNode*> v0;
    	vector<ASTNode*> v1;
    	m.insert( pair<int, vector<ASTNode*>>(0, v0));
    	m.insert( pair<int, vector<ASTNode*>>(1, v1));
    	m.at(0).push_back(lex.root);
    	m.at(0).push_back(lex.root);
    	m.at(0).push_back(lex.root);
    	m.at(0).push_back(lex.root);
    	m.at(0).push_back(lex.root);
    	m.at(0).push_back(lex.root);
    	m.at(1).push_back(lex.root->left);
    	m.at(1).push_back(lex.root->left);
    	m.at(1).push_back(lex.root->left);
    	m.at(1).push_back(lex.root->left);
    	m.at(1).push_back(lex.root->left);
    	printInt(m.size());
    	cout << lex.OPERATOR_SYMBOL[0] << endl;
   		vector<int> v;
   		v.push_back(0);
   		v.push_back(1);
   		v.push_back(2);
   		v.push_back(3);
   		vector<int>::reverse_iterator rit;
		int offset = v.size();
		for (rit = v.rbegin(); rit != v.rend(); rit++) {
			offset--;
			if (*rit == 2) {
				break;
			}
		}
		v.erase(v.begin() + offset);
		for (int i : v) {
			cout << i << endl;
		}*//*
		string a = "x,a,g,z,g,a";
		string b = "x0";
		char c = ',';
		cout << cseMachine.split(a,c).size() << endl;
		cout << cseMachine.split(b,c).size() << endl;*/
	} else if (option.compare("-debug") == 0) {
		Lexicon lex (argv[2]);
		lex.startParse();
		lex.printTree();
		cout << "-----------------standardizeTree-------------------" << endl;
		lex.standardizeTree();
		lex.printTree();
    	CSEMachine cseMachine (lex.root);
    	cseMachine.printControlStructure();
    	cseMachine.runMachine();
	} else if (option.compare("-l") == 0) {
		cout << "-ast:		Show abstract syntax tree." << endl;
	} else {
		cout << "Unknown arguments: " << argv[1] << " and " << argv[2] << endl;
	}
	}

/*	else {
		lex.nextToken.print();
		cout << lex.nextToken.equals(Token::IDENTIFIER, "let") << endl;
		Token str = lex.nextToken;
		//lex.read(1, "let");
		cout << "-----------------------------------" << endl;
		lex.reopen();
		lex.print();
		lex.reopen();
		cout << "-----------------------------------" << endl;
		while (lex.hasNext()) {
			Token token = lex.getNextToken();
			token.print();
		}
		ASTNode* x1 = new ASTNode_ID(1, "x");
		ASTNode* five = new ASTNode_INT(5);
		ASTNode* print = new ASTNode_ID(1, "Print");
		ASTNode* x2 = new ASTNode_ID(3, "true");
		lex.stackOfNodes.push(x1);
		lex.stackOfNodes.push(five);
		lex.buildTree("neg", 1);
		lex.buildTree("=", 2);
		lex.stackOfNodes.push(print);
		lex.stackOfNodes.push(x2);
		lex.buildTree("garmma", 2);
		lex.buildTree("let", 2);
		lex.root = lex.stackOfNodes.top();
		lex.stackOfNodes.pop();
		lex.printTree();
	}*/

}
