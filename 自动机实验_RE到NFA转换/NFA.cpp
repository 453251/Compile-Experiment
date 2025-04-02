#include"NFA.h"
#include<iostream>
using namespace std;

// 构造函数
NFA::NFA(set<int> states, set<char> alphabet, map<pair<int, char>, set<int>> transitions, int startState, set<int> finalStates)
	:states(states), alphabet(alphabet), transitions(transitions), startState(startState), finalStates(finalStates) {}

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

/*四个函数，分别处理读取到四种字符的情况*/
/*构建最基本的NFA，接收字符*/
NFA NFA::basicNFA(char symbol)
{
	set<int> states = { 0, 1 };
	set<char> alphabet = { symbol };
	map<pair<int, char>, set<int>> transitions = { {{0, symbol}, {1}} };
	return NFA(states, alphabet, transitions, 0, { 1 });
}

/*处理正规式中的.连接字符，nfa1的初态为最终初态，nfa1的终态经过epsilon到达nfa2的初态，nfa2的终态为最后的终态*/
NFA NFA::concatNFA(const NFA& nfa1, const NFA& nfa2)
{
	/*状态集*/
	set<int> newStates = nfa1.states;
	/*偏移量*/
	int offset = *nfa1.states.rbegin() + 1;
	for (int state : nfa2.states)
	{
		newStates.insert(state + offset);
	}


	/*输入字母表*/
	set<char> newAlphabet = nfa1.alphabet;
	newAlphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());


	/*状态转换函数*/
	map<pair<int, char>, set<int>> newTransitions = nfa1.transitions;
	for (const auto& transition : nfa2.transitions)
	{
		pair<int, char> newKey = { transition.first.first + offset, transition.first.second };
		set<int> newTargets;
		for (int target : transition.second)
		{
			newTargets.insert(target + offset);
		}
		newTransitions[newKey].insert(newTargets.begin(), newTargets.end());
	}

	/*将nfa1的终态连接到nfa2的初始状态*/
	for (int finalState : nfa1.finalStates)
	{
		newTransitions[{finalState, -1}].insert(nfa2.startState + offset);
	}

	/*nfa2的终态别忘了加offset*/
	set<int> newFinalStates;
	for (int finalState : nfa2.finalStates)
	{
		newFinalStates.insert(finalState + offset);
	}

	return NFA(newStates, newAlphabet, newTransitions, nfa1.startState, newFinalStates);
}

/*处理正规式中的|或字符，添加新初态和新终态，新初态经过epsilon到达原初态，原终态经过epsilon到达新终态*/
NFA NFA::unionNFA(const NFA& nfa1, const NFA& nfa2)
{
	/*状态集*/
	set<int> newStates = nfa1.states;
	// 偏移量
	int offset = *nfa1.states.rbegin() + 1;
	for (int state : nfa2.states)
	{
		newStates.insert(state + offset);
	}

	/*分配一个新的初态和终态*/
	int newStartState = *newStates.rbegin() + 1;
	int newFinalState = newStartState + 1;
	// 相应地加入到状态集中
	newStates.insert(newStartState); newStates.insert(newFinalState);

	/*输入字母表*/
	set<char> newAlphabet = nfa1.alphabet;
	newAlphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());

	/*状态转换函数*/
	map<pair<int, char>, set<int>> newTransitions = nfa1.transitions;
	for (const auto& transition : nfa2.transitions)
	{
		pair<int, char> newKey = { transition.first.first + offset, transition.first.second };
		set<int> newTargets;
		for (int target : transition.second)
		{
			newTargets.insert(target + offset);
		}
		newTransitions[newKey].insert(newTargets.begin(), newTargets.end());
	}
	/*新初态经过空串到达原本的初态*/
	newTransitions[{newStartState, -1}].insert(nfa1.startState);
	newTransitions[{newStartState, -1}].insert(nfa2.startState+offset);
	/*原本终态经过空串到达新终态*/
	for (int finalState : nfa1.finalStates) newTransitions[{finalState, -1}].insert(newFinalState);
	for (int finalState : nfa2.finalStates) newTransitions[{finalState + offset, -1}].insert(newFinalState);

	return NFA(newStates, newAlphabet, newTransitions, newStartState, { newFinalState });
}

/*
	对于正规式中*闭包的处理，自循环采用原终态到原初态的epsilon实现，然后利用规则加入新初态和新终态，用epsilon连接
	由于*闭包可以为空串，故还需要添加新初态到新终态的epsilon
*/
NFA NFA::kleeneStar(const NFA& nfa)
{
	/*状态集*/
	set<int> newStates = nfa.states;
	/*新初态和终态*/
	int newStartState = *nfa.states.rbegin() + 1;
	int newFinalState = newStartState + 1;
	// 相应地，加入到状态集中
	newStates.insert(newStartState); newStates.insert(newFinalState);

	/*状态转换函数*/
	map<pair<int, char>, set<int>> newTransitions = nfa.transitions;
	/*新初态经过空串到达原初态和新终态*/
	newTransitions[{newStartState, -1}].insert(nfa.startState);
	newTransitions[{newStartState, -1}].insert(newFinalState);
	/*原终态经过空串到达新终态和原初态*/
	for (int finalState : nfa.finalStates)
	{
		newTransitions[{finalState, -1}].insert(newFinalState);
		newTransitions[{finalState, -1}].insert(nfa.startState);
	}

	return NFA(newStates, nfa.alphabet, newTransitions, newStartState, { newFinalState });
}