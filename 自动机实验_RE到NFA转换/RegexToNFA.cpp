#include"RegexToNFA.h"
#include<stack>
#include<iostream>
using namespace std;

NFA RegexToNFA::convert(const string& input)
{
	/*输入正规式，转化为逆波兰表达式进行处理*/
	Regex regex(input);
	vector<char>tokens = regex.toRPN();	// 逆波兰表达式
	/*打印转化的逆波兰表达式*/
	cout << "正规式对应的逆波兰表达式:" << endl;
	for (const char& token : tokens)
	{
		cout << token;
	}
	cout << endl;

	stack<NFA> nfaStack;
	for (const char& token : tokens)
	{
		if (isalnum(token))
		{
			nfaStack.push(NFA::basicNFA(token));
		}
		else if (token == '*')
		{
			NFA nfa = nfaStack.top(); nfaStack.pop();
			nfaStack.push(NFA::kleeneStar(nfa));
		}
		else if (token == '.')
		{
			/*注意读出来的顺序*/
			/*
				从左往右匹配正规式，压入栈中
				所以当前栈顶元素应该是在右边，也就是nfa2
				连接的时候应该放在右边，也就是nfa1的终态应该指向它的初态
			*/
			/*顺序错了答案就错了*/
			NFA nfa2 = nfaStack.top(); nfaStack.pop();
			NFA nfa1 = nfaStack.top(); nfaStack.pop();
			nfaStack.push(NFA::concatNFA(nfa1, nfa2));
		}
		else if (token == '|')
		{
			/*注意读出来的顺序*/
			/*
				从左往右匹配正规式，压入栈中
				所以当前栈顶元素应该是在右边，也就是nfa2
				连接的时候应该放在右边，也就是nfa1的终态应该指向它的初态
			*/
			/*顺序错了答案就错了*/
			NFA nfa2 = nfaStack.top(); nfaStack.pop();
			NFA nfa1 = nfaStack.top(); nfaStack.pop();
			nfaStack.push(NFA::unionNFA(nfa1, nfa2));
		}
	}

	return nfaStack.top();
}
