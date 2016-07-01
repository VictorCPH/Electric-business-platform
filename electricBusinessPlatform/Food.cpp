#include "stdafx.h"
#include "Food.h"

Food::Food(istringstream & line)
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

double Food::getPrice() const
{
	return getQuantity() * getUnitPrice() * discount;
}
double Food::getDiscount() const
{
	return discount;
}
void Food::description() const
{
	cout << "��Ʒ����: " << getName() << endl;
	cout << "ԭ��: " << getUnitPrice() << endl;
	cout << "�ۿۼ�: " << getUnitPrice() * getDiscount() << endl;
	cout << "�ۿ�ϵ��: " << getDiscount() << endl;
	cout << "���: " << getInventory() << endl;
	cout << "��Ʒ����: " << endl << getDetails() << endl;
}
void Food::setDiscount(double dis)
{
	discount = dis;
}
