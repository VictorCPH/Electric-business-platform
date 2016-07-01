#include "stdafx.h"
#include "Product.h"

string Product::getId() const
{
	return id;
}

string Product::getName() const
{
	return name;
}

double Product::getUnitPrice() const
{
	return unitPrice;
}

int Product::getQuantity() const
{
	return quantity;
}

int Product::getInventory() const
{
	return inventory;
}

string Product::getDetails() const
{
	return details;
}

void Product::setId(string id)
{
	this->id = id;
}

void Product::setName(string name)
{
	this->name = name;
}

void Product::setUnitPrice(double unitPrice)
{
	this->unitPrice = unitPrice;
}

void Product::setQuantity(int quan)
{
	quantity = quan;
}

void Product::setInventory(int inven)
{
	inventory = inven;
}

void Product::setDetails(string details)
{
	this->details = details;
}





