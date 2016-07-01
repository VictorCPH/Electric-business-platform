
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
		cout << "欢迎进入银行管理系统" << endl;
		cout << "1.银行卡注册" << endl;
		cout << "2.银行卡登陆" << endl;
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
	WSADATA			wsd;			//WSADATA变量
	SOCKET			sServer;		//服务器套接字
	SOCKET			sClient;		//客户端套接字
	SOCKADDR_IN		addrServ;;		//服务器地址
	char			buf[BUF_SIZE];	//接收数据缓冲区
	char			sendBuf[BUF_SIZE];//返回给客户端得数据
	int				retVal;			//返回值

	while (1) {
		//初始化套结字动态库
		if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
		{
			cout << "WSAStartup failed!" << endl;
			system("pause");
			return 1;
		}

		//创建套接字
		sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == sServer)
		{
			cout << "socket failed!" << endl;
			WSACleanup();//释放套接字资源;
			system("pause");
			return  -1;
		}

		//服务器套接字地址 
		addrServ.sin_family = AF_INET;
		addrServ.sin_port = htons(4999);
		addrServ.sin_addr.s_addr = INADDR_ANY;
		//绑定套接字
		retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
		if (SOCKET_ERROR == retVal)
		{
			cout << "bind failed!" << endl;
			closesocket(sServer);	//关闭套接字
			WSACleanup();			//释放套接字资源;
			system("pause");
			return -1;
		}

		//开始监听 
		retVal = listen(sServer, 1);
		if (SOCKET_ERROR == retVal)
		{
			cout << "listen failed!" << endl;
			closesocket(sServer);	//关闭套接字
			WSACleanup();			//释放套接字资源;
			system("pause");
			return -1;
		}

		//接受客户端请求
		sockaddr_in addrClient;
		int addrClientlen = sizeof(addrClient);
		sClient = accept(sServer, (sockaddr FAR*)&addrClient, &addrClientlen);
		if (INVALID_SOCKET == sClient)
		{
			closesocket(sServer);	//关闭套接字
			WSACleanup();			//释放套接字资源;
			system("pause");
			return -1;
		}

		while (true)
		{
			//接收客户端数据
			ZeroMemory(buf, BUF_SIZE);
			retVal = recv(sClient, buf, BUF_SIZE, 0);
			if (SOCKET_ERROR == retVal)
			{
				closesocket(sServer);	//关闭套接字
				closesocket(sClient);	//关闭套接字		
				WSACleanup();			//释放套接字资源;
				break;
			}

			string dataPacket(buf);
			istringstream line(dataPacket);

			string number, password, pay;

			line >> number >> password >> pay;
			double money = atof(pay.c_str());

			int result = bank.isdeduct(number, password, money);
			if (result == 1) {
				strcpy_s(sendBuf, "true");//支付成功
			}
			else if (result == -1) {
				strcpy_s(sendBuf, "false");//密码错误
			}
			else {
				strcpy_s(sendBuf, "lack");//余额不足
			}
			send(sClient, sendBuf, strlen(sendBuf), 0);
		}

		//退出
		closesocket(sServer);	//关闭套接字
		closesocket(sClient);	//关闭套接字
		WSACleanup();			//释放套接字资源;
	}
	return 0;
}