#include"Regex.h"
#include<stack>
#include<unordered_map>
#include<cctype>

Regex::Regex(const string& regex):regex(regex){}

// 插入连接符
string Regex::addConcatOperator(const string& regex)
{
	string result = "";
	for (int i = 0; i < regex.size(); i++)
	{
		result += regex[i];
		if (i + 1 < regex.size())
		{
			char current = regex[i], next = regex[i + 1];
			// 如果当前是输入字母（数字或字母）或*或)且下一个是输入字母或(，则加入连接符
			if ((isalnum(current) || current == '*' || current == ')') && (isalnum(next) || next == '('))
			{
				result += '.';	// 将.号作为连接符
			}
		}
	}
	return result;
}

// 将中缀表达式转换成逆波兰表达式
vector<char> Regex::toRPN()
{
	string processedRegex = addConcatOperator(regex);

	// 使用map存放算符的优先级
	unordered_map<char, int>precedence =
	{ {'|', 1}, {'.', 2}, {'*', 3} };

	/*需要保证优先级最高的运算符先加入表达式中，故栈顶元素的优先级总是高的，如果当前运算符优先级低于栈顶运算符，则栈顶运算符直接加入到表达式中*/
	stack<char>operators;
	vector<char>output;

	for (const char& token : processedRegex)
	{
		// 是输入字母，则直接加入到表达式中
		if (isalnum(token))
		{
			output.push_back(token);
		}
		// 如果是左括号，直接入栈
		else if (token == '(')
		{
			operators.push(token);
		}
		// 如果是右括号，则将括号中的所有运算符（即栈中括号之间的所有运算符）按优先级加入到表达式中
		// 栈顶运算符的优先级总是高的
		else if (token == ')')
		{
			while (!operators.empty() && operators.top() != '(')
			{
				output.push_back(operators.top());
				operators.pop();
			}
			// 左括号出栈
			operators.pop();
		}
		// 如果是运算符，则比较和栈顶的优先级，栈顶高，则栈顶运算符入表达式，当前运算符高，则入栈等待比较，栈顶是括号则必须入栈等待比较
		else
		{
			// 栈顶运算符的优先级总是高的，如果遇到当前运算符优先级比他低，则直接加入到表达式中，先做计算
			while (!operators.empty() && operators.top() != '(' && precedence[operators.top()] >= precedence[token])
			{
				// 栈顶运算符的优先级更高，则先做运算，将其加入到逆波兰表达式中，并将其出栈
				output.push_back(operators.top());
				operators.pop();
			}
			// 当前是更高优先级的运算符或当前栈顶是括号或栈为空，直接入栈，等待比较
			operators.push(token);
		}
	}

	/*将剩余的操作符全部加入表达式中*/
	while (!operators.empty())
	{
		output.push_back(operators.top());
		operators.pop();
	}

	return output;
}