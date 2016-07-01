#ifndef BANKCARD_H
#define BANKCARD_H

#include <string>

using namespace std;

class BankCard 
{
public:
	BankCard(const string& bankCard);//注册银行卡
	void modifyPassword(const string& password);//修改密码
	void saveIn(double money);//存款
	bool takeOut(double money);//取款
	string getNumber() const;//获取卡号
	string getUserID() const;//获取身份证号
	string getBankName() const;//获取所属银行名
	double getDeposit() const;//获取余额
	string toString() const;//银行卡信息转成字符串
	string getPassword() const;//获取密码
private:
	string number;//卡号,10位
	string password;//密码
	string userID;//用户身份证号
	string bankName;//所属银行名
	double deposit;//存款
};

#endif
