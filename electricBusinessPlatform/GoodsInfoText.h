
#ifndef GOODSINFOTEXT_H
#define GOODSINFOTEXT_H

#include "stdafx.h"
#include <map>
#include <utility>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Product.h"

using namespace std;

class GoodsInfoText
{
public:
	typedef string::size_type strSize;
	typedef vector<string>::size_type lineNo;

	void readFile();
	vector<lineNo> runQueryWord(const string&) const;
	vector<lineNo> runQueryId(const string&) const;
	lineNo getProductIndex(string& id);
	string textLine(lineNo) const;
	string getName(const string&) const;
	string getId(const string&) const;
	void updateProductInfo(Product * prod);
	void storeFile(ifstream &);
	void saveFile() const;
private:
	void buildWordMap();
	void buildTypeMap();
	void buildIdMap();
	vector<string> linesOfText;
	map< string, vector<lineNo> > wordMap;
	map< string, vector<lineNo> > typeMap;
	map< string, lineNo > idMap;
};

#endif
