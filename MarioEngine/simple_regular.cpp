#include "simple_regular.h"

SimpleRegular::SimpleRegular(string regex)
	:reg(regex){}

void SimpleRegular::assign(string regex) {
	reg.assign(regex);
}

void SimpleRegular::search(string text) {
	resultText.clear();
	str = text;
	while (regex_search(str, match, reg)) {
		for (auto obj : match) 
			resultText.push_back(obj);
		str = match.suffix().str();
	}
}

list<string> SimpleRegular::result() {
	return resultText;
}
