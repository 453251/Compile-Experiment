#pragma once
#ifndef REGEXTONFA_H_
#define REGEXTONFA_H_

#include"NFA.h"
#include"Regex.h"

class RegexToNFA
{
public:
	static NFA convert(const string& regex);	// ���沨�����ʽת��ΪNFA
};


#endif // !REGEXTONFA_H_
