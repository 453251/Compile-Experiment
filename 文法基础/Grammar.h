#pragma once
#ifndef GRAMMAR_H_
#define GRAMMAR_H_

#include"Production.h"
#include<string>
#include<vector>
#include<map>
#include<set>
using namespace std;

class Grammar
{
private:
	vector<string> nonTerminals;	// 非终结符集合
	vector<string> terminals;		// 终结符集合
	vector<Production> productions;	// 产生式集合
	string startSymbol;				// 开始符号

public:
	void loadFromFile(const string& filename);
	void printGrammar();

	// getter
	vector<string> getNonTerminals();
	vector<string> getTerminals();
	vector<Production> getProductions();
	string getStartSymbol();


};

#endif // !GRAMMAR_H_
