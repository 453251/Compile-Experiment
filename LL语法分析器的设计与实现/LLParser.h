#pragma once
#ifndef LLPARSER_H_
#define LLPARSER_H_

#include"LLParserTable.h"
#include<vector>
#include<stack>
using namespace std;

class LLParser
{
private:
	LLParserTable table;
	stack<string> parseStack;
	vector<string> inputTokens;

public:
	LLParser(LLParserTable& table);
	void parse(const string& filename);
};

#endif // !LLPARSER_H_
