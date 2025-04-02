#include"DFA.h"
#include<queue>
#include<stack>
#include<iostream>
using namespace std;


// 构造函数，直接接收扁平化后的参数
DFA::DFA(const set<int>& dfaStates, const set<char>& alphabet, const map<pair<int, char>, int>& transitions, int startState, const set<int>& finalStates)
	: dfaStates(dfaStates), alphabet(alphabet), transitions(transitions), startState(startState), finalStates(finalStates) {}

// NFA确定化为DFA
DFA DFA::determinize(const NFA& nfa)
{
	// 检查NFA是否已经确定化
	bool alreadyDeterminized = true;
	for (const auto& transition : nfa.transitions) {
		/*如果经过某个字符到达了多个状态，则不是确定的*/
		if (transition.second.size() > 1) {
			alreadyDeterminized = false;
			break;
		}
		/*如果包含了epsilon，则不是确定的*/
		if (transition.first.second == -1)
		{
			alreadyDeterminized = false;
			break;
		}
	}

	if (alreadyDeterminized) {
		// 如果NFA已经确定化，直接将NFA转换为DFA
		cout << endl << "NFA已确定化，直接转换为DFA" << endl;
		set<int> dfaStates = nfa.states;
		set<char> alphabet = nfa.alphabet;
		map<pair<int, char>, int> transitions;

		for (const auto& transition : nfa.transitions) {
			int fromState = transition.first.first;
			char symbol = transition.first.second;
			int toState = *transition.second.begin();  // 由于是确定化，只有一个目标状态
			transitions[{fromState, symbol}] = toState;
		}

		int startState = nfa.startState;
		set<int> finalStates = nfa.finalStates;

		return DFA(dfaStates, alphabet, transitions, startState, finalStates);
	}

	/*利用子集法，将NFA确定化为DFA，主要思路为，先找到初始状态的epsilon闭包，作为新的初始状态，然后从初始状态开始
	  利用状态转换函数和已有的字母表进行搜索，将搜索到的新状态加入到状态集中，同时更新状态转换函数和终止状态集
	*/
	// 状态集合
	set<set<int>> dfaStates;

	// 输入字母表，与NFA相同
	set<char> alphabet = nfa.alphabet;

	// 状态转换函数
	map<pair<set<int>, char>, set<int>> dfaTransitions;

	// 初始状态，即输入nfa的初始状态的epsilon闭包
	set<int> startState = epsilonClosure({ nfa.startState }, nfa.transitions);
	dfaStates.insert(startState);	// 相应地，将初始状态加入状态集合中
	// 维护一个用于状态转换函数搜索的队列
	queue<set<int>> stateQueue;
	stateQueue.push(startState);	// 初始状态加入搜索队列中

	// 终止状态集
	set<set<int>> finalStates;

	// 利用状态转换函数，进行搜索
	while (!stateQueue.empty())
	{
		set<int> currentState = stateQueue.front(); stateQueue.pop();

		// 获取一个symbol后，就可利用状态和输入字母进行一次搜索
		for (char symbol : alphabet)
		{
			// 进行一次搜索
			set<int> nextState = move(currentState, symbol, nfa.transitions);

			// 当且仅当搜索到的状态集合不为空时，才求闭包，并且将结果加入到状态集合中，并更新状态转换函数
			// 如果是空集则直接弃用了，不然就会多出来状态
			if (!nextState.empty())
			{
				// 别忘了对搜索后的结果求闭包
				nextState = epsilonClosure(nextState, nfa.transitions);

				// 然后将结果加入到状态集合中，同时更新状态转换函数
				if (!dfaStates.count(nextState))
				{
					dfaStates.insert(nextState);
					stateQueue.push(nextState);
				}
				dfaTransitions[{currentState, symbol}] = nextState;
			}

			// 对于包含原终止状态的状态（因为现在一个状态就是一个状态集），被视为终止状态，加入到终止状态集中
			for (int state : currentState)
			{
				if (nfa.finalStates.count(state))
				{
					finalStates.insert(currentState);
					break;	// 加一次就够了，不重复加
				}
			}
		}
	}

	// ---- 开始进行扁平化操作 ----
	map<set<int>, int> stateMapping;
	int stateCounter = 0;

	// 生成扁平化后的状态映射表
	set<int> flattenedStates;
	for (const auto& stateSet : dfaStates) {
		stateMapping[stateSet] = stateCounter++;
		flattenedStates.insert(stateMapping[stateSet]);
	}

	// 扁平化转换表
	map<pair<int, char>, int> flattenedTransitions;
	for (const auto& [key, value] : dfaTransitions) {
		int fromState = stateMapping[key.first];
		int toState = stateMapping[value];
		flattenedTransitions[{fromState, key.second}] = toState;
	}

	// 扁平化终态集
	set<int> flattenedFinalStates;
	for (const auto& stateSet : finalStates) {
		flattenedFinalStates.insert(stateMapping[stateSet]);
	}

	// 扁平化初始状态
	int flattenedStartState = stateMapping[startState];

	// 返回新的扁平化后的 DFA
	return DFA(flattenedStates, alphabet, flattenedTransitions, flattenedStartState, flattenedFinalStates);
}

// 寻找状态的闭包
set<int> DFA::epsilonClosure(const set<int>& states, const map<pair<int, char>, set<int>>& transitions)
{
	/*寻找状态的epsilon闭包，这里不止寻找初始状态，初始状态就是单一的一个状态，而其他状态有可能是一个状态集*/
	set<int> closure = states;
	// stateStack用来寻找闭包
	stack<int> stateStack;

	// 先加入已有状态
	for (int state : states)
	{
		stateStack.push(state);
	}

	// 对于stateStack中的状态，利用状态转换函数进行搜索
	while (!stateStack.empty())
	{
		// 取出栈顶状态，用来搜索
		int state = stateStack.top(); stateStack.pop();

		// 如果存在（状态，空串）的键，则加入值
		if (transitions.count(make_pair(state, -1)))
		{
			// 将可到达的状态加入闭包和stack中
			for (int nextState : transitions.at(make_pair(state, -1)))
			{
				if (!closure.count(nextState))
				{
					// 如果不存在，则加入，避免重复加入
					closure.insert(nextState);
					stateStack.push(nextState);
				}
			}
		}
	}
	return closure;
}

// 利用状态转换函数做一次搜索，同时输入还需要一个输入字母，否则不好找键
set<int> DFA::move(const set<int>& states, char symbol, const map<pair<int, char>, set<int>>& transitions)
{
	/*利用状态转换函数做一次搜索*/
	set<int> nextState;
	for (int state : states)
	{
		if (transitions.count(make_pair(state, symbol)))
		{
			nextState.insert(transitions.at(make_pair(state, symbol)).begin(), transitions.at(make_pair(state, symbol)).end());
		}
	}
	return nextState;
}

void DFA::printDFA(const DFA& dfa)
{
	cout << "状态集:";
	for (const int state : dfa.dfaStates)
	{
		cout << state << " ";
	}
	cout << endl << "输入字母表:";
	for (const char alpha : dfa.alphabet)
	{
		cout << alpha << " ";
	}
	cout << endl << "状态转换函数:" << endl;
	for (const auto& transition : dfa.transitions)
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



