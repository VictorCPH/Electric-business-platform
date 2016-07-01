#include "stdafx.h"
#include "ShoppingCart.h"

void ShoppingCart::show(ostream& out) const
{
	int i = 1;
	for (list<Product*>::const_iterator it = productList.begin(); it != productList.end(); ++it, ++i) {
		out << i << "." <<(*it)->getName() << "\t" << (*it)->getQuantity() << "\t"
			<< (*it)->getPrice() << "\n";
	}
	out << "�ϼ�: " << total() << endl;
	out << "�ۿ۲���: ��" << static_cast<int>(discount.first) << "��" << static_cast<int>(discount.second) << endl;
}

void ShoppingCart::addProduct(Product *prod)
{
	for (list<Product*>::const_iterator it = productList.begin(); it != productList.end(); ++it)
		if ((*it)->getId() == prod->getId())//����������Ʒ�Ѵ��ڣ����¹�������
		{
			(*it)->setQuantity((*it)->getQuantity() + prod->getQuantity());
			return;
		}

	productList.push_back(prod);//����������Ʒ�����ڣ����뵽��������
}

void ShoppingCart::removeProduct(int index)
{
	int i = 1;
	for (list<Product *>::iterator it = productList.begin(); it != productList.end(); ++it)
		if (i == index)
		{
			productList.erase(it);
			break;
		}
		else
			++i;
}

void ShoppingCart::clear()
{
	productList.clear();
}

double ShoppingCart::total() const
{
	double total = 0.0;

	for (list<Product*>::const_iterator it = productList.begin(); it != productList.end(); ++it) {
		total += (*it)->getPrice();
	}

	total -= static_cast<int>(total / discount.first) * discount.second;
	return total;
}
