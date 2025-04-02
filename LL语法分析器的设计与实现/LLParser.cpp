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
	/*��#�Ϳ�ʼ�����뵽����ջ��*/
	parseStack.push("#");
	parseStack.push(table.getGrammar().getStartSymbol());
}

void LLParser::parse(const string& filename)
{
	ifstream file(filename);
	if (!file)
	{
		cerr << "�޷��򿪷��Ŵ��ļ�: " << filename << endl;
		return;
	}
	string token;
	/*��ȡ���Ŵ�*/
	while (file >> token)
		inputTokens.push_back(token);
	inputTokens.push_back("#");	// �����ֹ����#
	size_t index = 0;	// ָ�����봮��ǰ��λ��
	int step = 1;	// �������
	ostringstream oss;
	for (const auto& inputToken : inputTokens)
		oss << inputToken;
	cout << oss.str() + "��������:" << endl;
	cout << "��ʼ��:#��ջ��" << table.getGrammar().getStartSymbol() + "��ջ��" << endl;

	while (!parseStack.empty())
	{
		string top = parseStack.top();
		string currentToken = inputTokens[index];
		// ����Ƿ�ƥ��ɹ������ջ���͵�ǰ���Ŷ���#����ƥ��ɹ���
		if (top == "#" && currentToken == "#") {
			cout << setw(2) << step++ << ": ��ջX=#�� ����c=#��ƥ�䣬�ɹ���" << endl;
			parseStack.pop();
			break;
		}

		cout << setw(2) << step++ << ": ��ջX=" << top << "�� ����c=" << currentToken;


		/*���ҽ������ս�����ſ���ƥ�䣬�������������ʵ�൱�ھ����ս���������*/
		if (top == currentToken)
		{
			// ƥ��
			cout << "��ƥ�䣬����ָ����ƣ�" << endl;
			parseStack.pop();
			index++;
		}
		else if(table.getTable().count(top) && table.getTable().at(top).count(currentToken))
		{
			// ���
			Production production = table.getTable().at(top).at(currentToken);
			cout << "�����M[X,c]=" << production.toString_withoutSpace();
			cout << "������ʽ�Ҳ�������ջ��" << endl;

			/*������ʽ�Ҳ�������ջ*/
			parseStack.pop();
			for (auto it = production.right.rbegin(); it != production.right.rend(); it++)
			{
				/*�ղ���ʽ����*/
				if (*it != "��")
					parseStack.push(*it);
			}
		}
		else
		{
			cerr << "������: ����ƥ�� " << top << " �� " << currentToken << endl;
			cerr << "����ʧ�ܣ�������Ŵ�δ��ȫƥ�䡣" << endl;
			return;
		}
	}

	if (!parseStack.empty())
	{
		cout << "����ʧ�ܣ�������Ŵ�δ��ȫƥ�䡣" << endl;
	}
}

