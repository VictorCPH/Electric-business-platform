#include "stdafx.h"
#include "ElectronicBusiness.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

void ElectronicBusiness::initialization()
{
	goods.readFile();
	users.readFile();
	readDiscount();
	curUser = NULL;
}

void ElectronicBusiness::registerUser()
{
	system("cls");

	string password;//����
	string userName;//�û���

	while (1) {
		cout << "�������û���(4~10���ַ�): ";
		cin >> userName;

		if (userName.size() > 10 || userName.size() < 4) {
			cout << "�û������Ȳ���, ���������룡" << endl;
		}
		else if (users.isExist(userName)) {
			cout << "���û����Ѵ��ڣ��볢�������û�����" << endl;
		}
		else {
			cout << "���û������ã�" << endl;
			break;
		}
	}

	cout << "����������(4~10���ַ�): ";
	cin >> password;

	while (password.size() > 10 || password.size() < 4) {
		cout << "���볤�Ȳ���������������:" << endl;
		cin >> password;
	}

	users.updateInfo(userName, password);
	users.saveFile();

	

	string fileNameConsume = userName + "���Ѽ�¼.txt";//�������ʺŵ����Ѽ�¼�ļ�
	ofstream outConsume(fileNameConsume.c_str(), ios::out);
	if (!outConsume) {
		cout << fileNameConsume + "���Ѽ�¼.txt ��ʧ��!" << endl;
		system("pause");
		exit(-1);
	}
	outConsume.close();

	string fileNameCart = userName + "���ﳵ��Ϣ.txt";//�������ʺŵĹ��ﳵ��Ϣ�ļ�
	ofstream outCart(fileNameCart.c_str(), ios::out);
	if (!outCart)
	{
		cout << fileNameCart + "���ﳵ��Ϣ.txt ��ʧ��!" << endl;
		system("pause");
		exit(-1);
	}
	outCart.close();

	cout << "ע��ɹ�!" << endl;
	cout << "1.������һҳ��" << endl;

	int index = 0;
	cin >> index;

	while (index != 1 || cin.fail()) {
		cout << "��������!����������:" << endl;
		cin.clear(); //���std::cin�Ĵ���״̬
		cin.ignore(1024, '\n');//������뻺����
		cin >> index;
	}
	
}

void ElectronicBusiness::logInUser()
{
	system("cls");
	string userName, password;

	while (1) {
		cout << "�������û���: ";
		cin >> userName;
		cout << "����������: ";
		cin >> password;

		if (users.isPassword(userName, password)) {
			curUser = new User(userName, password, users.getUserCard(userName), discount);//�����û�
			
			string fileName = userName + "���ﳵ��Ϣ.txt";
			ifstream in(fileName.c_str());
			if (!in)
			{
				cout << fileName + "���ﳵ��Ϣ.txt ��ʧ��!" << endl;
				system("pause");
				exit(-1);
			}

			//��ʼ�����û����ﳵ
			string info;
			while (getline(in, info)) {
				istringstream ss(info);
				string id, tmp;
				int amount;
				ss >> id >> tmp >> amount >> tmp;

				string textline = goods.textLine(goods.getProductIndex(id));
				istringstream line(textline);
				Product* prod = 0;

				if (id.at(0) == '1')
					prod = new Cloth(line);
				else if (id.at(0) == '2')
					prod = new Food(line);
				else if (id.at(0) == '3')
					prod = new Book(line);

				prod->setQuantity(amount);

				curUser->addProduct(prod);
			}


			while (1) {
				Product* prod = searchProduct();
				if (prod != NULL)
					showProduct(prod);
			}
			break;
		}
		else
			cout << "����������������룡" << endl;
	}
}

