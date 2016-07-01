#include "stdafx.h"
#include "Cloth.h"


Cloth::Cloth(istringstream & line)
{
	string id, name;
	double unitPrice;
	int inventory;
	line >> id >> name >> unitPrice >> inventory;

	string details, tmp;
	while (line >> tmp)
	{
		details += tmp;
		details += "\t";
	}
	setId(id);
	setName(name);
	setUnitPrice(unitPrice);
	setInventory(inventory);
	setDetails(details);
}

double Cloth::getPrice() const
{
	return getQuantity() * getUnitPrice() * discount;
}
void Cloth::description() const
{
	cout << "商品名称: " << getName() << endl;
	cout << "原价: " << getUnitPrice() << endl;
	cout << "折扣价: " << getUnitPrice() * getDiscount() << endl;
	cout << "折扣系数: " << getDiscount() << endl;
	cout << "库存: " << getInventory() << endl;
	cout << "商品详情: " << endl << getDetails() << endl;
}
double Cloth::getDiscount() const
{
	return discount;
}
void Cloth::setDiscount(double dis)
{
	discount = dis;
}

