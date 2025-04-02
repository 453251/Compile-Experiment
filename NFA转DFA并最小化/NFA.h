#pragma once
#ifndef NFA_H_
#define NFA_H_

#include<set>
#include<map>
#include<utility>
using namespace std;

class NFA
{
public:
	set<int> states;	// 有穷状态集
	set<char> alphabet;	// 输入字母表
	map<pair<int, char>, set<int>> transitions;	// 状态转换函数
	int startState;		// 开始状态
	set<int> finalStates;	// 终止状态集

	NFA(set<int> states, set<char> alphabet, map<pair<int, char>, set<int>> transitions, int startState, set<int> finalStates);

	// 静态函数merge，因为函数参数是两个NFA
	static NFA merge(const NFA& nfa1, const NFA& nfa2);
	// 静态函数printNFA，用于输出
	static void printNFA(NFA& nfa);
};

#endif // !NFA_H_