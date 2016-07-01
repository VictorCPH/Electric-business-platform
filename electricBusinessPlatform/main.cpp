
#include "stdafx.h"
#include <iostream>
#include "ElectronicBusiness.h"

double Cloth::discount = 1.0;
double Food::discount = 1.0;
double Book::discount = 1.0;

int main()
{
	ElectronicBusiness eBusiness;
	eBusiness.initialization();

	while (1) {
		system("cls");
		cout << "欢迎使用电商平台" << endl;
		cout << "1.用户注册" << endl;
		cout << "2.用户登陆" << endl;
		cout << "3.退出程序" << endl;
		cout << "请选择:" << endl;

		int index;
		cin >> index;

		while (index > 3 || index < 1 || cin.fail())
		{
			cout << "输入有误!请重新输入:" << endl;
			cin.clear(); //清除std::cin的错误状态
			cin.ignore(1024, '\n');//清空输入缓冲区
			cin >> index;
		}

		if (index == 1)
			eBusiness.registerUser();
		else if (index == 2)
			eBusiness.logInUser();
		else if (index == 3) {
			eBusiness.saveFile();
			break;
		}
	}

	system("pause");

	return 0;
}
