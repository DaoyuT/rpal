#include "CSEMachine.h"

// --------------------------------------------CSEMachine--------------------------------------------

CSEMachine :: CSEMachine(ASTNode* root) {
	buildControlStructure(root, 0);
	loadControlerWithEnv(0);
	loadControlerWithDelta(0);
	loadStackWithEnv(0);
	envNumStack.push_back(0);
	loadEnvironmentZero();
}

CSEMachine :: ~CSEMachine() {

}

void CSEMachine :: buildControlStructure(ASTNode* node, int deltaNumber) {
	if (node == NULL) {
		return;
	}
	if (node->getType() == -1 && node->getContent() == "lambda") { // deal ,
		ASTNode* param = node->left;
		string paramStr = param->getContent();
		if (node->getType() == -1 && paramStr == ",") {
			ASTNode* p = param->left;
			paramStr = p->getContent();
			p = p->right;
			while (p != NULL) {
				paramStr += "," + p->getContent();
				p = p->right;
			}
		}
		if(controlStructure.find(deltaNumber) == controlStructure.end()) {
			vector<ControlNode*> newVec;
			controlStructure.insert( pair<int, vector<ControlNode*>>(deltaNumber, newVec) );
		}
		ControlNode* lambda = new ControlNode(ControlNode::LAMBDA, paramStr, controlStructure.size());
		addControlNode(lambda, deltaNumber);
		buildControlStructure(param->right, controlStructure.size());
		buildControlStructure(node->right, deltaNumber);
		return;
	} else if (node->getType() == -1 && node->getContent() == "->") {
		ASTNode* condition = node->left;
		ASTNode* ifClause = condition->right;
		ASTNode* elseClause = ifClause->right;
		condition->right = NULL;
		ifClause->right = NULL;
		if (controlStructure.find(deltaNumber) == controlStructure.end()) {
			vector<ControlNode*> newVec;
			controlStructure.insert( pair<int, vector<ControlNode*>>(deltaNumber, newVec) );
		}
		int ifNumber = controlStructure.size();
		buildControlStructure(ifClause, controlStructure.size());
		int elseNumber = controlStructure.size();
		buildControlStructure(elseClause, controlStructure.size());
		ControlNode* beta = new ControlNode(ControlNode::BETA, to_string(ifNumber), elseNumber);
		addControlNode(beta, deltaNumber);
		buildControlStructure(condition, deltaNumber);
		condition->right = ifClause;
		ifClause->right = elseClause;
		return;
	} else if (node->getType() == -1 && node->getContent() == "tau") {
		int n = 0;
		ASTNode* p = node->left;
		while (p != NULL) {
			n++;
			p = p->right;
		}
		ControlNode* tau = new ControlNode(ControlNode::TAU, "tau", n);
		addControlNode(tau, deltaNumber);
	}
	else {
		ControlNode* cs = new ControlNode(node->getType(), node->getContent(), 0);
		addControlNode(cs, deltaNumber);
	}
	buildControlStructure(node->left, deltaNumber);
	buildControlStructure(node->right, deltaNumber);
}

void CSEMachine :: addControlNode(ControlNode* cs, int deltaNumber) {
	if (controlStructure.find(deltaNumber) != controlStructure.end()) {
		controlStructure.at(deltaNumber).push_back(cs);
	} else {
		vector<ControlNode*> v;
		v.push_back(cs);
		controlStructure.insert( pair<int, vector<ControlNode*>>(deltaNumber, v));
	}
}

void CSEMachine :: printControlStructure() {
	map<int, vector<ControlNode*>> ::iterator it;
	for (it=controlStructure.begin(); it!=controlStructure.end(); ++it){
		cout << "\nDelta " << it->first << ":" << endl;
		vector<ControlNode*> v = it->second;
		for (ControlNode* cs : v) {
			cout << cs->toString() << endl;
		}
	}
}

