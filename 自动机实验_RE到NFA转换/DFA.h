#pragma once
#ifndef DFA_H_
#define DFA_H_

#include<set>
#include<map>
#include<utility>
#include"NFA.h"
using namespace std;

// DFA,子集法，实现将NFA确定化为DFA
class DFA
{
public:
	///*在使用子集法，将NFA确定化为DFA时，多个状态组成的状态集会被视为一个状态，状态集是类似这种状态的集合，所以注意定义方式*/
	//set<set<int>> dfaStates;	// 状态集，每个状态也是一个状态集
	//set<char> alphabet;	// 输入字母表
	//map<pair<set<int>, char>, set<int>> transitions;	// 状态转换函数
	//set<int> startState;		// 开始状态
	//set<set<int>> finalStates;	// 终止状态集

	/*扁平化处理后的DFA，否则会嵌套，可视化后难以读懂*/
	set<int> dfaStates;		// 状态集
	set<char> alphabet;		// 输入字母表
	map<pair<int, char>, int> transitions;		// 状态转换函数	
	int startState;			// 开始状态
	set<int> finalStates;	// 终止状态集


	// 构造函数，直接接收扁平化后的参数
	DFA(const set<int>& dfaStates, const set<char>& alphabet, const map<pair<int, char>, int>& transitions, int startState, const set<int>& finalStates);
	// 静态函数，将NFA确定化为DFA，并扁平化
	static DFA determinize(const NFA& nfa);
	static void printDFA(const DFA& dfa);

private:
	// epsilon闭包，用于子集法
	static set<int> epsilonClosure(const set<int>& states, const map<pair<int, char>, set<int>>& transitions);
	// 子集法，返回经过字母可到达的状态集合
	static set<int> move(const set<int>& states, char symbol, const map<pair<int, char>, set<int>>& transitions);

};

#endif // !DFA_H_
