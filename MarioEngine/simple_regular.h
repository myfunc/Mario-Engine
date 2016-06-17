#pragma once
#include <iostream>
#include <regex>
#include <string>
#include <list>
using namespace std;

class SimpleRegular final {
private:
	string str;
	regex reg;
	smatch match;
	list<string> resultText;
public:
	SimpleRegular() = default;
	SimpleRegular(string regex);
	void assign(string regex);
	void search(string text);
	list<string> result();
};