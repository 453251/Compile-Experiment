#pragma once
#ifndef MINIMIZEDDFA_H_
#define MINIMIZEDDFA_H_

#include <set>
#include <map>
#include "DFA.h"

// ��DFA��С��
class MinimizedDFA {
public:
    //// ��С���Ĺ���Ҳ��һ����һ��״̬���ϱ���Ϊһ��״̬�����յ�״̬��������Щ״̬Ԫ��
    //set<set<int>> minStates;
    //set<char> alphabet;
    //map<pair<set<int>, char>, set<int>> minTransitions;
    //set<int> startState;
    //set<set<int>> finalStates;

    /*��ƽ�������СDFA*/
    set<int> minStates;     // ״̬��
    set<char> alphabet;     // ������ĸ��
    map<pair<int, char>, int> minTransitions;       // ״̬ת������
    int startState;         // ��ʼ״̬
    set<int> finalStates;   // ��ֹ״̬��

    // ���캯��
    MinimizedDFA(const set<int>& minStates, const set<char>& alphabet, const map<pair<int, char>, int>& minTransitions, int startState, const set<int>& finalStates);
    static MinimizedDFA minimize(const DFA& dfa);
    static void printMinimizedDFA(const MinimizedDFA& dfa);
};


#endif // !MINIMIZEDDFA_H_
