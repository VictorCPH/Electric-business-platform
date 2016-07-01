#include "Bank.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <time.h> 
#include <stdio.h> 

string getCurTime()
{
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));
	return string(tmp);
}


void Bank::getBankName()
{
	ifstream in("BankName.txt");
	if (!in) {
		cout << "BankName.txt ��ʧ��!" << endl;
		exit(-1);
	}

	string bankName;
	while (in >> bankName) {
		bankNameStr.push_back(bankName);
	}
}

string Bank::CreateNumber()
{
	string number;
	while (1) {
		number = "";
		srand(static_cast<unsigned int>(time(NULL)));
		for (int i = 0; i < numberSize; ++i) {
			number += static_cast<char>(rand() % 10 + '0');
		}
		if (!bankCardText.isExist(number))
			break;
	}

	return number;
}

void Bank::showDeposit(BankCard * bankCard)
{
	char str[50];
	sprintf(str, "%.2f", bankCard->getDeposit());
	cout << "ʣ����: " << str << endl;
}

void Bank::saveInMoney(BankCard * bankCard)
{
	cout << "����������(����С��0.01�����2000W): " << endl;
	double money;
	cin >> money;

	while (cin.fail() || money < 0.01 || money > 20000000) {
		cout << "�����������������룡" << endl;
		cin.clear(); //���std::cin�Ĵ���״̬
		cin.ignore(1024, '\n');//������뻺����
		cin >> money;
	}

	bankCard->saveIn(money);
		
	
	updateBankCard(bankCard);
	saveInfo();
	cout << "�ɹ����룡" << endl;

	string fileName = bankCard->getNumber() + "�Ĳ�����¼.txt";//������¼����
	ofstream out(fileName.c_str(), ios::app);

	if (!out) {
		cout << fileName + "�Ĳ�����¼.txt ��ʧ��!" << endl;
		system("pause");
		exit(-1);
	}

	char moneyStr[50];
	sprintf(moneyStr, "%.2f", money);
	out << getCurTime() << "," << "���� " << moneyStr << " Ԫ" << endl;
}

void Bank::takeOutMoney(BankCard * bankCard)
{
	showDeposit(bankCard);
	while (1) {
		cout << "������ȡ����(��������100��): " << endl;
		double money;

		cin >> money;
		if (!cin.fail()) {
			if (money > 1000000) {
				cout << "������������,ȡ��ʧ�ܣ�" << endl;
			}
			else if (bankCard->takeOut(money)) {
				updateBankCard(bankCard);
				saveInfo();
				cout << "�ɹ�ȡ����" << endl;

				string fileName = bankCard->getNumber() + "�Ĳ�����¼.txt";//������¼����
				ofstream out(fileName.c_str(), ios::app);

				if (!out) {
					cout << fileName + "�Ĳ�����¼.txt ��ʧ��!" << endl;
					system("pause");
					exit(-1);
				}
				char moneyStr[50];
				sprintf(moneyStr, "%.2f", money);
				out << getCurTime() << "," << "ȡ�� " << moneyStr << " Ԫ" << endl;
			}
			else {
				cout << "���㣡" << endl;
			}
			break;
		}
		else {
			cout << "�����������������룡" << endl;
			cin.clear(); //���std::cin�Ĵ���״̬
			cin.ignore(1024, '\n');//������뻺����
		}
	}
}

void Bank::modifyPassword(BankCard * bankCard)
{
	cout << "������������(4~10���ַ�): " << endl;
	string password;
	cin >> password;

	while (1) {
		if (password.size() > 10 || password.size() < 4) {
			cout << "���볤�Ȳ���������������:" << endl;
			cin >> password;
		}
		else if (password == bankCard->getPassword()) {
			cout << "�������������һ�£�����������:" << endl;
			cin >> password;
		}
		else{
			break;
		}
	}

	bankCard->modifyPassword(password);
	updateBankCard(bankCard);
	saveInfo();
	cout << "�����޸ĳɹ�" << endl;

	string fileName = bankCard->getNumber() + "�Ĳ�����¼.txt";//������¼����
	ofstream out(fileName.c_str(), ios::app);

	if (!out) {
		cout << fileName + "�Ĳ�����¼.txt ��ʧ��!" << endl;
		system("pause");
		exit(-1);
	}

	out << getCurTime() << "," << "�û��޸�������" << endl;
}

void Bank::checkOperationRecord(BankCard * bankCard)
{
	string fileName = bankCard->getNumber() + "�Ĳ�����¼.txt";
	ifstream in(fileName.c_str());

	if (!in) {
		cout << fileName + "�Ĳ�����¼.txt ��ʧ��!" << endl;
		system("pause");
		exit(-1);
	}

	string line;
	while (getline(in, line)) {
		cout << line << endl;
	}
}



void Bank::initialization()
{
	bankCardText.readFile();
	getBankName();

}

