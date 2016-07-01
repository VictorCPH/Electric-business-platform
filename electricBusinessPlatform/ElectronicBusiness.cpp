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

	string password;//密码
	string userName;//用户名

	while (1) {
		cout << "请输入用户名(4~10个字符): ";
		cin >> userName;

		if (userName.size() > 10 || userName.size() < 4) {
			cout << "用户名长度不符, 请重新输入！" << endl;
		}
		else if (users.isExist(userName)) {
			cout << "该用户名已存在，请尝试其他用户名！" << endl;
		}
		else {
			cout << "该用户名可用！" << endl;
			break;
		}
	}

	cout << "请设置密码(4~10个字符): ";
	cin >> password;

	while (password.size() > 10 || password.size() < 4) {
		cout << "密码长度不符，请重新输入:" << endl;
		cin >> password;
	}

	users.updateInfo(userName, password);
	users.saveFile();

	

	string fileNameConsume = userName + "消费记录.txt";//创建该帐号的消费记录文件
	ofstream outConsume(fileNameConsume.c_str(), ios::out);
	if (!outConsume) {
		cout << fileNameConsume + "消费记录.txt 打开失败!" << endl;
		system("pause");
		exit(-1);
	}
	outConsume.close();

	string fileNameCart = userName + "购物车信息.txt";//创建该帐号的购物车信息文件
	ofstream outCart(fileNameCart.c_str(), ios::out);
	if (!outCart)
	{
		cout << fileNameCart + "购物车信息.txt 打开失败!" << endl;
		system("pause");
		exit(-1);
	}
	outCart.close();

	cout << "注册成功!" << endl;
	cout << "1.返回上一页面" << endl;

	int index = 0;
	cin >> index;

	while (index != 1 || cin.fail()) {
		cout << "输入有误!请重新输入:" << endl;
		cin.clear(); //清除std::cin的错误状态
		cin.ignore(1024, '\n');//清空输入缓冲区
		cin >> index;
	}
	
}

