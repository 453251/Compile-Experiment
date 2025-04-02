#include"FirstFollowSelectCalculator.h"

#include<iostream>
#include<sstream>
#include<algorithm>
#include<iomanip>
using namespace std;

FirstFollowSelectCalculator::FirstFollowSelectCalculator(Grammar& grammar):grammar(grammar)
{
	// ��ʼ��First����Follow����Select��
	for (const auto& nonTerminal : grammar.getNonTerminals())
	{
		firstSets[nonTerminal] = set<string>();
		followSets[nonTerminal] = set<string>();
	}
	//for (const auto& production : grammar.getProductions())
	//{
	//	ostringstream oss;
	//	oss << production.left << " -> ";
	//	for (const auto& symbol : production.right)
	//		oss << symbol << " ";
	//	string productionKey = oss.str();
	//	selectSets[productionKey] = set<string>();
	//}
	for (const auto& production : grammar.getProductions())
	{
		selectSets[production] = set<string>();
	}

	// startSymbol��Follow������'#'
	followSets[grammar.getStartSymbol()].insert("#");
}

void FirstFollowSelectCalculator::calculateFirstSets()
{
	bool changed = true;

	// ʹ�õ���������First����ֱ�����ϲ��ٱ仯
	while (changed)
	{
		changed = false;
		for (const auto& production : grammar.getProductions())
		{
			string left = production.left;
			vector<string> right = production.right;

			/*�Բ���ʽ�󲿵�First���и���*/
			size_t initialSize = firstSets[left].size();
			bool addEpsilon = true; // �Ƿ���Ҫ���epsilon���󲿵�First����
			for (const auto& symbol : right)
			{
				// ������ַ����ս��
				vector<string> terminals = grammar.getTerminals();
				if (find(terminals.begin(),terminals.end(),symbol)!=terminals.end())
				{
					// �տ�ʼ�����������ַ���������ս����ֱ�Ӽ��뵽first��
					firstSets[left].insert(symbol);
					addEpsilon = false;	// ������ҵ����ս������ô˵���϶�û�Ц�
					break;
				}
				// ����ʽ��ֻ������ĵ���epsilon����
				else if (symbol == "��")
				{
					addEpsilon = true;
					firstSets[left].insert(symbol);
					break;
				}
				// ������ַ��Ƿ��ս��������First���г�����ļ��뵽�󲿵�First����
				else
				{
					set<string> firstSetOfSymbol = firstSets[symbol];
					bool epsilonFound = false; // �жϸ÷��ŵ�First���Ƿ���ڿմ�
					for (const auto& s : firstSetOfSymbol)
					{
						if (s != "��")
							firstSets[left].insert(s);
						else
							epsilonFound = true;	// ���ڿմ�
					}
					// ��������ڿմ���ֱ���˳����������ղ���Ҫ���epsilon���󲿵�First����������ڣ������
					if (!epsilonFound)
					{
						addEpsilon = false;
						break;
					}
				}
			}
			if (addEpsilon)
				firstSets[left].insert("��");
			// ��鼯���Ƿ����仯
			if (firstSets[left].size() > initialSize)
				changed = true;
		}
	}

}

