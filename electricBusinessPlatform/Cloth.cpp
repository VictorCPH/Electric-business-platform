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
	cout << "��Ʒ����: " << getName() << endl;
	cout << "ԭ��: " << getUnitPrice() << endl;
	cout << "�ۿۼ�: " << getUnitPrice() * getDiscount() << endl;
	cout << "�ۿ�ϵ��: " << getDiscount() << endl;
	cout << "���: " << getInventory() << endl;
	cout << "��Ʒ����: " << endl << getDetails() << endl;
}
double Cloth::getDiscount() const
{
	return discount;
}
void Cloth::setDiscount(double dis)
{
	discount = dis;
}

