#include "stdafx.h"
#include "User.h"
#include "CurrentTime.h"
#include <fstream>
#include "winsock2.h"
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

User::User(const string& name, const string& pass, const vector<string>& bankCards,pair<double, double>& discount)
{
	this->userName = name;
	this->password = pass;
	this->bankCards = bankCards;
	myCart.discount = discount;
}

void User::addProduct(Product * prod)
{
	myCart.addProduct(prod);
}

void User::removeProduct(int i)
{
	myCart.removeProduct(i);
}

bool User::payOff(double pay)
{
	const int BUF_SIZE = 64;
	WSADATA wsd; //WSADATA变量
	SOCKET sHost; //服务器套接字
	SOCKADDR_IN servAddr; //服务器地址
	char buf[BUF_SIZE]; //接收数据缓冲区
	char bufRecv[BUF_SIZE];
	int retVal; //返回值
				//初始化套结字动态库
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		system("pause");
		return false;
	}
	//创建套接字
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		cout << "socket failed!" << endl;
		WSACleanup();//释放套接字资源
		system("pause");
		return false;
	}

	//设置服务器地址
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons((short)4999);
	int nServAddlen = sizeof(servAddr);

	//连接服务器
	retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (SOCKET_ERROR == retVal)
	{
		cout << "connect failed!" << endl;
		closesocket(sHost); //关闭套接字
		WSACleanup(); //释放套接字资源
		system("pause");
		return false;
	}

	system("cls");
	cout << "欢迎进入支付界面" << endl;

	string number, CardPassword;
	if (bankCards.size() == 0) {
		cout << userName + "，您还未绑定银行卡！" << endl;
		pair<string, string> bankCard = bindBankCard();

		number = bankCard.first;
		CardPassword = bankCard.second;

		while (1) {
			ZeroMemory(buf, BUF_SIZE);

			char money[20];
			strcpy(money, "0");
			string dataPacket = number + " " + CardPassword + " " + money;
			strcpy(buf, dataPacket.c_str());
			retVal = send(sHost, buf, strlen(buf), 0);

			ZeroMemory(bufRecv, BUF_SIZE);
			recv(sHost, bufRecv, BUF_SIZE, 0); // 接收服务器端的数据， 只接收5个字符

			if (strcmp(bufRecv, "true") == 0) {
				bankCards.push_back(number);
				cout << "绑定成功！" << endl;
				system("pause");
				closesocket(sHost); //关闭套接字
				WSACleanup(); //释放套接字资源
				return payOff(pay);
			}
			else if (strcmp(bufRecv, "false") == 0) {
				cout << "绑定失败，密码或卡号不匹配！" << endl;
				cout << "返回支付界面！" << endl;
				system("pause");
				closesocket(sHost); //关闭套接字
				WSACleanup(); //释放套接字资源
				return payOff(pay);
			}
			else {
				cout << "数据接受失败！" << endl;
			}
		}
	}
	else {
		for (int i = 0; i != bankCards.size(); ++i) {
			cout << "银行卡" << i + 1 << ". " << bankCards[i] << endl;
		}
		cout << "1.选择银行卡   2.绑定其他银行卡" << endl;

		int index;
		cin >> index;
		while (index > 2 || index < 1 || cin.fail()) {
			cout << "输入有误！请重新输入:";
			cin.clear(); //清除std::cin的错误状态
			cin.ignore(1024, '\n');//清空输入缓冲区
			cin >> index;
		}

		if (index == 1) {
			cout << "请选择要使用的银行卡: ";


			cin >> index;

			while (index > bankCards.size() || index < 1 || cin.fail()) {
				cout << "输入有误！请重新输入:";
				cin.clear(); //清除std::cin的错误状态
				cin.ignore(1024, '\n');//清空输入缓冲区
				cin >> index;
			}

			number = bankCards[index - 1];
			cout << "您选择了银行卡 " << number << endl;
			cout << "请输入银行卡密码: " << endl;
			cin >> CardPassword;

			while (1) {
				ZeroMemory(buf, BUF_SIZE);

				char money[20];
				sprintf(money, "%.1lf\n", pay);
				string dataPacket = number + " " + CardPassword + " " + money;
				strcpy(buf, dataPacket.c_str());
				retVal = send(sHost, buf, strlen(buf), 0);

				ZeroMemory(bufRecv, BUF_SIZE);
				recv(sHost, bufRecv, BUF_SIZE, 0); // 接收服务器端的数据， 只接收5个字符


				if (strcmp(bufRecv, "true") == 0) {
					cout << "支付成功，消费 " << pay << " 元" << endl;
					recordConsumption();
					system("pause");
					break;
				}
				else if (strcmp(bufRecv, "false") == 0) {
					cout << "密码错误！请重新输入:";
					cin >> CardPassword;
				}
				else if (strcmp(bufRecv, "lack") == 0) {
					cout << "余额不足" << endl;
					system("pause");
					return false;
				}
				else {
					cout << "数据接受失败！" << endl;
				}
			}
		}
		else {
			pair<string, string> bankCard = bindBankCard();

			number = bankCard.first;
			CardPassword = bankCard.second;

			while (1) {
				vector<string>::iterator it = find(bankCards.begin(), bankCards.end(), number);//判断该卡是否已经绑定过

				if (it != bankCards.end()) {
					cout << "该银行卡已经绑定过了,请重新输入！" << endl;
					pair<string, string> bankCard = bindBankCard();

					number = bankCard.first;
					CardPassword = bankCard.second;
				}
				else
					break;
			}

			while (1) {
				ZeroMemory(buf, BUF_SIZE);

				char money[20];
				strcpy(money, "0");
				string dataPacket = number + " " + CardPassword + " " + money;
				strcpy(buf, dataPacket.c_str());
				retVal = send(sHost, buf, strlen(buf), 0);

				ZeroMemory(bufRecv, BUF_SIZE);
				recv(sHost, bufRecv, BUF_SIZE, 0); // 接收服务器端的数据， 只接收5个字符

				if (strcmp(bufRecv, "true") == 0) {
					bankCards.push_back(number);
					cout << "绑定成功！" << endl;
					system("pause");
					closesocket(sHost); //关闭套接字
					WSACleanup(); //释放套接字资源
					return payOff(pay);
				}
				else if (strcmp(bufRecv, "false") == 0) {
					cout << "绑定失败，密码或卡号不匹配！" << endl;
					cout << "返回支付界面！" << endl;
					system("pause");
					closesocket(sHost); //关闭套接字
					WSACleanup(); //释放套接字资源
					return payOff(pay);
				}
				else {
					cout << "数据接受失败！" << endl;
				}
			}
		}
	}
	//退出
	closesocket(sHost); //关闭套接字
	WSACleanup(); //释放套接字资源
	return true;
}