Product* ElectronicBusiness::searchProduct()
{
	system("cls");
	cout << "����ƽ̨��ҳ" << endl;
	cout << "1.������Ʒ" << endl;
	cout << "2.����ƷĿ¼������Ʒ" << endl;
	cout << "3.�鿴���ﳵ" << endl;
	cout << "4.�鿴�����¼" << endl;
	cout << "5.�޸�����" << endl;
	cout << "6.�˳�����" << endl;
	int func;
	cin >> func;
	while (func >= 7 || func <= 0 || cin.fail()) {
		cout << "�������,����������(1-6):" << endl;
		cin.clear(); //���std::cin�Ĵ���״̬
		cin.ignore(1024, '\n');//������뻺����
		cin >> func;
	}

	pair<string, GoodsInfoText::lineNo> result;  //����������������Ʒ�Ľ��,����(��Ʒid, ��TextQuery�е��к�)
	if (func == 1) {
		result = keywordSearch();
		if (result == pair<string, GoodsInfoText::lineNo>())
			return NULL;
	}
	else if (func == 2)
		result = catalogueSearch();
	else if (func == 3) {
		enterShopCart();
		return NULL;
	}
	else if (func == 4) {
		system("cls");
		cout << "��ʷ������Ϣ����:" << endl;
		curUser->checkConsumptionRecord();
		system("pause");
		return NULL;
	}
	else if (func == 5) {
		system("cls");
		cout << "������������(4~10���ַ�): " << endl;
		string password;
		cin >> password;

		while (1) {
			if (password.size() > 10 || password.size() < 4) {
				cout << "���볤�Ȳ���������������:" << endl;
				cin >> password;
			}
			else if (password == curUser->getPassword()) {
				cout << "�������������һ�£�����������:" << endl;
				cin >> password;
			}
			else {
				break;
			}
		}

		curUser->modifyPassword(password);
		users.updateUserPassword(curUser->getUserName(), curUser->getPassword());
		saveFile();
		cout << "�޸ĳɹ���" << endl;
		system("pause");
		return NULL;
	}
	else{
		saveFile();
		system("pause");
		exit(0);
	}
		
	Product * prod = instantiateGoods(result);

	return prod;
}

void ElectronicBusiness::showProduct(Product * prod)
{
	system("cls");
	prod->description();

	cout << "1.������Ʒ   2.������ҳ" << endl;

	int index;
	cin >> index;

	while (index > 2 || index < 1 || cin.fail()) {
		cout << "�����������������룡" << endl;
		cin.clear(); //���std::cin�Ĵ���״̬
		cin.ignore(1024, '\n');//������뻺����
		cin >> index;
	}

	if (index == 1) {
		cout << "��������Ҫ���������: ";
		int quantity;
		cin >> quantity;

		while (quantity <= 0|| quantity > prod->getInventory() || cin.fail()) {
			if (!cin.fail() && quantity > prod->getInventory()) {
				cout << "����������棬����������:" << endl;
			}
			else {
				cout << "������������������:" << endl;
			}
			cin.clear(); //���std::cin�Ĵ���״̬
			cin.ignore(1024, '\n');//������뻺����
			cin >> quantity;
		}

		prod->setQuantity(quantity);

		cout << "Ӧ֧��: " << prod->getPrice() << endl;
		cout << "��ѡ��: 1.����֧��      2.���빺�ﳵ" << endl;
		int select1;
		cin >> select1;

		while (select1 > 2 || select1 < 1 || cin.fail()) {
			cout << "�����������������룡" << endl;
			cin.clear(); //���std::cin�Ĵ���״̬
			cin.ignore(1024, '\n');//������뻺����
			cin >> select1;
		}
		
		if (select1 == 1) {
			curUser->addProduct(prod);
			enterShopCart();
		}
		else {
			curUser->addProduct(prod);
			cout << "�ɹ����빺�ﳵ��\n";
		}

		cout << "��ѡ��: 1.���ؼ�������   2.���빺�ﳵ" << endl;
		int select2;
		cin >> select2;

		while (select2 > 2 || select2 < 1 || cin.fail()) {
			cout << "�����������������룡" << endl;
			cin.clear(); //���std::cin�Ĵ���״̬
			cin.ignore(1024, '\n');//������뻺����
			cin >> select2;
		}

		if (select2 == 2) {
			enterShopCart();
		}
	}
}

