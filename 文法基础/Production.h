#pragma once
#ifndef PRODUCTION_H_
#define PRODUCTION_H_

#include<string>
#include<vector>
using namespace std;

// ����ʽ
class Production
{
public:
	string left;	// ����ʽ��
	vector<string> right;	// ����ʽ�Ҳ���ÿ��string�洢һ����ѡʽ

	Production(const string& left, const vector<string>& right);

	void print() const;

	// ����<�ţ��Ա���Production��map�ļ���mapҪ�����������Ҫ����<������
	bool operator<(const Production& other) const {
		if (left != other.left) {
			return left < other.left;
		}
		return right < other.right;
	}
};

#endif // !PRODUCTION_H_
