#pragma once
#ifndef FIRSTFOLLOWSELECTCALCULATOR_H_
#define FIRSTFOLLOWSELECTCALCULATOR_H_

#include"Grammar.h"

class FirstFollowSelectCalculator
{
private:
	Grammar grammar;

	map<string, set<string>> firstSets;		// First��
	map<string, set<string>> followSets;	// Follow��
	map<Production, set<string>> selectSets;	// Select��

public:
	FirstFollowSelectCalculator(Grammar& grammar);

	// ����
	void calculateFirstSets();
	void calculateFollowSets();
	void calculateSelectSets();

	// ��ӡ
	void printFirstSets();
	void printFollowSets();
	void printSelectSets();
};

#endif // !FIRSTFOLLOWSELECTCALCULATOR_H_
