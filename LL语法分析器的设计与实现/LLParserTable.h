#pragma once
#ifndef LLPARSERTABLE_H_
#define LLPARSERTABLE_H_

#include"FirstFollowSelectCalculator.h"
#include<map>
#include<vector>

class LLParserTable
{
private:
	map<string, map<string, Production>> table;
	Grammar grammar;
	FirstFollowSelectCalculator calculator;

public:
	LLParserTable(Grammar& grammar);
	void constructTable();
	void printTable();

	// getter
	map<string, map<string, Production>> getTable();
	Grammar getGrammar();
	FirstFollowSelectCalculator getFirstFollowSelectCalculator();
};

#endif // !LLPARSERTABLE_H_
