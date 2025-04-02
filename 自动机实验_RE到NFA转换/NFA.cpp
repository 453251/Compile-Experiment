#include"NFA.h"
#include<iostream>
using namespace std;

// ���캯��
NFA::NFA(set<int> states, set<char> alphabet, map<pair<int, char>, set<int>> transitions, int startState, set<int> finalStates)
	:states(states), alphabet(alphabet), transitions(transitions), startState(startState), finalStates(finalStates) {}

// �ϲ�����NFA
NFA NFA::merge(const NFA& nfa1, const NFA& nfa2)
{
	/*�ϲ�����NFA*/
	// �ϲ�״̬��
	set<int> newStates = nfa1.states;
	newStates.insert(nfa2.states.begin(), nfa2.states.end());

	// �ϲ�������ĸ��
	set<char> newAlphabet = nfa1.alphabet;
	newAlphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());

	// �ϲ�״̬ת������
	map<pair<int, char>, set<int>> newTransitions = nfa1.transitions;
	for (const auto& transition : nfa2.transitions)
	{
		newTransitions[transition.first].insert(transition.second.begin(), transition.second.end());
	}

	// �ϲ���ʼ״̬������ѡ��ֱ�Ӵ���һ���µĳ�ʼ״̬�������ʼ״̬���Ծ���epsilon�ﵽԭ����������ʼ״̬
	int newStartState = *newStates.rbegin() + 1;	// ���³�̬����һ����һ�޶��ı��
	newStates.insert(newStartState);	// ��Ӧ�أ�״̬���м�������³�̬
	// ״̬ת�������м����³�̬�����մ���-1����ԭ������NFA��̬�ĺ���
	newTransitions[make_pair(newStartState, -1)].insert(nfa1.startState);
	newTransitions[make_pair(newStartState, -1)].insert(nfa2.startState);

	// �ϲ���ֹ״̬��
	set<int> newFinalStates = nfa1.finalStates;
	newFinalStates.insert(nfa2.finalStates.begin(), nfa2.finalStates.end());

	return NFA(newStates, newAlphabet, newTransitions, newStartState, newFinalStates);
}

void NFA::printNFA(NFA& nfa)
{
	cout << "״̬��:";
	for (const int state : nfa.states)
	{
		cout << state << " ";
	}
	cout << endl << "������ĸ��:";
	for (const char alpha : nfa.alphabet)
	{
		cout << alpha << " ";
	}
	cout << endl << "״̬ת������:" << endl;
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
	cout << endl << "��ʼ״̬:" << nfa.startState << endl;
	cout << "��ֹ״̬��:";
	for (const int state : nfa.finalStates)
	{
		cout << state << " ";
	}
	cout << endl;
}

/*�ĸ��������ֱ����ȡ�������ַ������*/
/*�����������NFA�������ַ�*/
NFA NFA::basicNFA(char symbol)
{
	set<int> states = { 0, 1 };
	set<char> alphabet = { symbol };
	map<pair<int, char>, set<int>> transitions = { {{0, symbol}, {1}} };
	return NFA(states, alphabet, transitions, 0, { 1 });
}

/*��������ʽ�е�.�����ַ���nfa1�ĳ�̬Ϊ���ճ�̬��nfa1����̬����epsilon����nfa2�ĳ�̬��nfa2����̬Ϊ������̬*/
NFA NFA::concatNFA(const NFA& nfa1, const NFA& nfa2)
{
	/*״̬��*/
	set<int> newStates = nfa1.states;
	/*ƫ����*/
	int offset = *nfa1.states.rbegin() + 1;
	for (int state : nfa2.states)
	{
		newStates.insert(state + offset);
	}


	/*������ĸ��*/
	set<char> newAlphabet = nfa1.alphabet;
	newAlphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());


	/*״̬ת������*/
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

	/*��nfa1����̬���ӵ�nfa2�ĳ�ʼ״̬*/
	for (int finalState : nfa1.finalStates)
	{
		newTransitions[{finalState, -1}].insert(nfa2.startState + offset);
	}

	/*nfa2����̬�����˼�offset*/
	set<int> newFinalStates;
	for (int finalState : nfa2.finalStates)
	{
		newFinalStates.insert(finalState + offset);
	}

	return NFA(newStates, newAlphabet, newTransitions, nfa1.startState, newFinalStates);
}

/*��������ʽ�е�|���ַ�������³�̬������̬���³�̬����epsilon����ԭ��̬��ԭ��̬����epsilon��������̬*/
NFA NFA::unionNFA(const NFA& nfa1, const NFA& nfa2)
{
	/*״̬��*/
	set<int> newStates = nfa1.states;
	// ƫ����
	int offset = *nfa1.states.rbegin() + 1;
	for (int state : nfa2.states)
	{
		newStates.insert(state + offset);
	}

	/*����һ���µĳ�̬����̬*/
	int newStartState = *newStates.rbegin() + 1;
	int newFinalState = newStartState + 1;
	// ��Ӧ�ؼ��뵽״̬����
	newStates.insert(newStartState); newStates.insert(newFinalState);

	/*������ĸ��*/
	set<char> newAlphabet = nfa1.alphabet;
	newAlphabet.insert(nfa2.alphabet.begin(), nfa2.alphabet.end());

	/*״̬ת������*/
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
	/*�³�̬�����մ�����ԭ���ĳ�̬*/
	newTransitions[{newStartState, -1}].insert(nfa1.startState);
	newTransitions[{newStartState, -1}].insert(nfa2.startState+offset);
	/*ԭ����̬�����մ���������̬*/
	for (int finalState : nfa1.finalStates) newTransitions[{finalState, -1}].insert(newFinalState);
	for (int finalState : nfa2.finalStates) newTransitions[{finalState + offset, -1}].insert(newFinalState);

	return NFA(newStates, newAlphabet, newTransitions, newStartState, { newFinalState });
}

/*
	��������ʽ��*�հ��Ĵ�����ѭ������ԭ��̬��ԭ��̬��epsilonʵ�֣�Ȼ�����ù�������³�̬������̬����epsilon����
	����*�հ�����Ϊ�մ����ʻ���Ҫ����³�̬������̬��epsilon
*/
NFA NFA::kleeneStar(const NFA& nfa)
{
	/*״̬��*/
	set<int> newStates = nfa.states;
	/*�³�̬����̬*/
	int newStartState = *nfa.states.rbegin() + 1;
	int newFinalState = newStartState + 1;
	// ��Ӧ�أ����뵽״̬����
	newStates.insert(newStartState); newStates.insert(newFinalState);

	/*״̬ת������*/
	map<pair<int, char>, set<int>> newTransitions = nfa.transitions;
	/*�³�̬�����մ�����ԭ��̬������̬*/
	newTransitions[{newStartState, -1}].insert(nfa.startState);
	newTransitions[{newStartState, -1}].insert(newFinalState);
	/*ԭ��̬�����մ���������̬��ԭ��̬*/
	for (int finalState : nfa.finalStates)
	{
		newTransitions[{finalState, -1}].insert(newFinalState);
		newTransitions[{finalState, -1}].insert(nfa.startState);
	}

	return NFA(newStates, nfa.alphabet, newTransitions, newStartState, { newFinalState });
}