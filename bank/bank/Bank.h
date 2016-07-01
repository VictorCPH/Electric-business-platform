#ifndef BANK_H
#define BANK_H

#include <string>
#include <vector>
#include "BankCardText.h"
using namespace std;

class Bank
{
public:
	void initialization();//构建银行卡信息表，获取银行名
	void registerBankCard();//注册银行卡
	void login();//登陆
	void operation(BankCard* bankCard);//用户进行的一系列操作
	void updateBankCard(BankCard* bankCard);//更新银行卡信息
	void saveInfo();//保存银行卡信息
	int isdeduct(string number, string password, double money);//是否扣费
private:
	const int numberSize = 10;
	void getBankName();
	string CreateNumber();//生成一个全新的卡号，10位
	void showDeposit(BankCard* bankCard);//显示余额
	void saveInMoney(BankCard* bankCard);//存款操作
	void takeOutMoney(BankCard* bankCard);//取款操作
	void modifyPassword(BankCard* bankCard);//修改密码
	void checkOperationRecord(BankCard* bankCard);//查看操作记录
	vector<string> bankNameStr;
	BankCardText bankCardText;
};

#endif