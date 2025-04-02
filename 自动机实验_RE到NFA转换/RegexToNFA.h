#pragma once
#ifndef REGEXTONFA_H_
#define REGEXTONFA_H_

#include"NFA.h"
#include"Regex.h"

class RegexToNFA
{
public:
	static NFA convert(const string& regex);	// 将逆波兰表达式转化为NFA
};


#endif // !REGEXTONFA_H_
