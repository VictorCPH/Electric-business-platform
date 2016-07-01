#ifndef BANKCARD_H
#define BANKCARD_H

#include <string>

using namespace std;

class BankCard 
{
public:
	BankCard(const string& bankCard);//ע�����п�
	void modifyPassword(const string& password);//�޸�����
	void saveIn(double money);//���
	bool takeOut(double money);//ȡ��
	string getNumber() const;//��ȡ����
	string getUserID() const;//��ȡ���֤��
	string getBankName() const;//��ȡ����������
	double getDeposit() const;//��ȡ���
	string toString() const;//���п���Ϣת���ַ���
	string getPassword() const;//��ȡ����
private:
	string number;//����,10λ
	string password;//����
	string userID;//�û����֤��
	string bankName;//����������
	double deposit;//���
};

#endif