void ElectronicBusiness::enterShopCart()
{
	while (1) {
		system("cls");
		cout << "���ﳵ��Ϣ����:" << endl;
		double pay = curUser->checkMyCart();
		
		if (curUser->buyProductList().size() == 0) {
			cout << "1.���ؼ�������" << endl;
			int index3;
			cin >> index3;

			while (index3 != 1 || cin.fail()) {
				cout << "��������!����������:" << endl;
				cin.clear(); //���std::cin�Ĵ���״̬
				cin.ignore(1024, '\n');//������뻺����
				cin >> index3;
			}
			break;
		}
		else {
			cout << "1.�ύ����   2.ɾ����Ʒ   3.���ؼ�������" << endl;

			int index;
			cin >> index;

			while (index > 3 || index < 1 || cin.fail()) {
				cout << "��������!����������:" << endl;
				cin.clear(); //���std::cin�Ĵ���״̬
				cin.ignore(1024, '\n');//������뻺����
				cin >> index;
			}

			if (index == 1) {
				if (curUser->payOff(pay) == true) {//֧���ɹ�
					users.updateUserBandCard(curUser->getUserName(), curUser->getBankCards());//���°󶨵����п�
					updateProduct(curUser->buyProductList());//���²�Ʒ���
					goods.saveFile();//�����Ʒ��Ϣ�ļ�
					curUser->clearCart();//��չ��ﳵ
				}
				break;
			}
			else if (index == 2) {
				cout << "������Ҫɾ������Ʒ�����: ";
				int index2;
				cin >> index2;

				while (index2 > curUser->buyProductList().size() || index2 < 1 || cin.fail()) {
					cout << "��������!����������:";
					cin.clear(); //���std::cin�Ĵ���״̬
					cin.ignore(1024, '\n');//������뻺����
					cin >> index2;
				}

				curUser->removeProduct(index2);
				cout << "ɾ���ɹ���" << endl;
				system("pause");
			}
			else
				break;
		}
	}
}

void ElectronicBusiness::updateProduct(list<Product*> productList)
{
	for (list<Product *>::const_iterator it = productList.begin(); it != productList.end(); ++it) {
		goods.updateProductInfo((*it));
	}
}



void ElectronicBusiness::saveFile()
{
	goods.saveFile();
	users.saveFile();
	if (curUser != NULL)
		curUser->recordShopCart();
}

pair<string, GoodsInfoText::lineNo> ElectronicBusiness::keywordSearch()
{
	cout << "������ؼ���:" << endl;
	string str;
	cin >> str;

	vector<GoodsInfoText::lineNo> locs = goods.runQueryWord(str);

	if (locs.size() == 0) {
		system("cls");
		cout << "û���ҵ������Ʒ" << endl;
		cout << "1.������ҳ" << endl;
		int index;
		cin >> index;
		while (index != 1 || cin.fail())
		{
			cout << "�������,����������:" << endl;
			cin.clear(); //���std::cin�Ĵ���״̬
			cin.ignore(1024, '\n');//������뻺����
			cin >> index;
		}
		return pair<string, GoodsInfoText::lineNo>();
	}
	else
		productList(locs);

	int goodsSelect;
	cin >> goodsSelect;
	while (goodsSelect > locs.size() || goodsSelect <= 0) {
		cout << "�������,����������(1-" << locs.size() << " ):" << endl;
		cin >> goodsSelect;
	}

	pair<string, GoodsInfoText::lineNo> result;
	int i = 1;
	for (vector<GoodsInfoText::lineNo>::const_iterator it = locs.begin();
	it != locs.end(); ++it, ++i)
		if (i == goodsSelect) {
			result = make_pair(goods.getId(goods.textLine(*it)), *it);
			break;
		}

	return result;
}

void ElectronicBusiness::productList(const vector<GoodsInfoText::lineNo>& locs)
{
	typedef vector<GoodsInfoText::lineNo> line_nums;
	int i = 1;
	system("cls");
	for (line_nums::const_iterator it = locs.begin(); it != locs.end(); ++it, ++i) {
		cout << i << "." << goods.getName(goods.textLine(*it)) << endl;
	}
}

Product * ElectronicBusiness::instantiateGoods(const pair<string, GoodsInfoText::lineNo>& result)
{
	Product* prod = 0;
	string index(result.first.begin(), result.first.begin() + 1);
	istringstream line(goods.textLine(result.second));

	if (index == "1")
		prod = new Cloth(line);
	else if (index == "2")
		prod = new Food(line);
	else if (index == "3")
		prod = new Book(line);

	return prod;
}

