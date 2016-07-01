#ifndef BANKCARDTEXT_H
#define BANKCARDTEXT_H

#include <string>
#include <map>
#include "BankCard.h"

using namespace std;

class BankCardText
{
public:
	void readFile();//���ļ�������map
	bool isExist(const string& number);//�жϸÿ��Ƿ����
	bool isPassword(const string& number, const string& password);//�ж������Ƿ���ȷ
	void updateInfo(const string& number, const string& bankCard);//����ĳ�ſ�����Ϣ(�о͸��£�û�оʹ�����
	string getBankCard(const string& number);//���ĳ�ſ�����Ϣ
	void saveFile();//д�ļ����������п���Ϣ
private:
	map<string, string> bankCardMap;//����-���п���Ϣ �� map
};

#endif