void User::clearCart()
{
	myCart.clear();
}

double User::checkMyCart()
{
	myCart.show(cout);
	return myCart.total();
}

list<Product*> User::buyProductList()
{
	for (list<Product *>::const_iterator it = myCart.productList.begin(); it != myCart.productList.end(); ++it) {
		(*it)->setInventory((*it)->getInventory() - (*it)->getQuantity());//当前库存等于原来库存 减去 购买数量
	}
	return myCart.productList;
}

pair<string, string> User::bindBankCard()
{
	cout << "请输入一张银行卡卡号进行绑定:";
	string number;
	cin >> number;
	cout << "请输入银行卡密码:";
	string cardPassword;
	cin >> cardPassword;

	return make_pair(number, cardPassword);
}

void User::recordShopCart()
{
	string fileName = userName + "购物车信息.txt";
	ofstream out(fileName.c_str(), ios::out);

	if (!out) {
		cout << fileName + "购物车信息.txt 打开失败!" << endl;
		system("pause");
		exit(-1);
	}

	for (list<Product*>::const_iterator it = myCart.productList.begin(); it != myCart.productList.end(); ++it) {
		out << (*it)->getId() << "\t" << (*it)->getName() << "\t" << (*it)->getQuantity() << "\t"
			<< (*it)->getPrice() << "\n";
	}
}

void User::checkConsumptionRecord()
{
	string fileName = userName + "消费记录.txt";
	ifstream in(fileName.c_str());

	if (!in) {
		cout << fileName + "消费记录.txt 打开失败!" << endl;
		system("pause");
		exit(-1);
	}
	string line;
	while (getline(in, line)) {
		cout << line << endl;
	}
}

vector<string> User::getBankCards()
{
	return bankCards;
}

string User::getUserName()
{
	return userName;
}

string User::getPassword()
{
	return password;
}

void User::modifyPassword(const string & password)
{
	this->password = password;
}

void User::recordConsumption()
{
	string fileName = userName + "消费记录.txt";
	ofstream out(fileName.c_str(), ios::app);

	if (!out) {
		cout << fileName + "消费记录.txt 打开失败!" << endl;
		system("pause");
		exit(-1);
	}
	
	out << getCurTime() << endl;
	myCart.show(out);
}
