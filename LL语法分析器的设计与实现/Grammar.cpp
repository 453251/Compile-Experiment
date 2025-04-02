#include"Grammar.h"

#include<fstream>
#include<iostream>
#include<sstream>
using namespace std;

// ���ļ��м���Grammar����
void Grammar::loadFromFile(const string& filename)
{
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    int numNonTerminals, numTerminals, numProductions;

    // ��ȡ���ս��
    file >> numNonTerminals;
    nonTerminals.resize(numNonTerminals);
    for (int i = 0; i < numNonTerminals; i++)
    {
        file >> nonTerminals[i];
    }

    // ��ȡ�ս��
    file >> numTerminals;
    terminals.resize(numTerminals);
    for (int i = 0; i < numTerminals; i++)
    {
        file >> terminals[i];
    }

    // ��ȡ����ʽ
    file >> numProductions;
    file.get(); // ��ȡʣ��Ļ��з�
    productions.reserve(numProductions);
    for (int i = 0; i < numProductions; i++)
    {
        string line;
        getline(file, line);    // ��ȡһ���У���������
        istringstream iss(line);    // ����ת��Ϊ�����ַ���
        string left, arrow, symbol;
        /*
            ���ڲ���ʽ�Ҳ��ĸ�������֮��ʹ���˿ո�������Ҹ������ſ��ܻ��������T'������������Ҫ��string����ʾ�������ţ���vector<string>����ʾ�Ҳ�
            vector<string>�洢�˲���ʽ�Ҳ������з���
        */
        vector<string>right;

        iss >> left >> arrow;   // ��ȡ�󲿺ͼ�ͷ
        while (iss >> symbol)
        {
            if (symbol == "ε") symbol = "��";    // ��������һ�±��뼯���µĦű��ε������
            right.push_back(symbol);
        }

        Production production(left, right);
        productions.push_back(production);
    }

    // ��ȡ��ʼ��
    file >> startSymbol;
}

// ��ӡGrammar����
void Grammar::printGrammar()
{
    cout << "CFG=(VN,VT,P,S)" << endl;
    cout << "VN: ";
    // ��ӡ���ս��
    for (const auto& nonTerminal : nonTerminals)
        cout << nonTerminal << " ";

    // ��ӡ�ս��
    cout << "\nVT: ";
    for (const auto& terminal : terminals)
        cout << terminal << " ";

    // ��ӡ����ʽ
    cout << "\nProduction:\n";
    for (size_t i = 0; i < productions.size(); i++)
    {
        cout << "  " << i << ": ";
        productions[i].print();
    }

    // ��ӡ��ʼ��
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
