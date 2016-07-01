#ifndef USERINFOTEXT_H
#define USERINFOTEXT_H

#include "stdafx.h"
#include <map>
#include <vector>
#include <utility>
#include <string>

using namespace std;

class UserInfoText
{
public:
	void readFile();//读取文件构建用户表
	void updateInfo(const string& userName, const string& password);//更新用户表
	bool isExist(const string& userName);//判断该用户是否存在
	bool isPassword(const string& userName, const string& password);//判断密码是否正确
	vector<string> getUserCard(const string& userName);//获取某个用户的所有绑定银行卡
	void updateUserBandCard(const string& userName, vector<string>& bandCards);//更新用户的银行卡表
	void updateUserPassword(const string& userName, const string& password);//更新用户名-密码 map
	void saveFile();//保存用户信息
private:
	map<string, string> userPassword;//用户名-密码 map
	map<string, vector<string> > userBankCard;//用户名-用户所有绑定的银行卡 map
};



#endif
