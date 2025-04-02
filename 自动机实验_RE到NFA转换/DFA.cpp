#include"DFA.h"
#include<queue>
#include<stack>
#include<iostream>
using namespace std;


// ���캯����ֱ�ӽ��ձ�ƽ����Ĳ���
DFA::DFA(const set<int>& dfaStates, const set<char>& alphabet, const map<pair<int, char>, int>& transitions, int startState, const set<int>& finalStates)
	: dfaStates(dfaStates), alphabet(alphabet), transitions(transitions), startState(startState), finalStates(finalStates) {}

// NFAȷ����ΪDFA
DFA DFA::determinize(const NFA& nfa)
{
	// ���NFA�Ƿ��Ѿ�ȷ����
	bool alreadyDeterminized = true;
	for (const auto& transition : nfa.transitions) {
		/*�������ĳ���ַ������˶��״̬������ȷ����*/
		if (transition.second.size() > 1) {
			alreadyDeterminized = false;
			break;
		}
		/*���������epsilon������ȷ����*/
		if (transition.first.second == -1)
		{
			alreadyDeterminized = false;
			break;
		}
	}

	if (alreadyDeterminized) {
		// ���NFA�Ѿ�ȷ������ֱ�ӽ�NFAת��ΪDFA
		cout << endl << "NFA��ȷ������ֱ��ת��ΪDFA" << endl;
		set<int> dfaStates = nfa.states;
		set<char> alphabet = nfa.alphabet;
		map<pair<int, char>, int> transitions;

		for (const auto& transition : nfa.transitions) {
			int fromState = transition.first.first;
			char symbol = transition.first.second;
			int toState = *transition.second.begin();  // ������ȷ������ֻ��һ��Ŀ��״̬
			transitions[{fromState, symbol}] = toState;
		}

		int startState = nfa.startState;
		set<int> finalStates = nfa.finalStates;

		return DFA(dfaStates, alphabet, transitions, startState, finalStates);
	}

	/*�����Ӽ�������NFAȷ����ΪDFA����Ҫ˼·Ϊ�����ҵ���ʼ״̬��epsilon�հ�����Ϊ�µĳ�ʼ״̬��Ȼ��ӳ�ʼ״̬��ʼ
	  ����״̬ת�����������е���ĸ�����������������������״̬���뵽״̬���У�ͬʱ����״̬ת����������ֹ״̬��
	*/
	// ״̬����
	set<set<int>> dfaStates;

	// ������ĸ����NFA��ͬ
	set<char> alphabet = nfa.alphabet;

	// ״̬ת������
	map<pair<set<int>, char>, set<int>> dfaTransitions;

	// ��ʼ״̬��������nfa�ĳ�ʼ״̬��epsilon�հ�
	set<int> startState = epsilonClosure({ nfa.startState }, nfa.transitions);
	dfaStates.insert(startState);	// ��Ӧ�أ�����ʼ״̬����״̬������
	// ά��һ������״̬ת�����������Ķ���
	queue<set<int>> stateQueue;
	stateQueue.push(startState);	// ��ʼ״̬��������������

	// ��ֹ״̬��
	set<set<int>> finalStates;

	// ����״̬ת����������������
	while (!stateQueue.empty())
	{
		set<int> currentState = stateQueue.front(); stateQueue.pop();

		// ��ȡһ��symbol�󣬾Ϳ�����״̬��������ĸ����һ������
		for (char symbol : alphabet)
		{
			// ����һ������
			set<int> nextState = move(currentState, symbol, nfa.transitions);

			// ���ҽ�����������״̬���ϲ�Ϊ��ʱ������հ������ҽ�������뵽״̬�����У�������״̬ת������
			// ����ǿռ���ֱ�������ˣ���Ȼ�ͻ�����״̬
			if (!nextState.empty())
			{
				// �����˶�������Ľ����հ�
				nextState = epsilonClosure(nextState, nfa.transitions);

				// Ȼ�󽫽�����뵽״̬�����У�ͬʱ����״̬ת������
				if (!dfaStates.count(nextState))
				{
					dfaStates.insert(nextState);
					stateQueue.push(nextState);
				}
				dfaTransitions[{currentState, symbol}] = nextState;
			}

			// ���ڰ���ԭ��ֹ״̬��״̬����Ϊ����һ��״̬����һ��״̬����������Ϊ��ֹ״̬�����뵽��ֹ״̬����
			for (int state : currentState)
			{
				if (nfa.finalStates.count(state))
				{
					finalStates.insert(currentState);
					break;	// ��һ�ξ͹��ˣ����ظ���
				}
			}
		}
	}

	// ---- ��ʼ���б�ƽ������ ----
	map<set<int>, int> stateMapping;
	int stateCounter = 0;

	// ���ɱ�ƽ�����״̬ӳ���
	set<int> flattenedStates;
	for (const auto& stateSet : dfaStates) {
		stateMapping[stateSet] = stateCounter++;
		flattenedStates.insert(stateMapping[stateSet]);
	}

	// ��ƽ��ת����
	map<pair<int, char>, int> flattenedTransitions;
	for (const auto& [key, value] : dfaTransitions) {
		int fromState = stateMapping[key.first];
		int toState = stateMapping[value];
		flattenedTransitions[{fromState, key.second}] = toState;
	}

	// ��ƽ����̬��
	set<int> flattenedFinalStates;
	for (const auto& stateSet : finalStates) {
		flattenedFinalStates.insert(stateMapping[stateSet]);
	}

	// ��ƽ����ʼ״̬
	int flattenedStartState = stateMapping[startState];

	// �����µı�ƽ����� DFA
	return DFA(flattenedStates, alphabet, flattenedTransitions, flattenedStartState, flattenedFinalStates);
}

