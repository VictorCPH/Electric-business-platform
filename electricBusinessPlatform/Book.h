#ifndef BOOK_H
#define BOOK_H

#include "stdafx.h"
#include "Product.h"
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

class Book : public Product
{
public:
	Book(istringstream& line);
	double getPrice() const;
	void description() const;
	double getDiscount() const;
	static void setDiscount(double dis);
private:
	static double discount;

};

#endif
