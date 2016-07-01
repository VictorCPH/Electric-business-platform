#ifndef BANK_H
#define BANK_H

#include <string>
#include <vector>
#include "BankCardText.h"
using namespace std;

class Bank
{
public:
	void initialization();//�������п���Ϣ����ȡ������
	void registerBankCard();//ע�����п�
	void login();//��½
	void operation(BankCard* bankCard);//�û����е�һϵ�в���
	void updateBankCard(BankCard* bankCard);//�������п���Ϣ
	void saveInfo();//�������п���Ϣ
	int isdeduct(string number, string password, double money);//�Ƿ�۷�
private:
	const int numberSize = 10;
	void getBankName();
	string CreateNumber();//����һ��ȫ�µĿ��ţ�10λ
	void showDeposit(BankCard* bankCard);//��ʾ���
	void saveInMoney(BankCard* bankCard);//������
	void takeOutMoney(BankCard* bankCard);//ȡ�����
	void modifyPassword(BankCard* bankCard);//�޸�����
	void checkOperationRecord(BankCard* bankCard);//�鿴������¼
	vector<string> bankNameStr;
	BankCardText bankCardText;
};

#endif