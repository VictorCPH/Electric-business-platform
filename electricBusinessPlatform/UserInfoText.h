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
	void readFile();//��ȡ�ļ������û���
	void updateInfo(const string& userName, const string& password);//�����û���
	bool isExist(const string& userName);//�жϸ��û��Ƿ����
	bool isPassword(const string& userName, const string& password);//�ж������Ƿ���ȷ
	vector<string> getUserCard(const string& userName);//��ȡĳ���û������а����п�
	void updateUserBandCard(const string& userName, vector<string>& bandCards);//�����û������п���
	void updateUserPassword(const string& userName, const string& password);//�����û���-���� map
	void saveFile();//�����û���Ϣ
private:
	map<string, string> userPassword;//�û���-���� map
	map<string, vector<string> > userBankCard;//�û���-�û����а󶨵����п� map
};



#endif