// Ѱ��״̬�ıհ�
set<int> DFA::epsilonClosure(const set<int>& states, const map<pair<int, char>, set<int>>& transitions)
{
	/*Ѱ��״̬��epsilon�հ������ﲻֹѰ�ҳ�ʼ״̬����ʼ״̬���ǵ�һ��һ��״̬��������״̬�п�����һ��״̬��*/
	set<int> closure = states;
	// stateStack����Ѱ�ұհ�
	stack<int> stateStack;

	// �ȼ�������״̬
	for (int state : states)
	{
		stateStack.push(state);
	}

	// ����stateStack�е�״̬������״̬ת��������������
	while (!stateStack.empty())
	{
		// ȡ��ջ��״̬����������
		int state = stateStack.top(); stateStack.pop();

		// ������ڣ�״̬���մ����ļ��������ֵ
		if (transitions.count(make_pair(state, -1)))
		{
			// ���ɵ����״̬����հ���stack��
			for (int nextState : transitions.at(make_pair(state, -1)))
			{
				if (!closure.count(nextState))
				{
					// ��������ڣ�����룬�����ظ�����
					closure.insert(nextState);
					stateStack.push(nextState);
				}
			}
		}
	}
	return closure;
}

// ����״̬ת��������һ��������ͬʱ���뻹��Ҫһ��������ĸ�����򲻺��Ҽ�
set<int> DFA::move(const set<int>& states, char symbol, const map<pair<int, char>, set<int>>& transitions)
{
	/*����״̬ת��������һ������*/
	set<int> nextState;
	for (int state : states)
	{
		if (transitions.count(make_pair(state, symbol)))
		{
			nextState.insert(transitions.at(make_pair(state, symbol)).begin(), transitions.at(make_pair(state, symbol)).end());
		}
	}
	return nextState;
}

void DFA::printDFA(const DFA& dfa)
{
	cout << "״̬��:";
	for (const int state : dfa.dfaStates)
	{
		cout << state << " ";
	}
	cout << endl << "������ĸ��:";
	for (const char alpha : dfa.alphabet)
	{
		cout << alpha << " ";
	}
	cout << endl << "״̬ת������:" << endl;
	for (const auto& transition : dfa.transitions)
	{
		cout << "  (" << transition.first.first << ", " << transition.first.second << ") -> " << transition.second;
		cout << endl;
	}
	cout << endl << "��ʼ״̬:" << dfa.startState << endl;
	cout << "��ֹ״̬��:";
	for (const int state : dfa.finalStates)
	{
		cout << state << " ";
	}
	cout << endl;
}