void CSEMachine :: printControlStructure2() {
	map<int, vector<ControlNode*>> ::iterator it;
	for (it=controlStructure.begin(); it!=controlStructure.end(); ++it){
		cout << "\nDelta " << it->first << ":" ;
		vector<ControlNode*> v = it->second;
		for (ControlNode* cs : v) {
			cout << " " <<cs->toString();
		}
	}
	cout << endl;
}

void CSEMachine :: runMachine() {
/*		printControler();
		cout << "\t";
		printStack();
		cout << envNumStack.back() << endl;
		if (environment.at(envNumStack.back())->parent != NULL){
			cout << environment.at(envNumStack.back())->parent->envNodeNum << endl;
		}*/
		//printEnv(envNumStack.back());
	while (!controler.empty()) {
		ControlNode* topNode = controler.back();
		controler.pop_back();
		if (topNode->type >= ControlNode::STRING && topNode->type <= ControlNode::DUMMY) {
			stack.push_back(new StackNode(topNode));
		} else if (topNode->type == ControlNode::INTEGER) {
			stack.push_back(new StackNode(topNode->type, "", stoi(topNode->contentStr)));
		} else if (topNode->type == ControlNode::LAMBDA) {
			// lambda
			stack.push_back(new StackNode(topNode, envNumStack.back()));
		} else if (topNode->type == ControlNode::TAU) {
			// tau if n == 0 ???
			int n = topNode->contentInt;
			StackNode* firstElement = stack.back();
			stack.pop_back();
			n--;
			StackNode* curr = firstElement;
			StackNode*  next = NULL;
			while (n > 0) {
				next = stack.back();
				stack.pop_back();
				n--;
				curr->nextElement = next;
				curr = next;
			}
			StackNode* newTau = new StackNode(topNode);
			newTau->tauNext = firstElement;
			stack.push_back(newTau);
		} else if (topNode->type == ControlNode::BETA) {
			// beta
			StackNode* condition = stack.back();
			stack.pop_back();
			checkType(condition, StackNode::BOOL);
			if (condition->valueStr == "true") {
				loadControlerWithDelta(stoi(topNode->contentStr));
			} else {
				loadControlerWithDelta(topNode->contentInt);
			}
		} else if (topNode->type == ControlNode::ENV) {
			// environment node, exit environment
			int envNumber = topNode->contentInt;
			vector<StackNode*>::reverse_iterator rit;
			int offset = stack.size();
			for (rit = stack.rbegin(); rit != stack.rend(); rit++) {
				offset--;
				if ((*rit)->type == StackNode::ENV && (*rit)->environmentNumber == envNumber) {
					break;
				}
			}
			stack.erase(stack.begin() + offset);
			if (envNumStack.back() != envNumber) {
				cout << "Environment not match: currEnvNum = " << envNumStack.back() << ", envNumber = " << envNumber << endl;
				exit(0);
			}
			envNumStack.pop_back();
		} else if (topNode->type == ControlNode::IDENTIFIER) {
			// id TODO
			int currEnvNum = envNumStack.back();
			if (currEnvNum >= environment.size()) {
				cout << currEnvNum << " fked " << environment.size() << endl;
			}
			StackNode* foundID = findID(topNode->contentStr, environment.at(envNumStack.back()));
			if (foundID != NULL) {
				stack.push_back(foundID);
			} else {
				cout << "ID not found: " << topNode->contentStr << endl;
				exit(0);
			}
		}else if (topNode->type == ControlNode::NT) {
			// nt: uop bop Y eta & gamma 
			if (topNode->contentStr == "gamma") {
				StackNode* nextOnStack = stack.back();
				if (nextOnStack->type == StackNode::LAMBDA) {
					// apply lambda
					stack.pop_back();
					StackNode* parameter = stack.back();
					stack.pop_back();
					// open a new environment
					int newEnvNum = environment.size();
					loadControlerWithEnv(newEnvNum);
					envNumStack.push_back(newEnvNum);
					loadControlerWithDelta(nextOnStack->valueInt);
					loadStackWithEnv(newEnvNum);
					// bind parameter TODO
					buildEnvironment(nextOnStack->environmentNumber, newEnvNum, nextOnStack->valueStr, parameter);
				} else if (nextOnStack->type == StackNode::NT && nextOnStack->valueStr == "YStar") {
					// apply YStar
					stack.pop_back();
					StackNode* lambda = stack.back();
					stack.pop_back();
					StackNode* eta = new StackNode(StackNode::ETA, lambda);
					stack.push_back(eta);
				} else if (nextOnStack->type == StackNode::ETA) {
					// apply eta
					ControlNode* newGamma1 = new ControlNode(ControlNode::NT, "gamma", 0);
					ControlNode* newGamma2 = new ControlNode(ControlNode::NT, "gamma", 0);
					controler.push_back(newGamma1);
					controler.push_back(newGamma2);
					StackNode* eta = stack.back();
					StackNode* newLambda = new StackNode(StackNode::LAMBDA, eta);
					stack.push_back(newLambda);
				} else if (nextOnStack->type == StackNode::TAU) {
					// apply tau
					StackNode* tau = stack.back();
					stack.pop_back();
					StackNode* kth = stack.back();
					stack.pop_back();
					checkType(kth, StackNode::INTEGER);
					StackNode* curr = tau->tauNext;
					int k = kth->valueInt--;
					while (k > 0) {
						if (curr == NULL) {
							cout << "Apply an tuple out of its order" << endl;
							exit(0);
						}
						curr = curr->nextElement;
						k--;
					}
					stack.push_back(curr);
				} else if (nextOnStack->type == StackNode::PRINT) {
					stack.pop_back();
					StackNode* printValue = stack.back();
					stack.pop_back();
					print(printValue);
					cout << endl;
				} else if (nextOnStack->type == StackNode::ORDER) {
					stack.pop_back();
					StackNode* tau = stack.back();
					checkType(tau, StackNode::TAU, StackNode::NIL);
					stack.pop_back();
					if (tau->type == StackNode::NIL) {
						stack.push_back(new StackNode(StackNode::INTEGER, "", 0));
					} else {
						StackNode* curr = tau->tauNext;
						int n = 0;
						while (curr != NULL) {
							n++;
							curr = curr->nextElement;
						}
						stack.push_back(new StackNode(StackNode::INTEGER, "", n));
					}
				} else if (nextOnStack->type == StackNode::ISINTEGER) {
					stack.pop_back();
					StackNode* value = stack.back();
					stack.pop_back();
					if (value->type == StackNode::INTEGER) {
						stack.push_back(new StackNode(StackNode::BOOL, "true", 0));
					} else {
						stack.push_back(new StackNode(StackNode::BOOL, "false", 0));
					}
				} else if (nextOnStack->type == StackNode::ISTRUTHVALUE) {
					stack.pop_back();
					StackNode* value = stack.back();
					stack.pop_back();
					if (value->type == StackNode::BOOL) {
						stack.push_back(new StackNode(StackNode::BOOL, "true", 0));
					} else {
						stack.push_back(new StackNode(StackNode::BOOL, "false", 0));
					}
				} else if (nextOnStack->type == StackNode::ISSTRING) {
					stack.pop_back();
					StackNode* value = stack.back();
					stack.pop_back();
					if (value->type == StackNode::STRING) {
						stack.push_back(new StackNode(StackNode::BOOL, "true", 0));
					} else {
						stack.push_back(new StackNode(StackNode::BOOL, "false", 0));
					}
				} else if (nextOnStack->type == StackNode::ISTUPLE) {
					stack.pop_back();
					StackNode* value = stack.back();
					stack.pop_back();
					if (value->type == StackNode::TAU || value->type == StackNode::NIL) {
						stack.push_back(new StackNode(StackNode::BOOL, "true", 0));
					} else {
						stack.push_back(new StackNode(StackNode::BOOL, "false", 0));
					}
				} else if (nextOnStack->type == StackNode::ISDUMMY) {
					stack.pop_back();
					StackNode* value = stack.back();
					stack.pop_back();
					if (value->type == StackNode::DUMMY) {
						stack.push_back(new StackNode(StackNode::BOOL, "true", 0));
					} else {
						stack.push_back(new StackNode(StackNode::BOOL, "false", 0));
					}
				} else if (nextOnStack->type == StackNode::CONC) {
					stack.pop_back();
					StackNode* value1 = stack.back();
					stack.pop_back();
					StackNode* value2 = stack.back();
					stack.pop_back();
					checkType(value1, StackNode::STRING);
					checkType(value2, StackNode::STRING);
					stack.push_back(new StackNode(StackNode::STRING, value1->valueStr + value2->valueStr, 0));
				}
			} else if (topNode->contentStr == "YStar") {
				StackNode* newYStar = new StackNode(topNode);
				stack.push_back(newYStar);
			} else if (topNode->contentStr == "not"){
				StackNode* rand1 = stack.back();
				stack.pop_back();
				checkType(rand1, StackNode::BOOL);
				if (rand1->valueStr == "true") {
					stack.push_back(new StackNode(StackNode::BOOL, "false", 0));
				} else {
					stack.push_back(new StackNode(StackNode::BOOL, "true", 0));
				}
			} else if (topNode->contentStr == "or") {
				StackNode* rand1 = stack.back();
				stack.pop_back();
				checkType(rand1, StackNode::BOOL);
				StackNode* rand2 = stack.back();
				stack.pop_back();
				checkType(rand2, StackNode::BOOL);
				if (rand1->valueStr == "false" && rand2->valueStr == "false") {
					stack.push_back(new StackNode(StackNode::BOOL, "false", 0));
				} else {
					stack.push_back(new StackNode(StackNode::BOOL, "true", 0));
				}
			} else if (topNode->contentStr == "&") {
				StackNode* rand1 = stack.back();
				stack.pop_back();
				checkType(rand1, StackNode::BOOL);
				StackNode* rand2 = stack.back();
				stack.pop_back();
				checkType(rand2, StackNode::BOOL);
				if (rand1->valueStr == "true" && rand2->valueStr == "true") {
					stack.push_back(new StackNode(StackNode::BOOL, "true", 0));
				} else {
					stack.push_back(new StackNode(StackNode::BOOL, "false", 0));
				}
			} else if (topNode->contentStr == "eq") {
				StackNode* rand1 = stack.back();
				stack.pop_back();
				StackNode* rand2 = stack.back();
				stack.pop_back();
				if (rand1->type == rand2->type && (rand1->type == StackNode::BOOL || rand1->type == StackNode::INTEGER || rand1->type == StackNode::STRING)) {
					if (rand1->valueStr == rand2->valueStr && rand1->valueInt == rand2->valueInt) {
						stack.push_back(new StackNode(StackNode::BOOL, "true", 0));
					} else {
						stack.push_back(new StackNode(StackNode::BOOL, "false", 0));
					}
				} else {
					cout << "Type error while trying to eq:" << endl;
					cout << rand1->toString() << endl;
					cout << "and" << endl;
					cout << rand2->toString() << endl;
				}
			} else if (topNode->contentStr == "ne") {
				StackNode* rand1 = stack.back();
				stack.pop_back();
				StackNode* rand2 = stack.back();
				stack.pop_back();
				if (rand1->type == rand2->type && (rand1->type == StackNode::BOOL || rand1->type == StackNode::INTEGER || rand1->type == StackNode::STRING)) {
					if (rand1->valueStr == rand2->valueStr && rand1->valueInt == rand2->valueInt) {
						stack.push_back(new StackNode(StackNode::BOOL, "false", 0));
					} else {
						stack.push_back(new StackNode(StackNode::BOOL, "true", 0));
					}
				} else {
					cout << "Type error while trying to eq:" << endl;
					cout << rand1->toString() << endl;
					cout << "and" << endl;
					cout << rand2->toString() << endl;
				}
			} else if (topNode->contentStr == "+") {
				StackNode* rand1 = stack.back();
				stack.pop_back();
				checkType(rand1, StackNode::INTEGER);
				StackNode* rand2 = stack.back();
				stack.pop_back();
				checkType(rand2, StackNode::INTEGER);
				stack.push_back(new StackNode(StackNode::INTEGER, "", rand1->valueInt + rand2->valueInt));
			} else if (topNode->contentStr == "-") {
				StackNode* rand1 = stack.back();
				stack.pop_back();
				checkType(rand1, StackNode::INTEGER);
				StackNode* rand2 = stack.back();
				stack.pop_back();
				checkType(rand2, StackNode::INTEGER);
				stack.push_back(new StackNode(StackNode::INTEGER, "", rand1->valueInt - rand2->valueInt));
			} else if (topNode->contentStr == "*") {
				StackNode* rand1 = stack.back();
				stack.pop_back();
				checkType(rand1, StackNode::INTEGER);
				StackNode* rand2 = stack.back();
				stack.pop_back();
				checkType(rand2, StackNode::INTEGER);
				stack.push_back(new StackNode(StackNode::INTEGER, "", rand1->valueInt * rand2->valueInt));
			} else if (topNode->contentStr == "**") {
				StackNode* rand1 = stack.back();
				stack.pop_back();
				checkType(rand1, StackNode::INTEGER);
				StackNode* rand2 = stack.back();
				stack.pop_back();
				checkType(rand2, StackNode::INTEGER);
				stack.push_back(new StackNode(StackNode::INTEGER, "", pow(rand1->valueInt, rand2->valueInt)));				
			} else if (topNode->contentStr == "/") {
				StackNode* rand1 = stack.back();
				stack.pop_back();
				checkType(rand1, StackNode::INTEGER);
				StackNode* rand2 = stack.back();
				stack.pop_back();
				checkType(rand2, StackNode::INTEGER);
				stack.push_back(new StackNode(StackNode::INTEGER, "", rand1->valueInt / rand2->valueInt));				
			} else if (topNode->contentStr == "ls" || topNode->contentStr == "<") {
				StackNode* rand1 = stack.back();
				stack.pop_back();
				checkType(rand1, StackNode::INTEGER);
				StackNode* rand2 = stack.back();
				stack.pop_back();
				checkType(rand2, StackNode::INTEGER);
				if (rand1->valueInt < rand2->valueInt) {
					stack.push_back(new StackNode(StackNode::BOOL, "true", 0));
				} else {
					stack.push_back(new StackNode(StackNode::BOOL, "false", 0));
				}
			} else if (topNode->contentStr == "gr" || topNode->contentStr == ">") {
				StackNode* rand1 = stack.back();
				stack.pop_back();
				checkType(rand1, StackNode::INTEGER);
				StackNode* rand2 = stack.back();
				stack.pop_back();
				checkType(rand2, StackNode::INTEGER);
				if (rand1->valueInt > rand2->valueInt) {
					stack.push_back(new StackNode(StackNode::BOOL, "true", 0));
				} else {
					stack.push_back(new StackNode(StackNode::BOOL, "false", 0));
				}
			} else if (topNode->contentStr == "le" || topNode->contentStr == "<=") {
				StackNode* rand1 = stack.back();
				stack.pop_back();
				checkType(rand1, StackNode::INTEGER);
				StackNode* rand2 = stack.back();
				stack.pop_back();
				checkType(rand2, StackNode::INTEGER);
				if (rand1->valueInt <= rand2->valueInt) {
					stack.push_back(new StackNode(StackNode::BOOL, "true", 0));
				} else {
					stack.push_back(new StackNode(StackNode::BOOL, "false", 0));
				}
			} else if (topNode->contentStr == "ge" || topNode->contentStr == ">=") {
				StackNode* rand1 = stack.back();
				stack.pop_back();
				checkType(rand1, StackNode::INTEGER);
				StackNode* rand2 = stack.back();
				stack.pop_back();
				checkType(rand2, StackNode::INTEGER);
				if (rand1->valueInt >= rand2->valueInt) {
					stack.push_back(new StackNode(StackNode::BOOL, "true", 0));
				} else {
					stack.push_back(new StackNode(StackNode::BOOL, "false", 0));
				}
			} else if (topNode->contentStr == "aug") {
				StackNode* rand1 = stack.back();
				stack.pop_back();
				checkType(rand1, StackNode::TAU, StackNode::NIL);
				StackNode* rand2 = stack.back();
				if (rand1->type == StackNode::TAU) {
					StackNode* p = rand1;
					while (p->nextElement != NULL) {
						p = p->nextElement;
					}
					p->nextElement = rand2;
					stack.push_back(rand1);
				} else {
					StackNode* newTau = new StackNode(StackNode::TAU, "tau", 0);
					newTau->tauNext = rand2;
					stack.push_back(newTau);
				}
			}
		}
/*		printControler();
		cout << "\t";
		printStack();
		cout << envNumStack.back() << endl;
		if (environment.at(envNumStack.back())->parent != NULL){
			cout << environment.at(envNumStack.back())->parent->envNodeNum << endl;
		}*/
		//printEnv();
		//printEnv(envNumStack.back());
	}
}

