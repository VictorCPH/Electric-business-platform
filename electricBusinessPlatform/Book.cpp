#include "stdafx.h"
#include "Book.h"

Book::Book(istringstream & line)
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

double Book::getPrice() const
{
	return getQuantity() * getUnitPrice() * discount;
}
void Book::description() const
{
	cout << "��Ʒ����: " << getName() << endl;
	cout << "ԭ��: " << getUnitPrice() << endl;
	cout << "�ۿۼ�: " << getUnitPrice() * getDiscount() << endl;
	cout << "�ۿ�ϵ��: " << getDiscount() << endl;
	cout << "���: " << getInventory() << endl;
	cout << "��Ʒ����: " << endl << getDetails() << endl;
}
double Book::getDiscount() const
{
	return discount;
}
void Book::setDiscount(double dis)
{
	discount = dis;
}