pair<string, GoodsInfoText::lineNo> ElectronicBusiness::catalogueSearch()
{
	system("cls");
	cout << "һ����Ʒ��Ŀ:" << endl;
	cout << "1.��װ" << endl
		<< "2.ʳƷ" << endl
		<< "3.ͼ��" << endl;

	cout << "��ѡ����Ŀ(1-3):" << endl;
	int indexOne, indexTwo;
	cin >> indexOne;

	while (indexOne >= 4 || indexOne <= 0 || cin.fail()) {
		cout << "�������,����������(1-3):" << endl;
		cin.clear(); //���std::cin�Ĵ���״̬
		cin.ignore(1024, '\n');//������뻺����
		cin >> indexOne;
	}
	switch (indexOne)
	{
	case 1:
		system("cls");
		cout << "������Ʒ��Ŀ:" << endl;
		cout << "1.ȹ��" << endl
			<< "2.����" << endl
			<< "3.��װ" << endl
			<< "4.Ь��" << endl
			<< "5.ñ��" << endl;
		cout << "��ѡ����Ŀ(1-5):" << endl;
		cin >> indexTwo;

		while (indexTwo >= 6 || indexOne <= 0 || cin.fail()) {
			cout << "�������,����������(1-5):" << endl;
			cin.clear(); //���std::cin�Ĵ���״̬
			cin.ignore(1024, '\n');//������뻺����
			cin >> indexTwo;
		}
		break;
	case 2:
		system("cls");
		cout << "������Ʒ��Ŀ:" << endl;
		cout << "1.������ʳ" << endl
			<< "2.Ӫ��Ʒ" << endl
			<< "3.��������" << endl
			<< "4.��/����" << endl;
		cout << "��ѡ����Ŀ(1-4):" << endl;
		cin >> indexTwo;

		while (indexTwo >= 5 || indexOne <= 0 || cin.fail()) {
			cout << "�������,����������(1-4):" << endl;
			cin.clear(); //���std::cin�Ĵ���״̬
			cin.ignore(1024, '\n');//������뻺����
			cin >> indexTwo;
		}
		break;
	case 3:
		system("cls");
		cout << "������Ʒ��Ŀ:" << endl;
		cout << "1.��ͯ�鼮" << endl
			<< "2.�̲�" << endl
			<< "3.��ѧ" << endl
			<< "4.�����" << endl
			<< "5.����ѧ" << endl;
		cout << "��ѡ����Ŀ(1-5):" << endl;
		cin >> indexTwo;

		while (indexTwo >= 6 || indexOne <= 0 || cin.fail()) {
			cout << "�������,����������(1-5):" << endl;
			cin.clear(); //���std::cin�Ĵ���״̬
			cin.ignore(1024, '\n');//������뻺����
			cin >> indexTwo;
		}
		break;
	default:
		break;
	}

	string idOneToTwo = intToString(indexOne) + intToString(indexTwo);
	vector<GoodsInfoText::lineNo> locs = goods.runQueryId(idOneToTwo);

	productList(locs);

	int goodsSelect;
	cin >> goodsSelect;

	while (goodsSelect > locs.size() || goodsSelect <= 0 || cin.fail()) {
		cout << "�������,����������(1-" << locs.size() << " ):" << endl;
		cin.clear(); //���std::cin�Ĵ���״̬
		cin.ignore(1024, '\n');//������뻺����
		cin >> goodsSelect;
	}


	pair<string, GoodsInfoText::lineNo> result;
	int i = 1;
	for (vector<GoodsInfoText::lineNo>::const_iterator it = locs.begin();
	it != locs.end(); ++it, ++i)
		if (i == goodsSelect) {
			result = make_pair(goods.getId(goods.textLine(*it)), *it);
			break;
		}

	return result;
}

void ElectronicBusiness::readDiscount()
{
	ifstream in("discount.txt");
	if (!in) {
		cout << "discount.txt ��ʧ��!" << endl;
		system("pause");
		exit(-1);
	}

	string temp;
	double clothDiscout, foodDiscount, bookDiscount;
	double X, Y;//��X��Y

	in >> temp >> clothDiscout >> temp >> foodDiscount
		>> temp >> bookDiscount >> temp >> X >> Y;

	Cloth::setDiscount(clothDiscout);
	Food::setDiscount(foodDiscount);
	Book::setDiscount(bookDiscount);

	discount.first = X;
	discount.second = Y;
}

string ElectronicBusiness::intToString(int index)
{
	stringstream ss;
	ss << index;
	return ss.str();
}