void CSEMachine :: checkType(StackNode* sn, int expected) {
	if (sn->type != expected) {
		cout << "Type Error:"<< endl;
		cout << sn->toString() << endl;
		cout << "Type " << StackNode::typeNames[expected + 1] << " expected, but type " << StackNode::typeNames[sn->type + 1] << " found." << endl;
		exit(0);
	}
}

void CSEMachine :: checkType(StackNode* sn, int expected1, int expected2) {
	if (sn->type != expected1 && sn->type != expected2) {
		cout << "Type Error:"<< endl;
		cout << sn->toString() << endl;
		cout << "Type " << StackNode::typeNames[expected1 + 1] << " or " + StackNode::typeNames[expected2 + 1] + " expected, but type " << StackNode::typeNames[sn->type + 1] << " found." << endl;
		exit(0);
	}
}

void CSEMachine :: checkType(ControlNode* cs, int expected) {
	if (cs->type != expected) {
		cout << "Type Error:"<< endl;
		cout << cs->toString() << endl;
		cout << "Type " << ControlNode::typeNames[expected + 1] << " expected, but type " << ControlNode::typeNames[cs->type + 1] << " found." << endl;
		exit(0);
	}
}

void CSEMachine :: loadControlerWithDelta(int deltaNumber) {
	controler.insert(controler.end(), controlStructure.at(deltaNumber).begin(), controlStructure.at(deltaNumber).end());
}

