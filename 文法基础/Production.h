#pragma once
#ifndef PRODUCTION_H_
#define PRODUCTION_H_

#include<string>
#include<vector>
using namespace std;

// 产生式
class Production
{
public:
	string left;	// 产生式左部
	vector<string> right;	// 产生式右部，每个string存储一个候选式

	Production(const string& left, const vector<string>& right);

	void print() const;

	// 重载<号，以便让Production作map的键（map要求有序，因此需要重载<来排序）
	bool operator<(const Production& other) const {
		if (left != other.left) {
			return left < other.left;
		}
		return right < other.right;
	}
};

#endif // !PRODUCTION_H_
