#include "stdafx.h"
#include "UserInfoText.h"
#include <iostream>
#include <sstream>
#include <fstream>

void UserInfoText::readFile()
{
	ifstream in("UserInfo.txt");
	if (!in)
	{
		cout << "UserInfo.txt 打开失败!" << endl;
		exit(-1);
	}
	string textline;
	while (getline(in, textline)) {
		istringstream line(textline);
		string userName, password, bankCard;
		line >> userName >> password;

		userPassword[userName] = password;
		
		userBankCard[userName] = vector<string>();
		while (line >> bankCard) {
			userBankCard[userName].push_back(bankCard);
		}	
	}
}

void UserInfoText::updateInfo(const string & userName, const string & password)
{
	userPassword[userName] = password;
}

bool UserInfoText::isExist(const string & userName)
{
	return userPassword.count(userName) == 0 ? false : true;;
}

bool UserInfoText::isPassword(const string & userName, const string & password)
{
	if (!isExist(userName))
		return false;
	else {
		if (userPassword[userName] == password)
			return true;
		else
			return false;
	}
}

vector<string> UserInfoText::getUserCard(const string & userName)
{
	return userBankCard[userName];
}

void UserInfoText::updateUserBandCard(const string & userName, vector<string>& bandCards)
{
	userBankCard[userName] = bandCards;
}

void UserInfoText::updateUserPassword(const string & userName, const string & password)
{
	userPassword[userName] = password;
}

void UserInfoText::saveFile()
{
	ofstream out("UserInfo.txt");
	if (!out)
	{
		cout << "UserInfo.txt 打开失败!" << endl;
		system("pause");
		exit(-1);
	}
	for (map<string, string>::const_iterator it = userPassword.begin();
	it != userPassword.end(); ++it) {
		out << it->first << " " << it->second;
		for (int i = 0; i != userBankCard[it->first].size(); ++i)
			out  << " " << userBankCard[it->first][i] ;
		out << endl;
	}
}
