#include "LLParser.h"
#include<stack>
#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include<iomanip>
using namespace std;

LLParser::LLParser(LLParserTable& table): table(table)
{
	/*将#和开始符加入到分析栈中*/
	parseStack.push("#");
	parseStack.push(table.getGrammar().getStartSymbol());
}

void LLParser::parse(const string& filename)
{
	ifstream file(filename);
	if (!file)
	{
		cerr << "无法打开符号串文件: " << filename << endl;
		return;
	}
	string token;
	/*读取符号串*/
	while (file >> token)
		inputTokens.push_back(token);
	inputTokens.push_back("#");	// 添加终止符号#
	size_t index = 0;	// 指向输入串当前的位置
	int step = 1;	// 步骤计数
	ostringstream oss;
	for (const auto& inputToken : inputTokens)
		oss << inputToken;
	cout << oss.str() + "分析过程:" << endl;
	cout << "初始化:#入栈，" << table.getGrammar().getStartSymbol() + "入栈；" << endl;

	while (!parseStack.empty())
	{
		string top = parseStack.top();
		string currentToken = inputTokens[index];
		// 检查是否匹配成功，如果栈顶和当前符号都是#，则匹配成功了
		if (top == "#" && currentToken == "#") {
			cout << setw(2) << step++ << ": 出栈X=#， 输入c=#，匹配，成功。" << endl;
			parseStack.pop();
			break;
		}

		cout << setw(2) << step++ << ": 出栈X=" << top << "， 输入c=" << currentToken;


		/*当且仅当是终结符，才可能匹配，所以这个条件其实相当于就是终结符的情况了*/
		if (top == currentToken)
		{
			// 匹配
			cout << "，匹配，输入指针后移；" << endl;
			parseStack.pop();
			index++;
		}
		else if(table.getTable().count(top) && table.getTable().at(top).count(currentToken))
		{
			// 查表
			Production production = table.getTable().at(top).at(currentToken);
			cout << "，查表，M[X,c]=" << production.toString_withoutSpace();
			cout << "，产生式右部逆序入栈；" << endl;

			/*将产生式右部逆序入栈*/
			parseStack.pop();
			for (auto it = production.right.rbegin(); it != production.right.rend(); it++)
			{
				/*空产生式除外*/
				if (*it != "ε")
					parseStack.push(*it);
			}
		}
		else
		{
			cerr << "，错误: 不能匹配 " << top << " 和 " << currentToken << endl;
			cerr << "解析失败，输入符号串未完全匹配。" << endl;
			return;
		}
	}

	if (!parseStack.empty())
	{
		cout << "解析失败，输入符号串未完全匹配。" << endl;
	}
}

