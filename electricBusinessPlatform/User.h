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
	void addProduct(Product * prod);//�����Ʒ�����ﳵ
	void removeProduct(int i);//�ӹ��ﳵ��ɾ����Ʒ
	bool payOff(double pay);//֧��
	void clearCart();//��չ��ﳵ
	double checkMyCart();//�鿴���ﳵ
	list<Product*> buyProductList();//�������Ʒ�б�
	void recordShopCart();//��¼���ﳵ��Ϣ
	void checkConsumptionRecord();//�鿴��ʷ���Ѽ�¼
	vector<string> getBankCards();//��ȡ�û�ȫ���󶨵����п�����
	string getUserName();//����û���
	string getPassword();//�������
	void modifyPassword(const string& password);//�޸�����
private:
	pair<string, string> bindBankCard();//�����п�������<���ţ�����>
	void recordConsumption();//�������ۼ�¼
	string userName;
	string password;
	ShoppingCart myCart;
	vector<string> bankCards;
};

#endif