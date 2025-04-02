#include"Regex.h"
#include"RegexToNFA.h"
#include"NFA.h"
#include"DFA.h"
#include"MINIMIZEDDFA.h"
#include<iostream>
using namespace std;

//int main()
//{
//	string s; getline(cin, s);
//    NFA nfa = RegexToNFA::convert(s);
//    cout << "正规式转化成的NFA:" << endl;
//    NFA::printNFA(nfa);
//
//    DFA dfa = DFA::determinize(nfa);
//    cout << "确定化后的DFA:" << endl;
//    DFA::printDFA(dfa);
//
//    MinimizedDFA minimizedDFA = MinimizedDFA::minimize(dfa);
//    cout << "最小化后的DFA:" << endl;
//    MinimizedDFA::printMinimizedDFA(minimizedDFA);
//
//    return 0;
//}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr << "缺少正规式参数" << endl;
        return 1;
    }

    string input = argv[1];
    cout << "输入的正规式:" << endl;
    cout << input << endl;
    NFA nfa = RegexToNFA::convert(input);
    cout << "正规式转化成的NFA:" << endl;
    NFA::printNFA(nfa);

    DFA dfa = DFA::determinize(nfa);
    cout <<endl<< "确定化后的DFA:" << endl;
    DFA::printDFA(dfa);

    MinimizedDFA minimizedDFA = MinimizedDFA::minimize(dfa);
    cout <<endl<< "最小化后的DFA:" << endl;
    MinimizedDFA::printMinimizedDFA(minimizedDFA);

    return 0;
}