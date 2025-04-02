#include"Production.h"
#include"Grammar.h"
#include"FirstFollowSelectCalculator.h"
#include"LLParser.h"
#include"LLParserTable.h"

#include<iostream>
using namespace std;

int main(int argc, char*argv[])
{
	Grammar grammar;
	string filename = argv[1];
	grammar.loadFromFile(filename);
	grammar.printGrammar();
	FirstFollowSelectCalculator firstFollowSelectCalculator(grammar);
	firstFollowSelectCalculator.calculateFirstSets();
	firstFollowSelectCalculator.calculateFollowSets();
	firstFollowSelectCalculator.calculateSelectSets();
	firstFollowSelectCalculator.printFirstSets();
	firstFollowSelectCalculator.printFollowSets();
	firstFollowSelectCalculator.printSelectSets();
	LLParserTable llParserTable(grammar);
	llParserTable.constructTable();
	llParserTable.printTable();
	LLParser llParser(llParserTable);
	string inputTokenFilename = argv[2];
	llParser.parse(inputTokenFilename);
	return 0;
}