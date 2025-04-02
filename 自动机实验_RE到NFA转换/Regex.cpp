#include"Regex.h"
#include<stack>
#include<unordered_map>
#include<cctype>

Regex::Regex(const string& regex):regex(regex){}

// �������ӷ�
string Regex::addConcatOperator(const string& regex)
{
	string result = "";
	for (int i = 0; i < regex.size(); i++)
	{
		result += regex[i];
		if (i + 1 < regex.size())
		{
			char current = regex[i], next = regex[i + 1];
			// �����ǰ��������ĸ�����ֻ���ĸ����*��)����һ����������ĸ��(����������ӷ�
			if ((isalnum(current) || current == '*' || current == ')') && (isalnum(next) || next == '('))
			{
				result += '.';	// ��.����Ϊ���ӷ�
			}
		}
	}
	return result;
}

// ����׺���ʽת�����沨�����ʽ
vector<char> Regex::toRPN()
{
	string processedRegex = addConcatOperator(regex);

	// ʹ��map�����������ȼ�
	unordered_map<char, int>precedence =
	{ {'|', 1}, {'.', 2}, {'*', 3} };

	/*��Ҫ��֤���ȼ���ߵ�������ȼ�����ʽ�У���ջ��Ԫ�ص����ȼ����Ǹߵģ������ǰ��������ȼ�����ջ�����������ջ�������ֱ�Ӽ��뵽���ʽ��*/
	stack<char>operators;
	vector<char>output;

	for (const char& token : processedRegex)
	{
		// ��������ĸ����ֱ�Ӽ��뵽���ʽ��
		if (isalnum(token))
		{
			output.push_back(token);
		}
		// ����������ţ�ֱ����ջ
		else if (token == '(')
		{
			operators.push(token);
		}
		// ����������ţ��������е��������������ջ������֮�������������������ȼ����뵽���ʽ��
		// ջ������������ȼ����Ǹߵ�
		else if (token == ')')
		{
			while (!operators.empty() && operators.top() != '(')
			{
				output.push_back(operators.top());
				operators.pop();
			}
			// �����ų�ջ
			operators.pop();
		}
		// ��������������ȽϺ�ջ�������ȼ���ջ���ߣ���ջ�����������ʽ����ǰ������ߣ�����ջ�ȴ��Ƚϣ�ջ���������������ջ�ȴ��Ƚ�
		else
		{
			// ջ������������ȼ����Ǹߵģ����������ǰ��������ȼ������ͣ���ֱ�Ӽ��뵽���ʽ�У���������
			while (!operators.empty() && operators.top() != '(' && precedence[operators.top()] >= precedence[token])
			{
				// ջ������������ȼ����ߣ����������㣬������뵽�沨�����ʽ�У��������ջ
				output.push_back(operators.top());
				operators.pop();
			}
			// ��ǰ�Ǹ������ȼ����������ǰջ�������Ż�ջΪ�գ�ֱ����ջ���ȴ��Ƚ�
			operators.push(token);
		}
	}

	/*��ʣ��Ĳ�����ȫ��������ʽ��*/
	while (!operators.empty())
	{
		output.push_back(operators.top());
		operators.pop();
	}

	return output;
}