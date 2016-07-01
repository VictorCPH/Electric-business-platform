
#ifndef SHOPPINGCART_H
#define SHOPPINGCART_H

#include "stdafx.h"
#include "Product.h"
#include <list>
#include <iostream>

using namespace std;

class  ShoppingCart
{
public:
	void show(ostream& out) const;
	void addProduct(Product *);
	void removeProduct(int);
	void clear();//支付后清空购物车
	double total() const;
	list<Product *> productList;
	pair<double, double> discount;//满X减Y
};

#endif;
