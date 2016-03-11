#include "Lexicon.h"

using namespace std;

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cout << "Got " << argc - 1 << " args, need 2 args!" << endl;
		return 1;
	}
	string option(argv[1]);
	if (option.compare("-ast") == 0) {
		//lex.setDebugMode(1);
		Lexicon lex (argv[2]);
		lex.start();
		lex.printTree();
	} else if (option.compare("-l") == 0) {
		cout << "-ast:		Show abstract syntax tree." << endl;
	} else {
		cout << "Unknown arguments: " << argv[1] << " and " << argv[2] << endl;
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