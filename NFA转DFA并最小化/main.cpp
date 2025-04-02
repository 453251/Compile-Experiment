#include "MINIMIZEDDFA.h"
#include "DFA.h"
#include"NFA.h"
#include <iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<sstream>
#include<vector>
using namespace std;

//// 测试函数
//void testDeterminize() {
//    // 构造NFA
//    set<int> states = { 1, 2, 3, 4 };
//    set<char> alphabet = { '0', '1' };
//
//    map<pair<int, char>, set<int>> transitions;
//    // 状态转换：0输入0到1，1输入0到2，1输入1到3，2输入1到3
//    transitions[{1, '0'}] = { 2 };
//    transitions[{1, '1'}] = { 3 };
//    transitions[{3, -1}] = { 4 };
//    transitions[{4, '1'}] = { 4 };
//    transitions[{4, '0'}] = { 4 };
//    transitions[{4, -1}] = { 2 };
//
//    int startState = 1;
//    set<int> finalStates = { 2 };
//
//    // 构造NFA对象
//    NFA nfa(states, alphabet, transitions, startState, finalStates);
//
//    // NFA确定化为DFA
//    DFA dfa = DFA::determinize(nfa);
//
//    // 打印DFA的结果
//    cout << "DFA 状态集: { ";
//    for (int state : dfa.dfaStates) {
//        cout << state << " ";
//    }
//    cout << "}" << endl;
//
//    cout << "符号表: { ";
//    for (char symbol : dfa.alphabet) {
//        cout << symbol << " ";
//    }
//    cout << "}" << endl;
//
//    cout << "状态转换:" << endl;
//    for (const auto& transition : dfa.transitions) {
//        cout << "  (" << transition.first.first << ", " << transition.first.second << ") -> " << transition.second << endl;
//    }
//
//    cout << "初始状态: " << dfa.startState << endl;
//
//    cout << "终止状态集: { ";
//    for (int finalState : dfa.finalStates) {
//        cout << finalState << " ";
//    }
//    cout << "}" << endl;
//}
//
//int main() {
//    // 创建一个示例 DFA
//    // DFA 状态转移表 (A = 0, B = 1, C = 2)
//    // 状态转移表：
//    // | 状态 | 输入 0 | 输入 1 |
//    // |------|--------|--------|
//    // |   A  |    B   |    C   |
//    // |   B  |    B   |    A   |
//    // |   C  |    C   |    A   |
//
//    //set<int> dfaStates = { 0, 1, 2, 3, 4, 5};  // 状态集 A, B, C
//    //set<char> alphabet = { '0', '1' };  // 输入符号集
//
//    //map<pair<int, char>, int> transitions = {
//    //    {{0, '0'}, 1}, {{0, '1'}, 2},  // A -> B, A -> C
//    //    {{1, '0'}, 1}, {{1, '1'}, 4},  // B -> B, B -> A
//    //    {{2, '0'}, 1}, {{2, '1'}, 3},   // C -> C, C -> A
//    //    {{3, '0'}, 3}, {{3, '1'}, 2},
//    //    {{4, '0'}, 0}, {{4, '1'}, 5},
//    //    {{5, '0'}, 5}, {{5, '1'}, 4}
//    //};
//
//    //int startState = 0;  // A 作为起始状态
//    //set<int> finalStates = { 0, 1 };  // C 作为终态
//
//    //set<int> dfaStates = { 0, 1, 2, 3 };  // 状态集 A, B, C
//    //set<char> alphabet = { '0', '1' };  // 输入符号集
//
//    //map<pair<int, char>, int> transitions = {
//    //    {{0, '0'}, 2}, {{0, '1'}, 1},  
//    //    {{1, '0'}, 3}, {{1, '1'}, 3},  
//    //    {{3, '0'}, 3}, {{3, '1'}, 3}
//    //};
//
//    //int startState = 0;  // A 作为起始状态
//    //set<int> finalStates = { 1, 2, 3 };  // C 作为终态
//
//    //// 创建 DFA 实例
//    //DFA dfa(dfaStates, alphabet, transitions, startState, finalStates);
//
//    //// 运行最小化算法
//    //MinimizedDFA minimizedDFA = MinimizedDFA::minimize(dfa);
//
//    //// 输出最小化后的结果
//    //cout << "最小化后的状态集: ";
//    //for (int state : minimizedDFA.minStates) {
//    //    cout << state << " ";
//    //}
//    //cout << endl;
//
//    //cout << "最小化后的起始状态: " << minimizedDFA.startState << endl;
//
//    //cout << "最小化后的终止状态: ";
//    //for (int finalState : minimizedDFA.finalStates) {
//    //    cout << finalState << " ";
//    //}
//    //cout << endl;
//
//    //cout << "最小化后的状态转换表:" << endl;
//    //for (const auto& transition : minimizedDFA.minTransitions) {
//    //    cout << "从状态 " << transition.first.first << " 输入 " << transition.first.second
//    //        << " 转移到状态 " << transition.second << endl;
//    //}
//
//    testDeterminize();
//    return 0;
//}

