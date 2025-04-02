#include"Production.h"
#include<iostream>
using namespace std;

Production::Production(const string& left, const vector<string>& right):left(left),right(right){}

void Production::print() const
{
	cout << left << " -> ";
	for (size_t i=0; i<right.size();i++)
	{
		cout << right[i];
		if (i != (right.size() - 1))
			cout << " ";
	}
	cout << endl;
}
