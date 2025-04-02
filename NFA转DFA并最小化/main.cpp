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

//// ���Ժ���
//void testDeterminize() {
//    // ����NFA
//    set<int> states = { 1, 2, 3, 4 };
//    set<char> alphabet = { '0', '1' };
//
//    map<pair<int, char>, set<int>> transitions;
//    // ״̬ת����0����0��1��1����0��2��1����1��3��2����1��3
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
//    // ����NFA����
//    NFA nfa(states, alphabet, transitions, startState, finalStates);
//
//    // NFAȷ����ΪDFA
//    DFA dfa = DFA::determinize(nfa);
//
//    // ��ӡDFA�Ľ��
//    cout << "DFA ״̬��: { ";
//    for (int state : dfa.dfaStates) {
//        cout << state << " ";
//    }
//    cout << "}" << endl;
//
//    cout << "���ű�: { ";
//    for (char symbol : dfa.alphabet) {
//        cout << symbol << " ";
//    }
//    cout << "}" << endl;
//
//    cout << "״̬ת��:" << endl;
//    for (const auto& transition : dfa.transitions) {
//        cout << "  (" << transition.first.first << ", " << transition.first.second << ") -> " << transition.second << endl;
//    }
//
//    cout << "��ʼ״̬: " << dfa.startState << endl;
//
//    cout << "��ֹ״̬��: { ";
//    for (int finalState : dfa.finalStates) {
//        cout << finalState << " ";
//    }
//    cout << "}" << endl;
//}
//
//int main() {
//    // ����һ��ʾ�� DFA
//    // DFA ״̬ת�Ʊ� (A = 0, B = 1, C = 2)
//    // ״̬ת�Ʊ�
//    // | ״̬ | ���� 0 | ���� 1 |
//    // |------|--------|--------|
//    // |   A  |    B   |    C   |
//    // |   B  |    B   |    A   |
//    // |   C  |    C   |    A   |
//
//    //set<int> dfaStates = { 0, 1, 2, 3, 4, 5};  // ״̬�� A, B, C
//    //set<char> alphabet = { '0', '1' };  // ������ż�
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
//    //int startState = 0;  // A ��Ϊ��ʼ״̬
//    //set<int> finalStates = { 0, 1 };  // C ��Ϊ��̬
//
//    //set<int> dfaStates = { 0, 1, 2, 3 };  // ״̬�� A, B, C
//    //set<char> alphabet = { '0', '1' };  // ������ż�
//
//    //map<pair<int, char>, int> transitions = {
//    //    {{0, '0'}, 2}, {{0, '1'}, 1},  
//    //    {{1, '0'}, 3}, {{1, '1'}, 3},  
//    //    {{3, '0'}, 3}, {{3, '1'}, 3}
//    //};
//
//    //int startState = 0;  // A ��Ϊ��ʼ״̬
//    //set<int> finalStates = { 1, 2, 3 };  // C ��Ϊ��̬
//
//    //// ���� DFA ʵ��
//    //DFA dfa(dfaStates, alphabet, transitions, startState, finalStates);
//
//    //// ������С���㷨
//    //MinimizedDFA minimizedDFA = MinimizedDFA::minimize(dfa);
//
//    //// �����С����Ľ��
//    //cout << "��С�����״̬��: ";
//    //for (int state : minimizedDFA.minStates) {
//    //    cout << state << " ";
//    //}
//    //cout << endl;
//
//    //cout << "��С�������ʼ״̬: " << minimizedDFA.startState << endl;
//
//    //cout << "��С�������ֹ״̬: ";
//    //for (int finalState : minimizedDFA.finalStates) {
//    //    cout << finalState << " ";
//    //}
//    //cout << endl;
//
//    //cout << "��С�����״̬ת����:" << endl;
//    //for (const auto& transition : minimizedDFA.minTransitions) {
//    //    cout << "��״̬ " << transition.first.first << " ���� " << transition.first.second
//    //        << " ת�Ƶ�״̬ " << transition.second << endl;
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
        cerr << "�޷����ļ���" << filename << endl;
        exit(1);
    }

    cerr << "�ɹ����ļ�" << endl;
    /*��ȡ״̬��*/
    // ���ļ��ж�ȡһ�У����������ַ�����
    string line;
    getline(file, line);
    // ���ļ��ж�ȡ����һ�У����������ַ�������
    istringstream stateStream(line);
    set<int>states;
    int state;
    // �������ַ������ж�ȡ״̬
    while (stateStream>>state)
    {
        states.insert(state);
    }

    /*��ȡ������ĸ��*/
    set<char>alphabet;
    // ���ļ��ж�ȡһ�У����������ַ�����
    getline(file, line);
    // ���ļ��ж�ȡ����һ�У����������ַ�������
    istringstream alphabetStream(line);
    char alpha;
    // �������ַ������ж�ȡ������ĸ
    while (alphabetStream >> alpha)
    {
        alphabet.insert(alpha);
    }

    /*��ȡ״̬ת������*/
    getline(file, line);
    istringstream nStream(line); int n; nStream >> n;
    map<pair<int, char>, set<int>> transitions;
    for (int i = 0; i < n; i++)
    {
        getline(file, line);    istringstream tranStream(line);
        int fromState; char symbol; set<int>toStateSet; int toState;
        int flag=0;   // �ж��Ƿ��ȡ��-1
        /*����-1�����*/
        tranStream >> fromState >> symbol;
        if (symbol == '-') { flag = -1; tranStream >> symbol; }
        while (tranStream >> toState)
        {
            toStateSet.insert(toState);
        }
        if (flag == -1)
        {
            char c = -1;
            transitions[{fromState, c}] = toStateSet;  // ����epsilon
        }
        else
        {
            transitions[{fromState, symbol}] = toStateSet;
        }
    }

    /*��ȡ��ʼ״̬*/
    getline(file, line);
    istringstream startStream(line);
    int startState; startStream >> startState;

    /*��ȡ��ֹ״̬��*/
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
//        cerr << "�޷����ļ���" << filename << endl;
//        exit(1);
//    }
//
//    cerr << "�ɹ����ļ�" << endl;
//    /*��ȡ״̬��*/
//    // ���ļ��ж�ȡһ�У����������ַ�����
//    string line;
//    getline(file, line);
//    // ���ļ��ж�ȡ����һ�У����������ַ�������
//    istringstream stateStream(line);
//    set<int>states;
//    int state;
//    // �������ַ������ж�ȡ״̬
//    while (stateStream >> state)
//    {
//        states.insert(state);
//    }
//
//    /*��ȡ������ĸ��*/
//    set<char>alphabet;
//    // ���ļ��ж�ȡһ�У����������ַ�����
//    getline(file, line);
//    // ���ļ��ж�ȡ����һ�У����������ַ�������
//    istringstream alphabetStream(line);
//    char alpha;
//    // �������ַ������ж�ȡ������ĸ
//    while (alphabetStream >> alpha)
//    {
//        alphabet.insert(alpha);
//    }
//
//    /*��ȡ״̬ת������*/
//    getline(file, line);
//    istringstream nStream(line); int n; nStream >> n;
//    map<pair<int, char>, int> transitions;
//    for (int i = 0; i < n; i++)
//    {
//        getline(file, line);    istringstream tranStream(line);
//        int fromState; char symbol; int toStateSet; int toState;
//        int flag = 0;   // �ж��Ƿ��ȡ��-1
//        /*����-1�����*/
//        tranStream >> fromState >> symbol;
//        if (symbol == '-') { flag = -1; tranStream >> symbol; }
//        tranStream >> toState;
//        if (flag == -1)
//        {
//            char c = -1;
//            transitions[{fromState, c}] = toStateSet;  // ����epsilon
//        }
//        else
//        {
//            transitions[{fromState, symbol}] = toStateSet;
//        }
//    }
//
//    /*��ȡ��ʼ״̬*/
//    getline(file, line);
//    istringstream startStream(line);
//    int startState; startStream >> startState;
//
//    /*��ȡ��ֹ״̬��*/
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
//    cout << "״̬��:";
//    for (const int state : nfa.states)
//    {
//        cout << state << " ";
//    }
//    cout << endl << "������ĸ��:";
//    for (const char alpha : nfa.alphabet)
//    {
//        cout << alpha << " ";
//    }
//    cout << endl << "״̬ת������:";
//    for (const auto& transition : nfa.transitions) 
//    {
//        cout << "  (" << transition.first.first << ", " << transition.first.second << ") -> ";
//        for (const int state : transition.second)
//        {
//            cout << state << " ";
//        }
//        cout << endl;
//    }
//    cout << endl << "��ʼ״̬:" << nfa.startState << endl;
//    cout << "��ֹ״̬��:";
//    for (const int state : nfa.finalStates)
//    {
//        cout << state << " ";
//    }
//}

// �ϲ����NFA
NFA mergeMultipleNFA(const vector<NFA>& nfas)
{
    if (nfas.size() == 0)
    {
        cout << "û��NFA���Ժϲ���" << endl;
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
        cerr << "ȱ���ļ�������" << endl;
        return 1;
    }
    vector<NFA> nfas;
    // ʹ�������ļ�������һ���ļ�
    for (int i = 1; i < argc; i++)
    {
        string s = argv[i];
        NFA nfa = readNFA(s);
        nfas.push_back(nfa);
    }

    // �ϲ�����NFA
    cout << "�ϲ����NFA:" << endl;
    NFA mergedNFA = mergeMultipleNFA(nfas);

    NFA::printNFA(mergedNFA);

    // ȷ����
    DFA dfa = DFA::determinize(mergedNFA);
    cout <<endl<< "ȷ�������DFA:" << endl;
    DFA::printDFA(dfa);

    // ��С��
    MinimizedDFA minimizedDFA = MinimizedDFA::minimize(dfa);
    cout <<endl<< "��С�����DFA:" << endl;
    MinimizedDFA::printMinimizedDFA(minimizedDFA);
}
