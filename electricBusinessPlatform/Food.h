#ifndef FOOD_H
#define FOOD_H

#include "stdafx.h"
#include "Product.h"
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

class Food : public Product
{
public:
	Food(istringstream& line);
	double getPrice() const;
	double getDiscount() const;
	void description() const;
	static void setDiscount(double dis);
private:
	static double discount;

};

#endif