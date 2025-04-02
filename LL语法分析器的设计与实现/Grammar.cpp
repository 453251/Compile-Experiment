#include"Grammar.h"

#include<fstream>
#include<iostream>
#include<sstream>
using namespace std;

// 从文件中加载Grammar对象
void Grammar::loadFromFile(const string& filename)
{
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    int numNonTerminals, numTerminals, numProductions;

    // 读取非终结符
    file >> numNonTerminals;
    nonTerminals.resize(numNonTerminals);
    for (int i = 0; i < numNonTerminals; i++)
    {
        file >> nonTerminals[i];
    }

    // 读取终结符
    file >> numTerminals;
    terminals.resize(numTerminals);
    for (int i = 0; i < numTerminals; i++)
    {
        file >> terminals[i];
    }

    // 读取产生式
    file >> numProductions;
    file.get(); // 读取剩余的换行符
    productions.reserve(numProductions);
    for (int i = 0; i < numProductions; i++)
    {
        string line;
        getline(file, line);    // 读取一整行，再做处理
        istringstream iss(line);    // 将其转化为输入字符流
        string left, arrow, symbol;
        /*
            由于产生式右部的各个符号之间使用了空格隔开，且各个符号可能会出现类似T'的情况，因此需要用string来表示各个符号，用vector<string>来表示右部
            vector<string>存储了产生式右部的所有符号
        */
        vector<string>right;

        iss >> left >> arrow;   // 读取左部和箭头
        while (iss >> symbol)
        {
            if (symbol == "蔚") symbol = "ε";    // 单独处理一下编码集导致的ε变成蔚的问题
            right.push_back(symbol);
        }

        Production production(left, right);
        productions.push_back(production);
    }

    // 读取开始符
    file >> startSymbol;
}

// 打印Grammar对象
void Grammar::printGrammar()
{
    cout << "CFG=(VN,VT,P,S)" << endl;
    cout << "VN: ";
    // 打印非终结符
    for (const auto& nonTerminal : nonTerminals)
        cout << nonTerminal << " ";

    // 打印终结符
    cout << "\nVT: ";
    for (const auto& terminal : terminals)
        cout << terminal << " ";

    // 打印产生式
    cout << "\nProduction:\n";
    for (size_t i = 0; i < productions.size(); i++)
    {
        cout << "  " << i << ": ";
        productions[i].print();
    }

    // 打印开始符
    cout << "StartSymbol: " << startSymbol << endl;
}

// getter
vector<string> Grammar::getNonTerminals()
{
    return nonTerminals;
}

vector<string> Grammar::getTerminals()
{
    return terminals;
}

vector<Production> Grammar::getProductions()
{
    return productions;
}

string Grammar::getStartSymbol()
{
    return startSymbol;
}
