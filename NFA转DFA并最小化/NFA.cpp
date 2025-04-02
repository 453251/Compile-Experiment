#include"NFA.h"
#include<iostream>
using namespace std;

// ���캯��
NFA::NFA(set<int> states, set<char> alphabet, map<pair<int, char>, set<int>> transitions, int startState, set<int> finalStates)
	:states(states), alphabet(alphabet), transitions(transitions), startState(startState), finalStates(finalStates){}

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
