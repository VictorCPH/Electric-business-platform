#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include "ShoppingCart.h"

using namespace std;

class User
{
public:
	User() {};
	User(const string& name, const string& pass, const vector<string>& bankCards, pair<double, double>& discount);
	void addProduct(Product * prod);//添加商品到购物车
	void removeProduct(int i);//从购物车中删除商品
	bool payOff(double pay);//支付
	void clearCart();//清空购物车
	double checkMyCart();//查看购物车
	list<Product*> buyProductList();//购买的商品列表
	void recordShopCart();//记录购物车信息
	void checkConsumptionRecord();//查看历史消费记录
	vector<string> getBankCards();//获取用户全部绑定的银行卡卡号
	string getUserName();//获得用户名
	string getPassword();//获得密码
	void modifyPassword(const string& password);//修改密码
private:
	pair<string, string> bindBankCard();//绑定银行卡，返回<卡号，密码>
	void recordConsumption();//保存销售记录
	string userName;
	string password;
	ShoppingCart myCart;
	vector<string> bankCards;
};

#endif