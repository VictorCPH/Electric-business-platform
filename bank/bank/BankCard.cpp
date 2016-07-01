#include "BankCard.h"
#include <sstream>
#include <iostream>
#include <fstream>

BankCard::BankCard(const string& bankCard)
{
	istringstream line(bankCard);

	line >> number >> password >> userID >> bankName >> deposit;

	string fileName = number + "的操作记录.txt";//操作记录初始化
	ofstream out(fileName.c_str(), ios::app);

	if (!out) {
		cout << fileName + "的操作记录.txt 打开失败!" << endl;
		system("pause");
		exit(-1);
	}
	out.close();

}

void BankCard::modifyPassword(const string & password)
{
	this->password = password;
}

void BankCard::saveIn(double money)
{
	deposit += money;
}

bool BankCard::takeOut(double money)
{
	if (money > deposit)
		return false;
	else {
		deposit -= money;
		return true;
	}
}

string BankCard::getNumber() const
{
	return number;
}

string BankCard::getUserID() const
{
	return userID;
}

string BankCard::getBankName() const
{
	return bankName;
}

double BankCard::getDeposit() const
{
	return deposit;
}

string BankCard::toString() const
{
	string bankCard;
	char depositStr[50];
	sprintf(depositStr, "%.2lf", deposit);
	bankCard = number + " " + password + " " + userID
		+ " " + bankName + " " + depositStr;

	return bankCard;
}

string BankCard::getPassword() const
{
	return password;
}
