CXX = g++
CFLAGS = --std=c++11 -g

src = ./src
bin = ./bin

default: all

all:  ASTNodes.o Token.o Lexicon.o ControlNode.o StackNode.o EnvironmentNode.o CSEMachine.o main.o
	$(CXX) $(CFLAGS) $(bin)/ASTNodes.o $(bin)/Token.o $(bin)/Lexicon.o $(bin)/ControlNode.o $(bin)/StackNode.o $(bin)/EnvironmentNode.o $(bin)/CSEMachine.o $(bin)/main.o -o p2
main.o:  $(src)/main.cc
	$(CXX) $(CFLAGS) -o $(bin)/main.o -c $(src)/main.cc
Lexicon.o:  $(src)/Lexicon.cc
	$(CXX) $(CFLAGS) -o $(bin)/Lexicon.o -c $(src)/Lexicon.cc
Token.o:  $(src)/Token.cc
	$(CXX) $(CFLAGS) -o $(bin)/Token.o -c $(src)/Token.cc
ASTNodes.o:  $(src)/ASTNodes.cc
	$(CXX) $(CFLAGS) -o $(bin)/ASTNodes.o -c $(src)/ASTNodes.cc
CSEMachine.o:  $(src)/CSEMachine.cc
	$(CXX) $(CFLAGS) -o $(bin)/CSEMachine.o -c $(src)/CSEMachine.cc
ControlNode.o:  $(src)/ControlNode.cc
	$(CXX) $(CFLAGS) -o $(bin)/ControlNode.o -c $(src)/ControlNode.cc
StackNode.o:  $(src)/StackNode.cc
	$(CXX) $(CFLAGS) -o $(bin)/StackNode.o -c $(src)/StackNode.cc
EnvironmentNode.o:  $(src)/EnvironmentNode.cc
	$(CXX) $(CFLAGS) -o $(bin)/EnvironmentNode.o -c $(src)/EnvironmentNode.cc
cl:
	rm -f ./bin/*.o p2 ./out/*.out
