CXX = g++
CFLAGS = --std=c++11 -g

src = ./src
bin = ./bin

default: all

all:  ASTNodes.o Token.o Lexicon.o main.o
	$(CXX) $(CFLAGS) $(bin)/ASTNodes.o $(bin)/Token.o $(bin)/Lexicon.o $(bin)/main.o -o p1
main.o:  $(src)/main.cc
	$(CXX) $(CFLAGS) -o $(bin)/main.o -c $(src)/main.cc
Lexicon.o:  $(src)/Lexicon.cc
	$(CXX) $(CFLAGS) -o $(bin)/Lexicon.o -c $(src)/Lexicon.cc
Token.o:  $(src)/Token.cc
	$(CXX) $(CFLAGS) -o $(bin)/Token.o -c $(src)/Token.cc
ASTNodes.o:  $(src)/ASTNodes.cc
	$(CXX) $(CFLAGS) -o $(bin)/ASTNodes.o -c $(src)/ASTNodes.cc
cl:
	rm -f ./bin/*.o p1 ./out/*.out
