#pragma once
#ifndef MINIMIZEDDFA_H_
#define MINIMIZEDDFA_H_

#include <set>
#include <map>
#include "DFA.h"

// 将DFA最小化
class MinimizedDFA {
public:
    //// 最小化的过程也是一样，一个状态集合被视为一个状态，最终的状态集包含这些状态元素
    //set<set<int>> minStates;
    //set<char> alphabet;
    //map<pair<set<int>, char>, set<int>> minTransitions;
    //set<int> startState;
    //set<set<int>> finalStates;

    /*扁平化后的最小DFA*/
    set<int> minStates;     // 状态集
    set<char> alphabet;     // 输入字母表
    map<pair<int, char>, int> minTransitions;       // 状态转换函数
    int startState;         // 开始状态
    set<int> finalStates;   // 终止状态集

    // 构造函数
    MinimizedDFA(const set<int>& minStates, const set<char>& alphabet, const map<pair<int, char>, int>& minTransitions, int startState, const set<int>& finalStates);
    static MinimizedDFA minimize(const DFA& dfa);
    static void printMinimizedDFA(const MinimizedDFA& dfa);
};


#endif // !MINIMIZEDDFA_H_
