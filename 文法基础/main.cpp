#include"Production.h"
#include"Grammar.h"
#include"FirstFollowSelectCalculator.h"

#include<iostream>
using namespace std;

int main(int argc, char* argv[])
{
	string filename = argv[1];
	Grammar grammar;
	grammar.loadFromFile(filename);
	grammar.printGrammar();
	FirstFollowSelectCalculator firstFollowSelectCalculator(grammar);
	firstFollowSelectCalculator.calculateFirstSets();
	firstFollowSelectCalculator.calculateFollowSets();
	firstFollowSelectCalculator.calculateSelectSets();
	firstFollowSelectCalculator.printFirstSets();
	firstFollowSelectCalculator.printFollowSets();
	firstFollowSelectCalculator.printSelectSets();
	return 0;
	//vector<string> v(3);
	//v.push_back("what");
	//v.push_back("can");
	//v.push_back("I");
	//if (find(v.begin(), v.end(), "I") != v.end())
	//	cout << "mamba out!";
}