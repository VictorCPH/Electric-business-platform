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
		cout << "BankName.txt 打开失败!" << endl;
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
	cout << "剩余金额: " << str << endl;
}

void Bank::saveInMoney(BankCard * bankCard)
{
	cout << "请输入存款金额(不能小于0.01或大于2000W): " << endl;
	double money;
	cin >> money;

	while (cin.fail() || money < 0.01 || money > 20000000) {
		cout << "输入有误！请重新输入！" << endl;
		cin.clear(); //清除std::cin的错误状态
		cin.ignore(1024, '\n');//清空输入缓冲区
		cin >> money;
	}

	bankCard->saveIn(money);
		
	
	updateBankCard(bankCard);
	saveInfo();
	cout << "成功存入！" << endl;

	string fileName = bankCard->getNumber() + "的操作记录.txt";//操作记录更新
	ofstream out(fileName.c_str(), ios::app);

	if (!out) {
		cout << fileName + "的操作记录.txt 打开失败!" << endl;
		system("pause");
		exit(-1);
	}

	char moneyStr[50];
	sprintf(moneyStr, "%.2f", money);
	out << getCurTime() << "," << "存入 " << moneyStr << " 元" << endl;
}

void Bank::takeOutMoney(BankCard * bankCard)
{
	showDeposit(bankCard);
	while (1) {
		cout << "请输入取款金额(单笔上限100万): " << endl;
		double money;

		cin >> money;
		if (!cin.fail()) {
			if (money > 1000000) {
				cout << "超过单笔上限,取款失败！" << endl;
			}
			else if (bankCard->takeOut(money)) {
				updateBankCard(bankCard);
				saveInfo();
				cout << "成功取出！" << endl;

				string fileName = bankCard->getNumber() + "的操作记录.txt";//操作记录更新
				ofstream out(fileName.c_str(), ios::app);

				if (!out) {
					cout << fileName + "的操作记录.txt 打开失败!" << endl;
					system("pause");
					exit(-1);
				}
				char moneyStr[50];
				sprintf(moneyStr, "%.2f", money);
				out << getCurTime() << "," << "取出 " << moneyStr << " 元" << endl;
			}
			else {
				cout << "余额不足！" << endl;
			}
			break;
		}
		else {
			cout << "输入有误！请重新输入！" << endl;
			cin.clear(); //清除std::cin的错误状态
			cin.ignore(1024, '\n');//清空输入缓冲区
		}
	}
}

void Bank::modifyPassword(BankCard * bankCard)
{
	cout << "请设置新密码(4~10个字符): " << endl;
	string password;
	cin >> password;

	while (1) {
		if (password.size() > 10 || password.size() < 4) {
			cout << "密码长度不符，请重新输入:" << endl;
			cin >> password;
		}
		else if (password == bankCard->getPassword()) {
			cout << "新密码与旧密码一致，请重新输入:" << endl;
			cin >> password;
		}
		else{
			break;
		}
	}

	bankCard->modifyPassword(password);
	updateBankCard(bankCard);
	saveInfo();
	cout << "密码修改成功" << endl;

	string fileName = bankCard->getNumber() + "的操作记录.txt";//操作记录更新
	ofstream out(fileName.c_str(), ios::app);

	if (!out) {
		cout << fileName + "的操作记录.txt 打开失败!" << endl;
		system("pause");
		exit(-1);
	}

	out << getCurTime() << "," << "用户修改了密码" << endl;
}