void CSEMachine :: loadControlerWithEnv(int envNumber) {
	controler.push_back(new ControlNode(ControlNode::ENV, "env", envNumber));
}

void CSEMachine :: loadStackWithEnv(int envNumber) {
	stack.push_back(new StackNode(StackNode::ENV, "env", 0, envNumber));
}

void CSEMachine :: buildEnvironment(int currEnvNum, int newEnvNum, string paramStr, StackNode* param) {
			if (currEnvNum >= environment.size()) {
				cout << "wow";
				cout << currEnvNum << " fked " << environment.size() << endl;
			}
	EnvironmentNode* envNode = new EnvironmentNode(newEnvNum, environment.at(currEnvNum));
	vector<string> params = split(paramStr, ',');
	if (params.size() > 1) {
		checkType(param, StackNode::TAU);
		StackNode* curr = param;
		int i = 0;
		for (i = 0; i < params.size() && curr != NULL; i++) {
			envNode->valueTable.insert( pair<string, StackNode*>(params.at(i), curr) );
			curr = curr->nextElement;
		}
	} else {
		envNode->valueTable.insert( pair<string, StackNode*>(paramStr, param) );
	}
	environment.push_back(envNode);
}

StackNode* CSEMachine :: findID(string id, EnvironmentNode* env) {
	if (env == NULL) {
		return NULL;
	}
	if (env->valueTable.find(id) != env->valueTable.end()) {
		return env->valueTable.at(id);
	} else {
		return findID(id, env->parent);
	}
}

