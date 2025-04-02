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
	vector<char> toRPN();	// 转化为逆波兰表达式
private:
	string regex;
	string addConcatOperator(const string& regex);	// 插入连接符
};

#endif // !REGEX_H_
