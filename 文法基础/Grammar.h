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
	vector<string> nonTerminals;	// ���ս������
	vector<string> terminals;		// �ս������
	vector<Production> productions;	// ����ʽ����
	string startSymbol;				// ��ʼ����

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