void FirstFollowSelectCalculator::calculateFollowSets()
{
	bool changed = true;

	// ʹ�õ���������Follow����ֱ�����ϲ��ٱ仯
	while (changed)
	{
		changed = false;
		for (const auto& production : grammar.getProductions())
		{
			string left = production.left;
			vector<string> right = production.right;
			for (size_t i = 0; i < right.size(); i++)
			{
				// ����ҵ���һ�����ս����ע��ȥ���ŵ������
				vector<string> nonTerminals = grammar.getNonTerminals();
				if (find(nonTerminals.begin(), nonTerminals.end(), right[i]) != nonTerminals.end() && right[i]!="��")
				{
					string currentNonTerminal = right[i];
					size_t inialSize = followSets[currentNonTerminal].size();

					// �����Ҳ��ĺ�������
					bool epsilonFound = true;	// �Ƿ��ҵ�epsilon
					bool addEpsilon = true;		// epsilon�Ƿ�����First����
					/*���������First��*/
					for (size_t j = i + 1; j < right.size(); j++)
					{
						epsilonFound = false;
						string nextSymbol = right[j];
						// �ս��
						vector<string> terminals = grammar.getTerminals();
						if (find(terminals.begin(), terminals.end(), nextSymbol) != terminals.end())
						{
							followSets[currentNonTerminal].insert(nextSymbol);
							addEpsilon = false;
							break;
						}
						// ���ս��
						else
						{
							set<string> firstSetOfSymbol = firstSets[nextSymbol];
							for (const auto& s : firstSetOfSymbol)
							{
								if (s != "��")
									followSets[currentNonTerminal].insert(s);
								else
									epsilonFound = true;
							}
							if (!epsilonFound)
							{
								addEpsilon = false;
								break;
							}
						}
					}
					// ���epsilon����First���ϣ����󲿵�Follow�����뵽��ǰ���ս����Follow����
					if (addEpsilon)
					{
						followSets[currentNonTerminal].insert(followSets[left].begin(), followSets[left].end());
					}
					if (followSets[currentNonTerminal].size() > inialSize)
					{
						changed = true;
					}
				}
			}
		}
	}
}

void FirstFollowSelectCalculator::calculateSelectSets()
{
	for (const auto& production : grammar.getProductions())
	{
		string left = production.left;
		vector<string>right = production.right;

		// ����select��
		set<string> selectSet;

		/*�����Ҳ����ţ�������First��*/
		bool epsilonFound = true;
		bool addEpsilon = true;
		for (const auto& symbol : right)
		{
			epsilonFound = false;
			vector<string> terminals = grammar.getTerminals();
			if (terminals.end() != find(terminals.begin(), terminals.end(), symbol)) 
			{
				// ������ս����ֱ�Ӽ���Select��
				selectSet.insert(symbol);
				addEpsilon = false;
				break;
			}
			else if (symbol == "��")
			{
				addEpsilon = true;
				break;
			}
			else
			{
				set<string> firstSetOfSymbol = firstSets[symbol];
				for (const auto& s : firstSetOfSymbol)
				{
					if (s != "��")
					{
						selectSet.insert(s);
					}
					else
					{
						epsilonFound = true;
					}
				}
				if (!epsilonFound)
				{
					addEpsilon = false;
					break;
				}
			}
		}
		if (addEpsilon)
		{
			selectSet.insert(followSets[left].begin(), followSets[left].end());
		}

		// ����select��
		selectSets[production] = selectSet;
	}
}

void FirstFollowSelectCalculator::printFirstSets()
{
	cout << "[First Set]\n";
	for (const auto& entry : firstSets) {
		// ʹ��iomanip�������������ʽ
		cout << "  " << setw(16)<<left <<entry.first << " : ";
		for (const auto& symbol : entry.second) {
			cout << symbol << " ";
		}
		cout << "\n";
	}
}

void FirstFollowSelectCalculator::printFollowSets()
{
	cout << "[Follow Set]\n";
	for (const auto& entry : followSets) {
		// ʹ��iomanip�������������ʽ
		cout << "  " << setw(16)<<left <<entry.first << " : ";
		for (const auto& symbol : entry.second) {
			cout << symbol << " ";
		}
		cout << "\n";
	}
}

// vector�е��ַ����Էָ���ƴ��
string join(const vector<std::string>& vec, const string& separator)
{
	ostringstream oss;
	for (size_t i = 0; i < vec.size(); ++i) {
		if (i != 0)
			oss << separator;
		oss << vec[i];
	}
	return oss.str();
}

void FirstFollowSelectCalculator::printSelectSets()
{
	cout << "[Select Set]\n";
	int j = 0;
	for (const auto& entry : selectSets) {
		cout << "  " << j++ << ":";
		// ʹ��iomanip�������������ʽ
		cout << setw(16)<<left<<(entry.first.left + " -> " + join(entry.first.right, " "));
		cout << ": ";
		for (const auto& symbol : entry.second) {
			cout << symbol << " ";
		}
		cout << "\n";
	}
}


