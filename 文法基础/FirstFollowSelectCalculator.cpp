#include"FirstFollowSelectCalculator.h"

#include<iostream>
#include<sstream>
#include<algorithm>
#include<iomanip>
using namespace std;

FirstFollowSelectCalculator::FirstFollowSelectCalculator(Grammar& grammar):grammar(grammar)
{
	// 初始化First集，Follow集，Select集
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

	// startSymbol的Follow集加上'#'
	followSets[grammar.getStartSymbol()].insert("#");
}

void FirstFollowSelectCalculator::calculateFirstSets()
{
	bool changed = true;

	// 使用迭代法计算First集，直到集合不再变化
	while (changed)
	{
		changed = false;
		for (const auto& production : grammar.getProductions())
		{
			string left = production.left;
			vector<string> right = production.right;

			/*对产生式左部的First进行更新*/
			size_t initialSize = firstSets[left].size();
			bool addEpsilon = true; // 是否需要添加epsilon到左部的First集中
			for (const auto& symbol : right)
			{
				// 如果首字符是终结符
				vector<string> terminals = grammar.getTerminals();
				if (find(terminals.begin(),terminals.end(),symbol)!=terminals.end())
				{
					// 刚开始遍历的是首字符，如果是终结符，直接加入到first集
					firstSets[left].insert(symbol);
					addEpsilon = false;	// 如果是找到了终结符，那么说明肯定没有ε
					break;
				}
				// 产生式中只有特殊的单个epsilon处理
				else if (symbol == "ε")
				{
					addEpsilon = true;
					firstSets[left].insert(symbol);
					break;
				}
				// 如果首字符是非终结符，则将其First集中除ε外的加入到左部的First集中
				else
				{
					set<string> firstSetOfSymbol = firstSets[symbol];
					bool epsilonFound = false; // 判断该符号的First集是否存在空串
					for (const auto& s : firstSetOfSymbol)
					{
						if (s != "ε")
							firstSets[left].insert(s);
						else
							epsilonFound = true;	// 存在空串
					}
					// 如果不存在空串，直接退出，并且最终不需要添加epsilon到左部的First集，如果存在，则继续
					if (!epsilonFound)
					{
						addEpsilon = false;
						break;
					}
				}
			}
			if (addEpsilon)
				firstSets[left].insert("ε");
			// 检查集合是否发生变化
			if (firstSets[left].size() > initialSize)
				changed = true;
		}
	}

}

void FirstFollowSelectCalculator::calculateFollowSets()
{
	bool changed = true;

	// 使用迭代法计算Follow集，直到集合不再变化
	while (changed)
	{
		changed = false;
		for (const auto& production : grammar.getProductions())
		{
			string left = production.left;
			vector<string> right = production.right;
			for (size_t i = 0; i < right.size(); i++)
			{
				// 如果找到了一个非终结符（注意去除ε的情况）
				vector<string> nonTerminals = grammar.getNonTerminals();
				if (find(nonTerminals.begin(), nonTerminals.end(), right[i]) != nonTerminals.end() && right[i]!="ε")
				{
					string currentNonTerminal = right[i];
					size_t inialSize = followSets[currentNonTerminal].size();

					// 考虑右部的后续符号
					bool epsilonFound = true;	// 是否找到epsilon
					bool addEpsilon = true;		// epsilon是否属于First集合
					/*求后续串的First集*/
					for (size_t j = i + 1; j < right.size(); j++)
					{
						epsilonFound = false;
						string nextSymbol = right[j];
						// 终结符
						vector<string> terminals = grammar.getTerminals();
						if (find(terminals.begin(), terminals.end(), nextSymbol) != terminals.end())
						{
							followSets[currentNonTerminal].insert(nextSymbol);
							addEpsilon = false;
							break;
						}
						// 非终结符
						else
						{
							set<string> firstSetOfSymbol = firstSets[nextSymbol];
							for (const auto& s : firstSetOfSymbol)
							{
								if (s != "ε")
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
					// 如果epsilon属于First集合，则将左部的Follow集加入到当前非终结符的Follow集中
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

		// 计算select集
		set<string> selectSet;

		/*遍历右部符号，计算其First集*/
		bool epsilonFound = true;
		bool addEpsilon = true;
		for (const auto& symbol : right)
		{
			epsilonFound = false;
			vector<string> terminals = grammar.getTerminals();
			if (terminals.end() != find(terminals.begin(), terminals.end(), symbol)) 
			{
				// 如果是终结符，直接加入Select集
				selectSet.insert(symbol);
				addEpsilon = false;
				break;
			}
			else if (symbol == "ε")
			{
				addEpsilon = true;
				break;
			}
			else
			{
				set<string> firstSetOfSymbol = firstSets[symbol];
				for (const auto& s : firstSetOfSymbol)
				{
					if (s != "ε")
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

		// 更新select集
		selectSets[production] = selectSet;
	}
}

void FirstFollowSelectCalculator::printFirstSets()
{
	cout << "[First Set]\n";
	for (const auto& entry : firstSets) {
		// 使用iomanip控制整齐输出格式
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
		// 使用iomanip控制整齐输出格式
		cout << "  " << setw(16)<<left <<entry.first << " : ";
		for (const auto& symbol : entry.second) {
			cout << symbol << " ";
		}
		cout << "\n";
	}
}

// vector中的字符串以分隔符拼接
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
		// 使用iomanip控制整齐输出格式
		cout << setw(16)<<left<<(entry.first.left + " -> " + join(entry.first.right, " "));
		cout << ": ";
		for (const auto& symbol : entry.second) {
			cout << symbol << " ";
		}
		cout << "\n";
	}
}


