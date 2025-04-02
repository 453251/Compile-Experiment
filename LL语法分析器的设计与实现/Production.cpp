#include"Production.h"
#include<sstream>
#include<iostream>
using namespace std;

Production::Production()
{

}

Production::Production(const string& left, const vector<string>& right) :left(left), right(right) {}

void Production::print() const
{
	cout << left << " -> ";
	for (size_t i = 0; i < right.size(); i++)
	{
		cout << right[i];
		if (i != (right.size() - 1))
			cout << " ";
	}
	cout << endl;
}

string Production::toString()
{
	ostringstream oss;
	oss << left << " -> ";
	for (size_t i = 0; i < right.size(); i++)
	{
		if (i != 0)
			oss << " ";
		oss << right[i];
	}
	return oss.str();
}

string Production::toString_withoutSpace()
{
	ostringstream oss;
	oss << left << "->";
	for (size_t i = 0; i < right.size(); i++)
	{
		oss << right[i];
	}
	return oss.str();
}
