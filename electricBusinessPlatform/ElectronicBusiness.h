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
	void initialization();//���̳�ʼ��
	void registerUser();//ע���û�
	void logInUser();//��½�û�
	Product* searchProduct();//������Ʒ
	void showProduct(Product* prod);//չʾĳ����Ʒ����ϸ��Ϣ
	void enterShopCart();//���빺�ﳵ
	void updateProduct(list<Product *> productList);//���²�Ʒ��Ϣ
	void saveFile();//����ȫ����Ʒ��Ϣ��ȫ���û���Ϣ
private:
	pair<string, GoodsInfoText::lineNo> keywordSearch();//ͨ���ؼ���������Ʒ���������з�����������Ʒ�ı��
	void productList(const vector<GoodsInfoText::lineNo> &locs);//������Ʒ�ı���б���ӡ����Ʒ���б�
	Product* instantiateGoods(const pair<string, GoodsInfoText::lineNo>& result);//������ƷIDʵ������Ʒ
	pair<string, GoodsInfoText::lineNo> catalogueSearch();//���ݷ���������Ʒ
	void readDiscount();//��ȡ�ۿ���Ϣ
	string intToString(int index);
	User* curUser;//��ǰ�û�
	pair<double, double> discount;//��X��Y
	GoodsInfoText goods;//ȫ����Ʒ��Ϣ
	UserInfoText users;//ȫ���û���Ϣ
};

#endif