void Bank::registerBankCard()
{
	system("cls");
	cout << "���������пɹ�ѡ��:" << endl;
	for (vector<string>::size_type i = 0; i != bankNameStr.size(); ++i) {
		cout << i + 1 << ". " << bankNameStr[i] << "\t";
		if ((i + 1) % 3 == 0)
			cout << endl;
	}
	cout << endl << "��ѡ����������:";
	int index;
	cin >> index;

	while (index < 1 || index > bankNameStr.size() || cin.fail()) {
		cout << "��������!����������:";
		cin.clear(); //���std::cin�Ĵ���״̬
		cin.ignore(1024, '\n');//������뻺����
		cin >> index;
	}

	string password;//����
	string userID;//�û����֤��
	string number = CreateNumber();

	cout << "��������: " << bankNameStr[index-1] << endl;
	cout << "���������֤��: ";
	cin >> userID;
	cout << "����������(4~10���ַ�): ";
	cin >> password;
	
	while (password.size() > 10 || password.size() < 4) {
		cout << "���볤�Ȳ���������������:" << endl;
		cin >> password;
	}

	string bankCard;
	bankCard = number +" " + password + " " + userID + " " + bankNameStr[index-1] + " 0.00";

	bankCardText.updateInfo(number, bankCard);

	cout << "���п�ע��ɹ������Ŀ�����: " << number << endl;
	saveInfo();
	cout << "1.������һҳ��" << endl;
	cout << "2.�˳�����" << endl;
	index = 0;
	cin >> index;

	while (index > 2 || index < 1 || cin.fail()) {
		cout << "��������!����������:" << endl;
		cin.clear(); //���std::cin�Ĵ���״̬
		cin.ignore(1024, '\n');//������뻺����
		cin >> index;
	}

	if (index == 2) {
		system("pause");
		exit(0);
	}
}

void Bank::login()
{
	system("cls");
	string number, password;

	while (1) {
		cout << "�����뿨��: ";
		cin >> number;
		cout << "����������: ";
		cin >> password;

		if (bankCardText.isPassword(number, password)) {
			BankCard* bankCard = new BankCard(bankCardText.getBankCard(number));
			operation(bankCard);
			break;
		}
		else
			cout << "����������������룡" << endl;
	}
}

void Bank::operation(BankCard* bankCard)
{
	system("cls");
	cout << "*******�˵�*******" << endl;
	cout << "*1.�˻����      *" << endl;
	cout << "*2.���          *" << endl;
	cout << "*3.ȡ��          *" << endl;
	cout << "*4.�޸�����      *" << endl;
	cout << "*5.�鿴������¼  *" << endl;
	cout << "*6.������һҳ��  *" << endl;
	//cout << "*5.ת��          *" << endl;
	//cout << "*6.ע��          *" << endl;
	cout << "******************" << endl;
	int index;
	cin >> index;

	while (index > 6 || index < 1 || cin.fail()) {
		cout << "��������!����������:" << endl;
		cin.clear(); //���std::cin�Ĵ���״̬
		cin.ignore(1024, '\n');//������뻺����
		cin >> index;
	}

	system("cls");
	if (index == 1)
		showDeposit(bankCard);
	else if (index == 2)
		saveInMoney(bankCard);
	else if (index == 3)
		takeOutMoney(bankCard);
	else if (index == 4)
		modifyPassword(bankCard);
	else if (index == 5)
		checkOperationRecord(bankCard);
	else if (index == 6)
		return;


	cout << "1.������һҳ��" << endl;
	cout << "2.�˳�����" << endl;
	index = 0;
	cin >> index;

	while (index > 2 || index < 1 || cin.fail()) {
		cout << "��������!����������:" << endl;
		cin.clear(); //���std::cin�Ĵ���״̬
		cin.ignore(1024, '\n');//������뻺����
		cin >> index;
	}

	if (index == 1)
		operation(bankCard);
	else {
		saveInfo();
		system("pause");
		exit(0);
	}
		
}

void Bank::updateBankCard(BankCard * bankCard)
{
	bankCardText.updateInfo(bankCard->getNumber(), bankCard->toString());
}

void Bank::saveInfo()
{
	bankCardText.saveFile();
}

int Bank::isdeduct(string number, string password, double money)
{
	if (!bankCardText.isPassword(number, password)) {
		return -1;		//�������
	}
	else {
		BankCard* bankCard = new BankCard(bankCardText.getBankCard(number));
		if (bankCard->takeOut(money)) {
			bankCardText.updateInfo(number, bankCard->toString());
			saveInfo();
			if (money > 1e-4) { //money��Ϊ0���������ڰ󶨣������ڸ���
				string fileName = bankCard->getNumber() + "�Ĳ�����¼.txt";//������¼����
				ofstream out(fileName.c_str(), ios::app);

				if (!out) {
					cout << fileName + "�Ĳ�����¼.txt ��ʧ��!" << endl;
					system("pause");
					exit(-1);
				}

				out << getCurTime() << ",��ĳ����ƽ̨������ " << money << " Ԫ" << endl;
			}
			bankCardText.saveFile();
			return 1;//֧���ɹ�
		}
		else
			return 0;//����
	}

}

