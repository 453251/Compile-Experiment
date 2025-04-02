#include"NFA.h"
#include<iostream>
using namespace std;

// 构造函数
NFA::NFA(set<int> states, set<char> alphabet, map<pair<int, char>, set<int>> transitions, int startState, set<int> finalStates)
	:states(states), alphabet(alphabet), transitions(transitions), startState(startState), finalStates(finalStates){}

// 合并两个NFA
NFA NFA::merge(const NFA& nfa1, const NFA& nfa2)
{
	/*合并两个NFA*/
	// 合并状态集
	set<int> newStates = nfa1.states;	
	newStates.insert(nfa2.states.begin(), nfa2.states.end());

	// 合并输入字母表
	set<char> newAlphabet = nfa1.alphabet;
	newAlphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());

	// 合并状态转换函数
	map<pair<int, char>, set<int>> newTransitions = nfa1.transitions;
	for (const auto& transition : nfa2.transitions)
	{
		newTransitions[transition.first].insert(transition.second.begin(), transition.second.end());
	}

	// 合并开始状态，我们选择直接创建一个新的初始状态，这个初始状态可以经过epsilon达到原来的两个初始状态
	int newStartState = *newStates.rbegin() + 1;	// 给新初态分配一个独一无二的编号
	newStates.insert(newStartState);	// 相应地，状态集中加入这个新初态
	// 状态转换函数中加入新初态经过空串（-1）到原来两个NFA初态的函数
	newTransitions[make_pair(newStartState, -1)].insert(nfa1.startState);
	newTransitions[make_pair(newStartState, -1)].insert(nfa2.startState);

	// 合并终止状态集
	set<int> newFinalStates = nfa1.finalStates;
	newFinalStates.insert(nfa2.finalStates.begin(), nfa2.finalStates.end());

	return NFA(newStates, newAlphabet, newTransitions, newStartState, newFinalStates);
}

void NFA::printNFA(NFA& nfa)
{
	cout << "状态集:";
	for (const int state : nfa.states)
	{
		cout << state << " ";
	}
	cout << endl << "输入字母表:";
	for (const char alpha : nfa.alphabet)
	{
		cout << alpha << " ";
	}
	cout << endl << "状态转换函数:" << endl;
	for (const auto& transition : nfa.transitions)
	{
		if (transition.first.second == -1)
		{
			cout << "  (" << transition.first.first << ", " << -1 << ") -> ";
		}
		else
		{
			cout << "  (" << transition.first.first << ", " << transition.first.second << ") -> ";
		}
		for (const int state : transition.second)
		{
			cout << state << " ";
		}
		cout << endl;
	}
	cout << endl << "初始状态:" << nfa.startState << endl;
	cout << "终止状态集:";
	for (const int state : nfa.finalStates)
	{
		cout << state << " ";
	}
	cout << endl;
}
