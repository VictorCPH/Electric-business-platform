#include "BankCardText.h"
#include <iostream>
#include <fstream>
#include <sstream>

void BankCardText::readFile()
{
	ifstream in("BankCardText.txt");
	if (!in) {
		cout << "BankCardText.txt 打开失败!" << endl;
		exit(-1);
	}

	string textline;
	while (getline(in, textline)) {
		istringstream line(textline);
		string number;
		line >> number;

		bankCardMap[number] = textline;
	}
}

bool BankCardText::isExist(const string& number)
{
	return bankCardMap.count(number) == 0 ? false : true;
}

bool BankCardText::isPassword(const string & number, const string & password)
{
	if (!isExist(number))
		return false;
	else {
		istringstream line(bankCardMap[number]);

		string num, pass;
		line >> num >> pass;

		if (pass == password)
			return true;
		else
			return false;
	}
}

void BankCardText::updateInfo(const string& number, const string& bankCard)
{
	bankCardMap[number] = bankCard;
}

string BankCardText::getBankCard(const string & number)
{
	return bankCardMap[number];
}


void BankCardText::saveFile()
{
	ofstream out("BankCardText.txt");
	if (!out) {
		cout << "BankCardText.txt 打开失败!" << endl;
		exit(-1);
	}

	for (map<string, string>::const_iterator it = bankCardMap.begin(); 
			it != bankCardMap.end(); ++it)
		out << it->second << endl;
}