void Bank::checkOperationRecord(BankCard * bankCard)
{
	string fileName = bankCard->getNumber() + "的操作记录.txt";
	ifstream in(fileName.c_str());

	if (!in) {
		cout << fileName + "的操作记录.txt 打开失败!" << endl;
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
	cout << "有以下银行可供选择:" << endl;
	for (vector<string>::size_type i = 0; i != bankNameStr.size(); ++i) {
		cout << i + 1 << ". " << bankNameStr[i] << "\t";
		if ((i + 1) % 3 == 0)
			cout << endl;
	}
	cout << endl << "请选择银行类型:";
	int index;
	cin >> index;

	while (index < 1 || index > bankNameStr.size() || cin.fail()) {
		cout << "输入有误!请重新输入:";
		cin.clear(); //清除std::cin的错误状态
		cin.ignore(1024, '\n');//清空输入缓冲区
		cin >> index;
	}

	string password;//密码
	string userID;//用户身份证号
	string number = CreateNumber();

	cout << "银行类型: " << bankNameStr[index-1] << endl;
	cout << "请输入身份证号: ";
	cin >> userID;
	cout << "请设置密码(4~10个字符): ";
	cin >> password;
	
	while (password.size() > 10 || password.size() < 4) {
		cout << "密码长度不符，请重新输入:" << endl;
		cin >> password;
	}

	string bankCard;
	bankCard = number +" " + password + " " + userID + " " + bankNameStr[index-1] + " 0.00";

	bankCardText.updateInfo(number, bankCard);

	cout << "银行卡注册成功，您的卡号是: " << number << endl;
	saveInfo();
	cout << "1.返回上一页面" << endl;
	cout << "2.退出程序" << endl;
	index = 0;
	cin >> index;

	while (index > 2 || index < 1 || cin.fail()) {
		cout << "输入有误!请重新输入:" << endl;
		cin.clear(); //清除std::cin的错误状态
		cin.ignore(1024, '\n');//清空输入缓冲区
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
		cout << "请输入卡号: ";
		cin >> number;
		cout << "请输入密码: ";
		cin >> password;

		if (bankCardText.isPassword(number, password)) {
			BankCard* bankCard = new BankCard(bankCardText.getBankCard(number));
			operation(bankCard);
			break;
		}
		else
			cout << "密码错误！请重新输入！" << endl;
	}
}

void Bank::operation(BankCard* bankCard)
{
	system("cls");
	cout << "*******菜单*******" << endl;
	cout << "*1.账户余额      *" << endl;
	cout << "*2.存款          *" << endl;
	cout << "*3.取款          *" << endl;
	cout << "*4.修改密码      *" << endl;
	cout << "*5.查看操作记录  *" << endl;
	cout << "*6.返回上一页面  *" << endl;
	//cout << "*5.转账          *" << endl;
	//cout << "*6.注销          *" << endl;
	cout << "******************" << endl;
	int index;
	cin >> index;

	while (index > 6 || index < 1 || cin.fail()) {
		cout << "输入有误!请重新输入:" << endl;
		cin.clear(); //清除std::cin的错误状态
		cin.ignore(1024, '\n');//清空输入缓冲区
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


	cout << "1.返回上一页面" << endl;
	cout << "2.退出程序" << endl;
	index = 0;
	cin >> index;

	while (index > 2 || index < 1 || cin.fail()) {
		cout << "输入有误!请重新输入:" << endl;
		cin.clear(); //清除std::cin的错误状态
		cin.ignore(1024, '\n');//清空输入缓冲区
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
		return -1;		//密码错误
	}
	else {
		BankCard* bankCard = new BankCard(bankCardText.getBankCard(number));
		if (bankCard->takeOut(money)) {
			bankCardText.updateInfo(number, bankCard->toString());
			saveInfo();
			if (money > 1e-4) { //money不为0，即不是在绑定，而是在付款
				string fileName = bankCard->getNumber() + "的操作记录.txt";//操作记录更新
				ofstream out(fileName.c_str(), ios::app);

				if (!out) {
					cout << fileName + "的操作记录.txt 打开失败!" << endl;
					system("pause");
					exit(-1);
				}

				out << getCurTime() << ",在某电商平台消费了 " << money << " 元" << endl;
			}
			bankCardText.saveFile();
			return 1;//支付成功
		}
		else
			return 0;//余额不足
	}

}

