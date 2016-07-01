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
	WSADATA wsd; //WSADATA����
	SOCKET sHost; //�������׽���
	SOCKADDR_IN servAddr; //��������ַ
	char buf[BUF_SIZE]; //�������ݻ�����
	char bufRecv[BUF_SIZE];
	int retVal; //����ֵ
				//��ʼ���׽��ֶ�̬��
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed!" << endl;
		system("pause");
		return false;
	}
	//�����׽���
	sHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sHost)
	{
		cout << "socket failed!" << endl;
		WSACleanup();//�ͷ��׽�����Դ
		system("pause");
		return false;
	}

	//���÷�������ַ
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons((short)4999);
	int nServAddlen = sizeof(servAddr);

	//���ӷ�����
	retVal = connect(sHost, (LPSOCKADDR)&servAddr, sizeof(servAddr));
	if (SOCKET_ERROR == retVal)
	{
		cout << "connect failed!" << endl;
		closesocket(sHost); //�ر��׽���
		WSACleanup(); //�ͷ��׽�����Դ
		system("pause");
		return false;
	}

	system("cls");
	cout << "��ӭ����֧������" << endl;

	string number, CardPassword;
	if (bankCards.size() == 0) {
		cout << userName + "������δ�����п���" << endl;
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
			recv(sHost, bufRecv, BUF_SIZE, 0); // ���շ������˵����ݣ� ֻ����5���ַ�

			if (strcmp(bufRecv, "true") == 0) {
				bankCards.push_back(number);
				cout << "�󶨳ɹ���" << endl;
				system("pause");
				closesocket(sHost); //�ر��׽���
				WSACleanup(); //�ͷ��׽�����Դ
				return payOff(pay);
			}
			else if (strcmp(bufRecv, "false") == 0) {
				cout << "��ʧ�ܣ�����򿨺Ų�ƥ�䣡" << endl;
				cout << "����֧�����棡" << endl;
				system("pause");
				closesocket(sHost); //�ر��׽���
				WSACleanup(); //�ͷ��׽�����Դ
				return payOff(pay);
			}
			else {
				cout << "���ݽ���ʧ�ܣ�" << endl;
			}
		}
	}
	else {
		for (int i = 0; i != bankCards.size(); ++i) {
			cout << "���п�" << i + 1 << ". " << bankCards[i] << endl;
		}
		cout << "1.ѡ�����п�   2.���������п�" << endl;

		int index;
		cin >> index;
		while (index > 2 || index < 1 || cin.fail()) {
			cout << "������������������:";
			cin.clear(); //���std::cin�Ĵ���״̬
			cin.ignore(1024, '\n');//������뻺����
			cin >> index;
		}

		if (index == 1) {
			cout << "��ѡ��Ҫʹ�õ����п�: ";


			cin >> index;

			while (index > bankCards.size() || index < 1 || cin.fail()) {
				cout << "������������������:";
				cin.clear(); //���std::cin�Ĵ���״̬
				cin.ignore(1024, '\n');//������뻺����
				cin >> index;
			}

			number = bankCards[index - 1];
			cout << "��ѡ�������п� " << number << endl;
			cout << "���������п�����: " << endl;
			cin >> CardPassword;

			while (1) {
				ZeroMemory(buf, BUF_SIZE);

				char money[20];
				sprintf(money, "%.1lf\n", pay);
				string dataPacket = number + " " + CardPassword + " " + money;
				strcpy(buf, dataPacket.c_str());
				retVal = send(sHost, buf, strlen(buf), 0);

				ZeroMemory(bufRecv, BUF_SIZE);
				recv(sHost, bufRecv, BUF_SIZE, 0); // ���շ������˵����ݣ� ֻ����5���ַ�


				if (strcmp(bufRecv, "true") == 0) {
					cout << "֧���ɹ������� " << pay << " Ԫ" << endl;
					recordConsumption();
					system("pause");
					break;
				}
				else if (strcmp(bufRecv, "false") == 0) {
					cout << "�����������������:";
					cin >> CardPassword;
				}
				else if (strcmp(bufRecv, "lack") == 0) {
					cout << "����" << endl;
					system("pause");
					return false;
				}
				else {
					cout << "���ݽ���ʧ�ܣ�" << endl;
				}
			}
		}
		else {
			pair<string, string> bankCard = bindBankCard();

			number = bankCard.first;
			CardPassword = bankCard.second;

			while (1) {
				vector<string>::iterator it = find(bankCards.begin(), bankCards.end(), number);//�жϸÿ��Ƿ��Ѿ��󶨹�

				if (it != bankCards.end()) {
					cout << "�����п��Ѿ��󶨹���,���������룡" << endl;
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
				recv(sHost, bufRecv, BUF_SIZE, 0); // ���շ������˵����ݣ� ֻ����5���ַ�

				if (strcmp(bufRecv, "true") == 0) {
					bankCards.push_back(number);
					cout << "�󶨳ɹ���" << endl;
					system("pause");
					closesocket(sHost); //�ر��׽���
					WSACleanup(); //�ͷ��׽�����Դ
					return payOff(pay);
				}
				else if (strcmp(bufRecv, "false") == 0) {
					cout << "��ʧ�ܣ�����򿨺Ų�ƥ�䣡" << endl;
					cout << "����֧�����棡" << endl;
					system("pause");
					closesocket(sHost); //�ر��׽���
					WSACleanup(); //�ͷ��׽�����Դ
					return payOff(pay);
				}
				else {
					cout << "���ݽ���ʧ�ܣ�" << endl;
				}
			}
		}
	}
	//�˳�
	closesocket(sHost); //�ر��׽���
	WSACleanup(); //�ͷ��׽�����Դ
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
		(*it)->setInventory((*it)->getInventory() - (*it)->getQuantity());//��ǰ������ԭ����� ��ȥ ��������
	}
	return myCart.productList;
}

pair<string, string> User::bindBankCard()
{
	cout << "������һ�����п����Ž��а�:";
	string number;
	cin >> number;
	cout << "���������п�����:";
	string cardPassword;
	cin >> cardPassword;

	return make_pair(number, cardPassword);
}

void User::recordShopCart()
{
	string fileName = userName + "���ﳵ��Ϣ.txt";
	ofstream out(fileName.c_str(), ios::out);

	if (!out) {
		cout << fileName + "���ﳵ��Ϣ.txt ��ʧ��!" << endl;
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
	string fileName = userName + "���Ѽ�¼.txt";
	ifstream in(fileName.c_str());

	if (!in) {
		cout << fileName + "���Ѽ�¼.txt ��ʧ��!" << endl;
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
	string fileName = userName + "���Ѽ�¼.txt";
	ofstream out(fileName.c_str(), ios::app);

	if (!out) {
		cout << fileName + "���Ѽ�¼.txt ��ʧ��!" << endl;
		system("pause");
		exit(-1);
	}
	
	out << getCurTime() << endl;
	myCart.show(out);
}
