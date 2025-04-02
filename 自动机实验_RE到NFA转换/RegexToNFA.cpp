#include"RegexToNFA.h"
#include<stack>
#include<iostream>
using namespace std;

NFA RegexToNFA::convert(const string& input)
{
	/*��������ʽ��ת��Ϊ�沨�����ʽ���д���*/
	Regex regex(input);
	vector<char>tokens = regex.toRPN();	// �沨�����ʽ
	/*��ӡת�����沨�����ʽ*/
	cout << "����ʽ��Ӧ���沨�����ʽ:" << endl;
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
			/*ע���������˳��*/
			/*
				��������ƥ������ʽ��ѹ��ջ��
				���Ե�ǰջ��Ԫ��Ӧ�������ұߣ�Ҳ����nfa2
				���ӵ�ʱ��Ӧ�÷����ұߣ�Ҳ����nfa1����̬Ӧ��ָ�����ĳ�̬
			*/
			/*˳����˴𰸾ʹ���*/
			NFA nfa2 = nfaStack.top(); nfaStack.pop();
			NFA nfa1 = nfaStack.top(); nfaStack.pop();
			nfaStack.push(NFA::concatNFA(nfa1, nfa2));
		}
		else if (token == '|')
		{
			/*ע���������˳��*/
			/*
				��������ƥ������ʽ��ѹ��ջ��
				���Ե�ǰջ��Ԫ��Ӧ�������ұߣ�Ҳ����nfa2
				���ӵ�ʱ��Ӧ�÷����ұߣ�Ҳ����nfa1����̬Ӧ��ָ�����ĳ�̬
			*/
			/*˳����˴𰸾ʹ���*/
			NFA nfa2 = nfaStack.top(); nfaStack.pop();
			NFA nfa1 = nfaStack.top(); nfaStack.pop();
			nfaStack.push(NFA::unionNFA(nfa1, nfa2));
		}
	}

	return nfaStack.top();
}
