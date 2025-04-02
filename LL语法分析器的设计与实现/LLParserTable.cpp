#include"LLParserTable.h"
#include<iomanip>
#include<iostream>
using namespace std;

LLParserTable::LLParserTable(Grammar& grammar):grammar(grammar), calculator(grammar)
{
	calculator.calculateFirstSets();
	calculator.calculateFollowSets();
	calculator.calculateSelectSets();
}

void LLParserTable::constructTable()
{
	vector<Production> productions = grammar.getProductions();	// ����ʽ
	map<Production, set<string>> selectSets = calculator.getSelectSets();	// select��

	for (const auto& production : productions)
	{
		set<string> selectSet = selectSets[production];
		for (const auto& terminal : selectSet)
		{
			table[production.left][terminal] = production;
		}
	}
}

void LLParserTable::printTable()
{
	const int cellWidth = 16;
	vector<string> terminals = grammar.getTerminals();	// �ս��
	vector<string> nonTerminals = grammar.getNonTerminals();	// ���ս��
	cout << "Ԥ�������:" << endl;
	cout << setw(cellWidth) << " ";
	for (const auto& terminal : terminals) {
		std::cout << setw(cellWidth) << terminal;
	}
	cout << setw(cellWidth) << "#" << endl;
	for (const auto& nonTerminal : nonTerminals)
	{
		cout << setw(cellWidth) << nonTerminal;
		for (const auto& terminal : terminals)
		{
			if (table.at(nonTerminal).count(terminal))
			{
				cout << setw(cellWidth) << table.at(nonTerminal).at(terminal).toString();
			}
			else
			{
				cout << setw(cellWidth) << " ";
			}
		}
		cout << endl;
	}
}

map<string, map<string, Production>> LLParserTable::getTable()
{
	return table;
}

Grammar LLParserTable::getGrammar()
{
	return grammar;
}

FirstFollowSelectCalculator LLParserTable::getFirstFollowSelectCalculator()
{
	return calculator;
}

