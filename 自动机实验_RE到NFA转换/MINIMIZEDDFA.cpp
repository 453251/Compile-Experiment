#include "MINIMIZEDDFA.h"
#include<algorithm>
#include<iterator>
#include<queue>
#include<vector>
#include<iostream>
using namespace std;

// 构造函数
MinimizedDFA::MinimizedDFA(const set<int>& minStates, const set<char>& alphabet, const map<pair<int, char>, int>& minTransitions, int startState, const set<int>& finalStates)
    :minStates(minStates), alphabet(alphabet), minTransitions(minTransitions), startState(startState), finalStates(finalStates) {}

// 将DFA最小化
MinimizedDFA MinimizedDFA::minimize(const DFA& dfa) {
    // Step 1: 初始划分，分为终态和非终态
    set<set<int>> partitions;  // 分区，用于将状态集划分为不同的组
    set<int> nonFinalStates;

    // 非终态 = 所有状态 - 终态
    set_difference(dfa.dfaStates.begin(), dfa.dfaStates.end(),
        dfa.finalStates.begin(), dfa.finalStates.end(),
        inserter(nonFinalStates, nonFinalStates.begin()));
    // 初始划分：非终态集合作为一个组，终态集合作为另一个组
    if (!nonFinalStates.empty()) {
        partitions.insert(nonFinalStates);
    }
    if (!dfa.finalStates.empty()) {
        partitions.insert(dfa.finalStates);
    }

    /*对划分进行细化*/
    /*
        具体思路为，遍历每个划分，对划分中的每个状态，维护该状态的一个标记，该标记即为<符号，到达的分区号>的对的集合
        再利用一个映射来维护<标记，满足该标记的集合>，这表示经过同一符号都能到达同个分区的状态应该放在同一个划分中
        如果产生了新的划分，则映射中的任一集合的长度都会小于当前划分的长度
    */
    bool refined = true;
    while (refined)
    {
        refined = false;
        set<set<int>> newPartitions;

        for (const auto& partition : partitions)
        {
            // 对划分集合中的每个划分进行操作
            // 将每个子集按输入符号进行进一步划分
            // 构建一个映射，里面包含<分区号集合，状态集合>
            // 如果几个状态经过输入字母表后能到达的分区号相同，那么这几个状态等价，应该放在同一个集合里
            map<set<pair<char, int>>, set<int>> partitionMap;

            for (int state : partition)
            {
                // 对于每个状态，构建代表该状态的标记，即该状态经过输入字母表后能到达的分区号集合
                // 构建代表该状态的转移的“标记”
                // 使用一个对<char,int>来维护签名，保证是对应的
                set<pair<char, int>> signature;
                for (char symbol : dfa.alphabet)
                {
                    if (dfa.transitions.count({ state, symbol }))
                    {

                        // 查找该状态的 symbol 转移目标，并找到目标状态所在的分区
                        int nextState = dfa.transitions.at({ state, symbol });
                        for (const auto& p : partitions) {
                            if (p.count(nextState))
                            {
                                // 距离用来标记分区号
                                signature.insert({ symbol, distance(partitions.begin(), partitions.find(p)) });
                                break;
                            }
                        }
                    }
                    else
                    {
                        // 如果没有对应的转移，添加一个特殊标志
                        signature.insert({ symbol, -1 });
                    }
                }
                // 将该状态加入到能达到这个分区集合的状态集中
                // 将该状态按转移的“标记”归类
                partitionMap[signature].insert(state);
            }

            // 更新划分
            for (const auto& entry : partitionMap)
            {
                newPartitions.insert(entry.second);
                // 如果某个状态集合的长度小于原本划分的长度，说明产生了新的划分
                if (entry.second.size() < partition.size())
                {
                    refined = true;  // 如果有新的划分，说明需要继续细化
                }
            }
        }
        partitions = newPartitions;
    }

    // Step 3: 构建最小化 DFA 的状态映射
    map<set<int>, int> stateMapping;
    int stateCounter = 0;
    set<int> minStates;
    map<pair<int, char>, int> minTransitions;
    int minStartState;
    set<int> minFinalStates;

    // 为每个分区分配一个新状态
    for (const auto& partition : partitions) {
        stateMapping[partition] = stateCounter++;
        minStates.insert(stateMapping[partition]);

        // 如果分区中包含 DFA 的初始状态，则这是最小化 DFA 的初始状态
        if (partition.count(dfa.startState)) {
            minStartState = stateMapping[partition];
        }

        // 如果分区中包含任何一个 DFA 的终止状态，则这是最小化 DFA 的终止状态
        for (int state : partition) {
            if (dfa.finalStates.count(state)) {
                minFinalStates.insert(stateMapping[partition]);
                break;
            }
        }
    }

    // Step 4: 构建最小化 DFA 的状态转换函数
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

    // 返回最小化后的 DFA
    return MinimizedDFA(minStates, dfa.alphabet, minTransitions, minStartState, minFinalStates);
}

void MinimizedDFA::printMinimizedDFA(const MinimizedDFA& dfa)
{
    cout << "状态集:";
    for (const int state : dfa.minStates)
    {
        cout << state << " ";
    }
    cout << endl << "输入字母表:";
    for (const char alpha : dfa.alphabet)
    {
        cout << alpha << " ";
    }
    cout << endl << "状态转换函数:" << endl;
    for (const auto& transition : dfa.minTransitions)
    {
        cout << "  (" << transition.first.first << ", " << transition.first.second << ") -> " << transition.second;
        cout << endl;
    }
    cout << endl << "初始状态:" << dfa.startState << endl;
    cout << "终止状态集:";
    for (const int state : dfa.finalStates)
    {
        cout << state << " ";
    }
    cout << endl;
}
