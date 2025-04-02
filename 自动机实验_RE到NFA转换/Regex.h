#pragma once
#ifndef REGEX_H_
#define REGEX_H_

#include<vector>
#include<string>
using namespace std;

class Regex
{
public:
	Regex(const string& regex);
	vector<char> toRPN();	// ת��Ϊ�沨�����ʽ
private:
	string regex;
	string addConcatOperator(const string& regex);	// �������ӷ�
};

#endif // !REGEX_H_
