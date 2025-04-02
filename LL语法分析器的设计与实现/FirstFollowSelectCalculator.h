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

	// getter
	map<string, set<string>> getFirstSets();
	map<string, set<string>> getFollowSets();
	map<Production, set<string>> getSelectSets();
};

#endif // !FIRSTFOLLOWSELECTCALCULATOR_H_