void CSEMachine :: loadEnvironmentZero() {
	EnvironmentNode* env0 = new EnvironmentNode(0, NULL);
	env0->valueTable.insert( pair<string, StackNode*>("Print", new StackNode(StackNode::PRINT, "Print", 0)) );
	env0->valueTable.insert( pair<string, StackNode*>("Order", new StackNode(StackNode::ORDER, "Order", 0)) );
	env0->valueTable.insert( pair<string, StackNode*>("Isinteger", new StackNode(StackNode::ISINTEGER, "Isinteger", 0)) );
	env0->valueTable.insert( pair<string, StackNode*>("Istruthvalue", new StackNode(StackNode::ISTRUTHVALUE, "Istruthvalue", 0)) );
	env0->valueTable.insert( pair<string, StackNode*>("Isstring", new StackNode(StackNode::ISSTRING, "Isstring", 0)) );
	env0->valueTable.insert( pair<string, StackNode*>("Istuple", new StackNode(StackNode::ISTUPLE, "Istuple", 0)) );
	env0->valueTable.insert( pair<string, StackNode*>("IsDummy", new StackNode(StackNode::ISDUMMY, "IsDummy", 0)) );
	env0->valueTable.insert( pair<string, StackNode*>("Conc", new StackNode(StackNode::CONC, "Conc", 0)) );
	environment.push_back(env0);
}

