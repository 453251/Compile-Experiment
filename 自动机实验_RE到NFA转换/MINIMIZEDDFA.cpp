#include "MINIMIZEDDFA.h"
#include<algorithm>
#include<iterator>
#include<queue>
#include<vector>
#include<iostream>
using namespace std;

// ���캯��
MinimizedDFA::MinimizedDFA(const set<int>& minStates, const set<char>& alphabet, const map<pair<int, char>, int>& minTransitions, int startState, const set<int>& finalStates)
    :minStates(minStates), alphabet(alphabet), minTransitions(minTransitions), startState(startState), finalStates(finalStates) {}

// ��DFA��С��
MinimizedDFA MinimizedDFA::minimize(const DFA& dfa) {
    // Step 1: ��ʼ���֣���Ϊ��̬�ͷ���̬
    set<set<int>> partitions;  // ���������ڽ�״̬������Ϊ��ͬ����
    set<int> nonFinalStates;

    // ����̬ = ����״̬ - ��̬
    set_difference(dfa.dfaStates.begin(), dfa.dfaStates.end(),
        dfa.finalStates.begin(), dfa.finalStates.end(),
        inserter(nonFinalStates, nonFinalStates.begin()));
    // ��ʼ���֣�����̬������Ϊһ���飬��̬������Ϊ��һ����
    if (!nonFinalStates.empty()) {
        partitions.insert(nonFinalStates);
    }
    if (!dfa.finalStates.empty()) {
        partitions.insert(dfa.finalStates);
    }

    /*�Ի��ֽ���ϸ��*/
    /*
        ����˼·Ϊ������ÿ�����֣��Ի����е�ÿ��״̬��ά����״̬��һ����ǣ��ñ�Ǽ�Ϊ<���ţ�����ķ�����>�ĶԵļ���
        ������һ��ӳ����ά��<��ǣ�����ñ�ǵļ���>�����ʾ����ͬһ���Ŷ��ܵ���ͬ��������״̬Ӧ�÷���ͬһ��������
        ����������µĻ��֣���ӳ���е���һ���ϵĳ��ȶ���С�ڵ�ǰ���ֵĳ���
    */
    bool refined = true;
    while (refined)
    {
        refined = false;
        set<set<int>> newPartitions;

        for (const auto& partition : partitions)
        {
            // �Ի��ּ����е�ÿ�����ֽ��в���
            // ��ÿ���Ӽ���������Ž��н�һ������
            // ����һ��ӳ�䣬�������<�����ż��ϣ�״̬����>
            // �������״̬����������ĸ����ܵ���ķ�������ͬ����ô�⼸��״̬�ȼۣ�Ӧ�÷���ͬһ��������
            map<set<pair<char, int>>, set<int>> partitionMap;

            for (int state : partition)
            {
                // ����ÿ��״̬�����������״̬�ı�ǣ�����״̬����������ĸ����ܵ���ķ����ż���
                // ���������״̬��ת�Ƶġ���ǡ�
                // ʹ��һ����<char,int>��ά��ǩ������֤�Ƕ�Ӧ��
                set<pair<char, int>> signature;
                for (char symbol : dfa.alphabet)
                {
                    if (dfa.transitions.count({ state, symbol }))
                    {

                        // ���Ҹ�״̬�� symbol ת��Ŀ�꣬���ҵ�Ŀ��״̬���ڵķ���
                        int nextState = dfa.transitions.at({ state, symbol });
                        for (const auto& p : partitions) {
                            if (p.count(nextState))
                            {
                                // ����������Ƿ�����
                                signature.insert({ symbol, distance(partitions.begin(), partitions.find(p)) });
                                break;
                            }
                        }
                    }
                    else
                    {
                        // ���û�ж�Ӧ��ת�ƣ����һ�������־
                        signature.insert({ symbol, -1 });
                    }
                }
                // ����״̬���뵽�ܴﵽ����������ϵ�״̬����
                // ����״̬��ת�Ƶġ���ǡ�����
                partitionMap[signature].insert(state);
            }

            // ���»���
            for (const auto& entry : partitionMap)
            {
                newPartitions.insert(entry.second);
                // ���ĳ��״̬���ϵĳ���С��ԭ�����ֵĳ��ȣ�˵���������µĻ���
                if (entry.second.size() < partition.size())
                {
                    refined = true;  // ������µĻ��֣�˵����Ҫ����ϸ��
                }
            }
        }
        partitions = newPartitions;
    }

    // Step 3: ������С�� DFA ��״̬ӳ��
    map<set<int>, int> stateMapping;
    int stateCounter = 0;
    set<int> minStates;
    map<pair<int, char>, int> minTransitions;
    int minStartState;
    set<int> minFinalStates;

    // Ϊÿ����������һ����״̬
    for (const auto& partition : partitions) {
        stateMapping[partition] = stateCounter++;
        minStates.insert(stateMapping[partition]);

        // ��������а��� DFA �ĳ�ʼ״̬����������С�� DFA �ĳ�ʼ״̬
        if (partition.count(dfa.startState)) {
            minStartState = stateMapping[partition];
        }

        // ��������а����κ�һ�� DFA ����ֹ״̬����������С�� DFA ����ֹ״̬
        for (int state : partition) {
            if (dfa.finalStates.count(state)) {
                minFinalStates.insert(stateMapping[partition]);
                break;
            }
        }
    }

    // Step 4: ������С�� DFA ��״̬ת������
    for (const auto& partition : partitions) {
        int fromState = stateMapping[partition];
        for (int state : partition) {
            for (char symbol : dfa.alphabet) {
                if (dfa.transitions.count({ state, symbol })) {
                    int nextState = dfa.transitions.at({ state, symbol });
                    for (const auto& p : partitions) {
                        if (p.count(nextState)) {
                            minTransitions[{fromState, symbol}] = stateMapping[p];
                            break;
                        }
                    }
                }
            }
        }
    }

    // ������С����� DFA
    return MinimizedDFA(minStates, dfa.alphabet, minTransitions, minStartState, minFinalStates);
}

void MinimizedDFA::printMinimizedDFA(const MinimizedDFA& dfa)
{
    cout << "״̬��:";
    for (const int state : dfa.minStates)
    {
        cout << state << " ";
    }
    cout << endl << "������ĸ��:";
    for (const char alpha : dfa.alphabet)
    {
        cout << alpha << " ";
    }
    cout << endl << "״̬ת������:" << endl;
    for (const auto& transition : dfa.minTransitions)
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
