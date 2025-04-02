#pragma once
#ifndef FIRSTFOLLOWSELECTCALCULATOR_H_
#define FIRSTFOLLOWSELECTCALCULATOR_H_

#include"Grammar.h"

class FirstFollowSelectCalculator
{
private:
	Grammar grammar;

	map<string, set<string>> firstSets;		// First集
	map<string, set<string>> followSets;	// Follow集
	map<Production, set<string>> selectSets;	// Select集

public:
	FirstFollowSelectCalculator(Grammar& grammar);

	// 计算
	void calculateFirstSets();
	void calculateFollowSets();
	void calculateSelectSets();

	// 打印
	void printFirstSets();
	void printFollowSets();
	void printSelectSets();
};

#endif // !FIRSTFOLLOWSELECTCALCULATOR_H_
