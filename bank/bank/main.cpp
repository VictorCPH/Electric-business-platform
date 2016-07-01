
#include <iostream>
#include <fstream>
#include <sstream>
#include "Bank.h"
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI Server(LPVOID lpParamter);

Bank bank;

int main()
{
	bank.initialization();

	HANDLE hThread = CreateThread(NULL, 0, Server, NULL, 0, NULL);
	CloseHandle(hThread);

	while (1) {
		system("cls");
		cout << "��ӭ�������й���ϵͳ" << endl;
		cout << "1.���п�ע��" << endl;
		cout << "2.���п���½" << endl;
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
			bank.registerBankCard();
		else if (index == 2)
			bank.login();
		else
			break;
	}
	bank.saveInfo();
	system("pause");
	return 0;
}

DWORD WINAPI Server(LPVOID lpParamter)
{
	const int BUF_SIZE = 64;
	WSADATA			wsd;			//WSADATA����
	SOCKET			sServer;		//�������׽���
	SOCKET			sClient;		//�ͻ����׽���
	SOCKADDR_IN		addrServ;;		//��������ַ
	char			buf[BUF_SIZE];	//�������ݻ�����
	char			sendBuf[BUF_SIZE];//���ظ��ͻ��˵�����
	int				retVal;			//����ֵ

	while (1) {
		//��ʼ���׽��ֶ�̬��
		if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
		{
			cout << "WSAStartup failed!" << endl;
			system("pause");
			return 1;
		}

		//�����׽���
		sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == sServer)
		{
			cout << "socket failed!" << endl;
			WSACleanup();//�ͷ��׽�����Դ;
			system("pause");
			return  -1;
		}

		//�������׽��ֵ�ַ 
		addrServ.sin_family = AF_INET;
		addrServ.sin_port = htons(4999);
		addrServ.sin_addr.s_addr = INADDR_ANY;
		//���׽���
		retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
		if (SOCKET_ERROR == retVal)
		{
			cout << "bind failed!" << endl;
			closesocket(sServer);	//�ر��׽���
			WSACleanup();			//�ͷ��׽�����Դ;
			system("pause");
			return -1;
		}

		//��ʼ���� 
		retVal = listen(sServer, 1);
		if (SOCKET_ERROR == retVal)
		{
			cout << "listen failed!" << endl;
			closesocket(sServer);	//�ر��׽���
			WSACleanup();			//�ͷ��׽�����Դ;
			system("pause");
			return -1;
		}

		//���ܿͻ�������
		sockaddr_in addrClient;
		int addrClientlen = sizeof(addrClient);
		sClient = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);
		if (INVALID_SOCKET == sClient)
		{
			closesocket(sServer);	//�ر��׽���
			WSACleanup();			//�ͷ��׽�����Դ;
			system("pause");
			return -1;
		}

		while (true)
		{
			//���տͻ�������
			ZeroMemory(buf, BUF_SIZE);
			retVal = recv(sClient, buf, BUF_SIZE, 0);
			if (SOCKET_ERROR == retVal)
			{
				closesocket(sServer);	//�ر��׽���
				closesocket(sClient);	//�ر��׽���		
				WSACleanup();			//�ͷ��׽�����Դ;
				break;
			}

			string dataPacket(buf);
			istringstream line(dataPacket);

			string number, password, pay;

			line >> number >> password >> pay;
			double money = atof(pay.c_str());

			int result = bank.isdeduct(number, password, money);
			if (result == 1) {
				strcpy_s(sendBuf, "true");//֧���ɹ�
			}
			else if (result == -1) {
				strcpy_s(sendBuf, "false");//�������
			}
			else {
				strcpy_s(sendBuf, "lack");//����
			}
			send(sClient, sendBuf, strlen(sendBuf), 0);
		}

		//�˳�
		closesocket(sServer);	//�ر��׽���
		closesocket(sClient);	//�ر��׽���
		WSACleanup();			//�ͷ��׽�����Դ;
	}
	return 0;
}