void ElectronicBusiness::logInUser()
{
	system("cls");
	string userName, password;

	while (1) {
		cout << "请输入用户名: ";
		cin >> userName;
		cout << "请输入密码: ";
		cin >> password;

		if (users.isPassword(userName, password)) {
			curUser = new User(userName, password, users.getUserCard(userName), discount);//创建用户
			
			string fileName = userName + "购物车信息.txt";
			ifstream in(fileName.c_str());
			if (!in)
			{
				cout << fileName + "购物车信息.txt 打开失败!" << endl;
				system("pause");
				exit(-1);
			}

			//初始化该用户购物车
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
			cout << "密码错误！请重新输入！" << endl;
	}
}

Product* ElectronicBusiness::searchProduct()
{
	system("cls");
	cout << "电商平台首页" << endl;
	cout << "1.搜索商品" << endl;
	cout << "2.按商品目录索引商品" << endl;
	cout << "3.查看购物车" << endl;
	cout << "4.查看购物记录" << endl;
	cout << "5.修改密码" << endl;
	cout << "6.退出程序" << endl;
	int func;
	cin >> func;
	while (func >= 7 || func <= 0 || cin.fail()) {
		cout << "输入错误,请重新输入(1-6):" << endl;
		cin.clear(); //清除std::cin的错误状态
		cin.ignore(1024, '\n');//清空输入缓冲区
		cin >> func;
	}

	pair<string, GoodsInfoText::lineNo> result;  //搜索或索引到的商品的结果,包括(商品id, 在TextQuery中的行号)
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
		cout << "历史消费信息如下:" << endl;
		curUser->checkConsumptionRecord();
		system("pause");
		return NULL;
	}
	else if (func == 5) {
		system("cls");
		cout << "请设置新密码(4~10个字符): " << endl;
		string password;
		cin >> password;

		while (1) {
			if (password.size() > 10 || password.size() < 4) {
				cout << "密码长度不符，请重新输入:" << endl;
				cin >> password;
			}
			else if (password == curUser->getPassword()) {
				cout << "新密码与旧密码一致，请重新输入:" << endl;
				cin >> password;
			}
			else {
				break;
			}
		}

		curUser->modifyPassword(password);
		users.updateUserPassword(curUser->getUserName(), curUser->getPassword());
		saveFile();
		cout << "修改成功！" << endl;
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

	cout << "1.购买商品   2.返回首页" << endl;

	int index;
	cin >> index;

	while (index > 2 || index < 1 || cin.fail()) {
		cout << "输入有误！请重新输入！" << endl;
		cin.clear(); //清除std::cin的错误状态
		cin.ignore(1024, '\n');//清空输入缓冲区
		cin >> index;
	}

	if (index == 1) {
		cout << "请输入需要购买的数量: ";
		int quantity;
		cin >> quantity;

		while (quantity <= 0|| quantity > prod->getInventory() || cin.fail()) {
			if (!cin.fail() && quantity > prod->getInventory()) {
				cout << "数量超过库存，请重新输入:" << endl;
			}
			else {
				cout << "输入有误！请重新输入:" << endl;
			}
			cin.clear(); //清除std::cin的错误状态
			cin.ignore(1024, '\n');//清空输入缓冲区
			cin >> quantity;
		}

		prod->setQuantity(quantity);

		cout << "应支付: " << prod->getPrice() << endl;
		cout << "请选择: 1.立即支付      2.加入购物车" << endl;
		int select1;
		cin >> select1;

		while (select1 > 2 || select1 < 1 || cin.fail()) {
			cout << "输入有误！请重新输入！" << endl;
			cin.clear(); //清除std::cin的错误状态
			cin.ignore(1024, '\n');//清空输入缓冲区
			cin >> select1;
		}
		
		if (select1 == 1) {
			curUser->addProduct(prod);
			enterShopCart();
		}
		else {
			curUser->addProduct(prod);
			cout << "成功加入购物车！\n";
		}

		cout << "请选择: 1.返回继续购物   2.进入购物车" << endl;
		int select2;
		cin >> select2;

		while (select2 > 2 || select2 < 1 || cin.fail()) {
			cout << "输入有误！请重新输入！" << endl;
			cin.clear(); //清除std::cin的错误状态
			cin.ignore(1024, '\n');//清空输入缓冲区
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
		cout << "购物车信息如下:" << endl;
		double pay = curUser->checkMyCart();
		
		if (curUser->buyProductList().size() == 0) {
			cout << "1.返回继续购物" << endl;
			int index3;
			cin >> index3;

			while (index3 != 1 || cin.fail()) {
				cout << "输入有误!请重新输入:" << endl;
				cin.clear(); //清除std::cin的错误状态
				cin.ignore(1024, '\n');//清空输入缓冲区
				cin >> index3;
			}
			break;
		}
		else {
			cout << "1.提交订单   2.删除商品   3.返回继续购物" << endl;

			int index;
			cin >> index;

			while (index > 3 || index < 1 || cin.fail()) {
				cout << "输入有误!请重新输入:" << endl;
				cin.clear(); //清除std::cin的错误状态
				cin.ignore(1024, '\n');//清空输入缓冲区
				cin >> index;
			}

			if (index == 1) {
				if (curUser->payOff(pay) == true) {//支付成功
					users.updateUserBandCard(curUser->getUserName(), curUser->getBankCards());//更新绑定的银行卡
					updateProduct(curUser->buyProductList());//更新产品库存
					goods.saveFile();//保存产品信息文件
					curUser->clearCart();//清空购物车
				}
				break;
			}
			else if (index == 2) {
				cout << "请输入要删除的商品的序号: ";
				int index2;
				cin >> index2;

				while (index2 > curUser->buyProductList().size() || index2 < 1 || cin.fail()) {
					cout << "输入有误!请重新输入:";
					cin.clear(); //清除std::cin的错误状态
					cin.ignore(1024, '\n');//清空输入缓冲区
					cin >> index2;
				}

				curUser->removeProduct(index2);
				cout << "删除成功！" << endl;
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
	cout << "请输入关键词:" << endl;
	string str;
	cin >> str;

	vector<GoodsInfoText::lineNo> locs = goods.runQueryWord(str);

	if (locs.size() == 0) {
		system("cls");
		cout << "没有找到相关商品" << endl;
		cout << "1.返回首页" << endl;
		int index;
		cin >> index;
		while (index != 1 || cin.fail())
		{
			cout << "输入错误,请重新输入:" << endl;
			cin.clear(); //清除std::cin的错误状态
			cin.ignore(1024, '\n');//清空输入缓冲区
			cin >> index;
		}
		return pair<string, GoodsInfoText::lineNo>();
	}
	else
		productList(locs);

	int goodsSelect;
	cin >> goodsSelect;
	while (goodsSelect > locs.size() || goodsSelect <= 0) {
		cout << "输入错误,请重新输入(1-" << locs.size() << " ):" << endl;
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
	cout << "一级商品类目:" << endl;
	cout << "1.服装" << endl
		<< "2.食品" << endl
		<< "3.图书" << endl;

	cout << "请选择类目(1-3):" << endl;
	int indexOne, indexTwo;
	cin >> indexOne;

	while (indexOne >= 4 || indexOne <= 0 || cin.fail()) {
		cout << "输入错误,请重新输入(1-3):" << endl;
		cin.clear(); //清除std::cin的错误状态
		cin.ignore(1024, '\n');//清空输入缓冲区
		cin >> indexOne;
	}
	switch (indexOne)
	{
	case 1:
		system("cls");
		cout << "二级商品类目:" << endl;
		cout << "1.裙子" << endl
			<< "2.裤子" << endl
			<< "3.上装" << endl
			<< "4.鞋子" << endl
			<< "5.帽子" << endl;
		cout << "请选择类目(1-5):" << endl;
		cin >> indexTwo;

		while (indexTwo >= 6 || indexOne <= 0 || cin.fail()) {
			cout << "输入错误,请重新输入(1-5):" << endl;
			cin.clear(); //清除std::cin的错误状态
			cin.ignore(1024, '\n');//清空输入缓冲区
			cin >> indexTwo;
		}
		break;
	case 2:
		system("cls");
		cout << "二级商品类目:" << endl;
		cout << "1.休闲零食" << endl
			<< "2.营养品" << endl
			<< "3.粮油米面" << endl
			<< "4.茶/饮料" << endl;
		cout << "请选择类目(1-4):" << endl;
		cin >> indexTwo;

		while (indexTwo >= 5 || indexOne <= 0 || cin.fail()) {
			cout << "输入错误,请重新输入(1-4):" << endl;
			cin.clear(); //清除std::cin的错误状态
			cin.ignore(1024, '\n');//清空输入缓冲区
			cin >> indexTwo;
		}
		break;
	case 3:
		system("cls");
		cout << "二级商品类目:" << endl;
		cout << "1.儿童书籍" << endl
			<< "2.教材" << endl
			<< "3.文学" << endl
			<< "4.计算机" << endl
			<< "5.社会科学" << endl;
		cout << "请选择类目(1-5):" << endl;
		cin >> indexTwo;

		while (indexTwo >= 6 || indexOne <= 0 || cin.fail()) {
			cout << "输入错误,请重新输入(1-5):" << endl;
			cin.clear(); //清除std::cin的错误状态
			cin.ignore(1024, '\n');//清空输入缓冲区
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
		cout << "输入错误,请重新输入(1-" << locs.size() << " ):" << endl;
		cin.clear(); //清除std::cin的错误状态
		cin.ignore(1024, '\n');//清空输入缓冲区
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
		cout << "discount.txt 打开失败!" << endl;
		system("pause");
		exit(-1);
	}

	string temp;
	double clothDiscout, foodDiscount, bookDiscount;
	double X, Y;//满X减Y

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