void CSEMachine :: print(StackNode* printValue) {
	if (printValue->type >= StackNode::STRING && printValue->type <= StackNode::DUMMY) {
		cout << printValue->valueStr;
	} else if (printValue->type == StackNode::INTEGER ) {
		cout << to_string(printValue->valueInt);
	} else if (printValue->type == StackNode::TAU){
		cout << "(";
		StackNode* curr = printValue->tauNext;
		print(curr);
		curr = curr->nextElement;
		while (curr != NULL) {
			cout << ", ";
			print(curr);
			curr = curr->nextElement;
		}
		cout << ")";
	} else {
		cout << "Error: Trying to print a value of type " << printValue->type << endl;
		cout << printValue->toString() << endl;
		exit(0);
	}
}

vector<string> CSEMachine :: split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
  
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }
  
  return internal;
}

void CSEMachine :: printStack() {
	vector<StackNode*>::reverse_iterator rit = stack.rbegin();
	for (; rit!= stack.rend(); ++rit){
		cout << (*rit)->toString() << " ";
	}
	cout << endl;
}

void CSEMachine :: printControler() {
	vector<ControlNode*>::iterator it = controler.begin();
	for (; it!= controler.end(); ++it){
		cout << (*it)->toString() << " ";
	}
	cout << endl;
}

void CSEMachine :: printEnv(int envNum) {
	if (envNum >= environment.size()) {
		cout << "fk" << endl;
		cout << envNum << " >= " << environment.size();
	}
	EnvironmentNode* env = environment.at(envNum);
	for (map<string, StackNode*>::iterator it=env->valueTable.begin(); it!=env->valueTable.end(); ++it)
    	cout << it->first << " => " << it->second->toString() << '\n';
}

void CSEMachine :: printEnv() {
	for (EnvironmentNode* env : environment) {
		cout << "EnvNo " << env->envNodeNum << endl;
		cout << "ParNo " << env->parent->envNodeNum << endl;
		for (map<string, StackNode*>::iterator it=env->valueTable.begin(); it!=env->valueTable.end(); ++it)
    		cout << it->first << " => " << it->second->toString() << '\n';
	}
}