NFA readNFA(string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "无法打开文件：" << filename << endl;
        exit(1);
    }

    cerr << "成功打开文件" << endl;
    /*读取状态集*/
    // 从文件中读取一行，存入输入字符串流
    string line;
    getline(file, line);
    // 将文件中读取到的一行，存入输入字符串流中
    istringstream stateStream(line);
    set<int>states;
    int state;
    // 从输入字符串流中读取状态
    while (stateStream>>state)
    {
        states.insert(state);
    }

    /*读取输入字母表*/
    set<char>alphabet;
    // 从文件中读取一行，存入输入字符串流
    getline(file, line);
    // 将文件中读取到的一行，存入输入字符串流中
    istringstream alphabetStream(line);
    char alpha;
    // 从输入字符串流中读取输入字母
    while (alphabetStream >> alpha)
    {
        alphabet.insert(alpha);
    }

    /*读取状态转换函数*/
    getline(file, line);
    istringstream nStream(line); int n; nStream >> n;
    map<pair<int, char>, set<int>> transitions;
    for (int i = 0; i < n; i++)
    {
        getline(file, line);    istringstream tranStream(line);
        int fromState; char symbol; set<int>toStateSet; int toState;
        int flag=0;   // 判定是否读取到-1
        /*处理-1的情况*/
        tranStream >> fromState >> symbol;
        if (symbol == '-') { flag = -1; tranStream >> symbol; }
        while (tranStream >> toState)
        {
            toStateSet.insert(toState);
        }
        if (flag == -1)
        {
            char c = -1;
            transitions[{fromState, c}] = toStateSet;  // 处理epsilon
        }
        else
        {
            transitions[{fromState, symbol}] = toStateSet;
        }
    }

    /*读取初始状态*/
    getline(file, line);
    istringstream startStream(line);
    int startState; startStream >> startState;

    /*读取终止状态集*/
    getline(file, line);
    istringstream finalStream(line);
    set<int>finalStates; int finalState;
    while (finalStream >> finalState)
    {
        finalStates.insert(finalState);
    }
    return NFA(states, alphabet, transitions, startState, finalStates);
}

