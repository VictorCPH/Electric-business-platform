#ifndef PRODUCT_H
#define PRODUCT_H
#include "stdafx.h"
#include <string>
#include <iostream>

using namespace std;

class Product
{
public:
	string getId() const;
	string getName() const;
	double getUnitPrice() const;
	int getQuantity() const;
	int getInventory() const;
	string getDetails() const;
	virtual double getDiscount() const = 0;
	void setId(string id);
	void setName(string name);
	void setUnitPrice(double unitPrice);
	void setQuantity(int quan);
	void setInventory(int inven);
	void setDetails(string details);
	virtual double getPrice() const = 0;
	virtual void description() const = 0;
	
private:
	string id;//产品编号
	string name;//产品名称
	double unitPrice;//单价
	int quantity;//购买数量
	int inventory;//库存
	string details;//产品详情
};

#endif
