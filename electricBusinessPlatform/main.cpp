
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
		cout << "��ӭʹ�õ���ƽ̨" << endl;
		cout << "1.�û�ע��" << endl;
		cout << "2.�û���½" << endl;
		cout << "3.�˳�����" << endl;
		cout << "��ѡ��:" << endl;

		int index;
		cin >> index;

		while (index > 3 || index < 1 || cin.fail())
		{
			cout << "��������!����������:" << endl;
			cin.clear(); //���std::cin�Ĵ���״̬
			cin.ignore(1024, '\n');//������뻺����
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
