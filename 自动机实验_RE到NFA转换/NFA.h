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
	set<int> states;	// ����״̬��
	set<char> alphabet;	// ������ĸ��
	map<pair<int, char>, set<int>> transitions;	// ״̬ת������
	int startState;		// ��ʼ״̬
	set<int> finalStates;	// ��ֹ״̬��

	NFA(set<int> states, set<char> alphabet, map<pair<int, char>, set<int>> transitions, int startState, set<int> finalStates);

	// ��̬����merge����Ϊ��������������NFA
	static NFA merge(const NFA& nfa1, const NFA& nfa2);
	// ��̬����printNFA���������
	static void printNFA(NFA& nfa);

	/*���ڽ�����ʽת��ΪNFA*/
	/*�ĸ��������ֱ����ȡ�������ַ������*/
	static NFA basicNFA(char symbol);
	static NFA concatNFA(const NFA& nfa1, const NFA& nfa2);
	static NFA unionNFA(const NFA& nfa1, const NFA& nfa2);
	static NFA kleeneStar(const NFA& nfa);
};

#endif // !NFA_H_