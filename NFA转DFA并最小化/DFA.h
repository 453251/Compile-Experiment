#pragma once
#ifndef DFA_H_
#define DFA_H_

#include<set>
#include<map>
#include<utility>
#include"NFA.h"
using namespace std;

// DFA,�Ӽ�����ʵ�ֽ�NFAȷ����ΪDFA
class DFA
{
public:
	///*��ʹ���Ӽ�������NFAȷ����ΪDFAʱ�����״̬��ɵ�״̬���ᱻ��Ϊһ��״̬��״̬������������״̬�ļ��ϣ�����ע�ⶨ�巽ʽ*/
	//set<set<int>> dfaStates;	// ״̬����ÿ��״̬Ҳ��һ��״̬��
	//set<char> alphabet;	// ������ĸ��
	//map<pair<set<int>, char>, set<int>> transitions;	// ״̬ת������
	//set<int> startState;		// ��ʼ״̬
	//set<set<int>> finalStates;	// ��ֹ״̬��

	/*��ƽ��������DFA�������Ƕ�ף����ӻ������Զ���*/
	set<int> dfaStates;		// ״̬��
	set<char> alphabet;		// ������ĸ��
	map<pair<int, char>, int> transitions;		// ״̬ת������	
	int startState;			// ��ʼ״̬
	set<int> finalStates;	// ��ֹ״̬��


	// ���캯����ֱ�ӽ��ձ�ƽ����Ĳ���
	DFA(const set<int>& dfaStates, const set<char>& alphabet, const map<pair<int, char>, int>& transitions, int startState, const set<int>& finalStates);
	// ��̬��������NFAȷ����ΪDFA������ƽ��
	static DFA determinize(const NFA& nfa);
	static void printDFA(const DFA& dfa);

private:
	// epsilon�հ��������Ӽ���
	static set<int> epsilonClosure(const set<int>& states, const map<pair<int, char>, set<int>>& transitions);
	// �Ӽ��������ؾ�����ĸ�ɵ����״̬����
	static set<int> move(const set<int>& states, char symbol, const map<pair<int, char>, set<int>>& transitions);

};

#endif // !DFA_H_
