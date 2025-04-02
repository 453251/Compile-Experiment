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
//    cout << "����ʽת���ɵ�NFA:" << endl;
//    NFA::printNFA(nfa);
//
//    DFA dfa = DFA::determinize(nfa);
//    cout << "ȷ�������DFA:" << endl;
//    DFA::printDFA(dfa);
//
//    MinimizedDFA minimizedDFA = MinimizedDFA::minimize(dfa);
//    cout << "��С�����DFA:" << endl;
//    MinimizedDFA::printMinimizedDFA(minimizedDFA);
//
//    return 0;
//}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cerr << "ȱ������ʽ����" << endl;
        return 1;
    }

    string input = argv[1];
    cout << "���������ʽ:" << endl;
    cout << input << endl;
    NFA nfa = RegexToNFA::convert(input);
    cout << "����ʽת���ɵ�NFA:" << endl;
    NFA::printNFA(nfa);

    DFA dfa = DFA::determinize(nfa);
    cout <<endl<< "ȷ�������DFA:" << endl;
    DFA::printDFA(dfa);

    MinimizedDFA minimizedDFA = MinimizedDFA::minimize(dfa);
    cout <<endl<< "��С�����DFA:" << endl;
    MinimizedDFA::printMinimizedDFA(minimizedDFA);

    return 0;
}