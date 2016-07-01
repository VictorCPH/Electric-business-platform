#ifndef ELECTRICBUSINESS_H
#define ELECTRICBUSINESS_H
#include "GoodsInfoText.h"
#include "UserInfoText.h"
#include "User.h"
#include "Product.h"
#include "Cloth.h"
#include "Book.h"
#include "Food.h"


using namespace std;

class ElectronicBusiness
{
public:
	void initialization();//电商初始化
	void registerUser();//注册用户
	void logInUser();//登陆用户
	Product* searchProduct();//搜索商品
	void showProduct(Product* prod);//展示某件商品的详细信息
	void enterShopCart();//进入购物车
	void updateProduct(list<Product *> productList);//更新产品信息
	void saveFile();//保存全部商品信息，全部用户信息
private:
	pair<string, GoodsInfoText::lineNo> keywordSearch();//通过关键词搜索商品，返回所有符合条件的商品的标号
	void productList(const vector<GoodsInfoText::lineNo> &locs);//根据商品的标号列表，打印出商品名列表
	Product* instantiateGoods(const pair<string, GoodsInfoText::lineNo>& result);//根据商品ID实例化商品
	pair<string, GoodsInfoText::lineNo> catalogueSearch();//根据分类搜索商品
	void readDiscount();//读取折扣信息
	string intToString(int index);
	User* curUser;//当前用户
	pair<double, double> discount;//满X减Y
	GoodsInfoText goods;//全部商品信息
	UserInfoText users;//全部用户信息
};

#endif