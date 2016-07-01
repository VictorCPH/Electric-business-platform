#ifndef BANKCARDTEXT_H
#define BANKCARDTEXT_H

#include <string>
#include <map>
#include "BankCard.h"

using namespace std;

class BankCardText
{
public:
	void readFile();//读文件，构造map
	bool isExist(const string& number);//判断该卡是否存在
	bool isPassword(const string& number, const string& password);//判断密码是否正确
	void updateInfo(const string& number, const string& bankCard);//更新某张卡的信息(有就更新，没有就创建）
	string getBankCard(const string& number);//获得某张卡的信息
	void saveFile();//写文件，保存银行卡信息
private:
	map<string, string> bankCardMap;//卡号-银行卡信息 的 map
};

#endif