//DFA readDFA(string& filename)
//{
//    ifstream file(filename);
//    if (!file.is_open())
//    {
//        cerr << "无法打开文件：" << filename << endl;
//        exit(1);
//    }
//
//    cerr << "成功打开文件" << endl;
//    /*读取状态集*/
//    // 从文件中读取一行，存入输入字符串流
//    string line;
//    getline(file, line);
//    // 将文件中读取到的一行，存入输入字符串流中
//    istringstream stateStream(line);
//    set<int>states;
//    int state;
//    // 从输入字符串流中读取状态
//    while (stateStream >> state)
//    {
//        states.insert(state);
//    }
//
//    /*读取输入字母表*/
//    set<char>alphabet;
//    // 从文件中读取一行，存入输入字符串流
//    getline(file, line);
//    // 将文件中读取到的一行，存入输入字符串流中
//    istringstream alphabetStream(line);
//    char alpha;
//    // 从输入字符串流中读取输入字母
//    while (alphabetStream >> alpha)
//    {
//        alphabet.insert(alpha);
//    }
//
//    /*读取状态转换函数*/
//    getline(file, line);
//    istringstream nStream(line); int n; nStream >> n;
//    map<pair<int, char>, int> transitions;
//    for (int i = 0; i < n; i++)
//    {
//        getline(file, line);    istringstream tranStream(line);
//        int fromState; char symbol; int toStateSet; int toState;
//        int flag = 0;   // 判定是否读取到-1
//        /*处理-1的情况*/
//        tranStream >> fromState >> symbol;
//        if (symbol == '-') { flag = -1; tranStream >> symbol; }
//        tranStream >> toState;
//        if (flag == -1)
//        {
//            char c = -1;
//            transitions[{fromState, c}] = toStateSet;  // 处理epsilon
//        }
//        else
//        {
//            transitions[{fromState, symbol}] = toStateSet;
//        }
//    }
//
//    /*读取初始状态*/
//    getline(file, line);
//    istringstream startStream(line);
//    int startState; startStream >> startState;
//
//    /*读取终止状态集*/
//    getline(file, line);
//    istringstream finalStream(line);
//    set<int>finalStates; int finalState;
//    while (finalStream >> finalState)
//    {
//        finalStates.insert(finalState);
//    }
//    return DFA(states, alphabet, transitions, startState, finalStates);
//}

//void printNFA(NFA& nfa)
//{
//    cout << "状态集:";
//    for (const int state : nfa.states)
//    {
//        cout << state << " ";
//    }
//    cout << endl << "输入字母表:";
//    for (const char alpha : nfa.alphabet)
//    {
//        cout << alpha << " ";
//    }
//    cout << endl << "状态转换函数:";
//    for (const auto& transition : nfa.transitions) 
//    {
//        cout << "  (" << transition.first.first << ", " << transition.first.second << ") -> ";
//        for (const int state : transition.second)
//        {
//            cout << state << " ";
//        }
//        cout << endl;
//    }
//    cout << endl << "初始状态:" << nfa.startState << endl;
//    cout << "终止状态集:";
//    for (const int state : nfa.finalStates)
//    {
//        cout << state << " ";
//    }
//}

// 合并多个NFA
NFA mergeMultipleNFA(const vector<NFA>& nfas)
{
    if (nfas.size() == 0)
    {
        cout << "没有NFA可以合并！" << endl;
        exit(1);
    }
    NFA mergedNFA = nfas[0];
    if (nfas.size() == 1)
    {
        return mergedNFA;
    }
    for (NFA nfa : nfas)
    {
        mergedNFA = NFA::merge(mergedNFA, nfa);
    }
    return mergedNFA;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr << "缺少文件名参数" << endl;
        return 1;
    }
    vector<NFA> nfas;
    // 使用输入文件流创建一个文件
    for (int i = 1; i < argc; i++)
    {
        string s = argv[i];
        NFA nfa = readNFA(s);
        nfas.push_back(nfa);
    }

    // 合并所有NFA
    cout << "合并后的NFA:" << endl;
    NFA mergedNFA = mergeMultipleNFA(nfas);

    NFA::printNFA(mergedNFA);

    // 确定化
    DFA dfa = DFA::determinize(mergedNFA);
    cout <<endl<< "确定化后的DFA:" << endl;
    DFA::printDFA(dfa);

    // 最小化
    MinimizedDFA minimizedDFA = MinimizedDFA::minimize(dfa);
    cout <<endl<< "最小化后的DFA:" << endl;
    MinimizedDFA::printMinimizedDFA(minimizedDFA);
}
