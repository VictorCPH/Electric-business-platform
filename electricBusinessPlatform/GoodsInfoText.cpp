#include "stdafx.h"
#include "GoodsInfoText.h"


void GoodsInfoText::readFile()
{
	ifstream inGoods("goodsData.txt");

	if (!inGoods) {
		cout << "goodsData.txt 打开失败!" << endl;
		system("pause");
		exit(-1);
	}

	storeFile(inGoods);
	buildWordMap();
	buildTypeMap();
	buildIdMap();
}

void GoodsInfoText::storeFile(ifstream &is)
{
	string textline;

	while (getline(is, textline)) {
		linesOfText.push_back(textline);
		//	cout << textline;
	}
}

void GoodsInfoText::saveFile() const
{
	ofstream out("goodsData.txt");
	if (!out) {
		cout << "goodsData.txt 打开失败!" << endl;
		system("pause");
		exit(-1);
	}

	for (int i = 0; i != linesOfText.size(); ++i)
		out << linesOfText[i] << endl;

}

void GoodsInfoText::buildWordMap()
{
	for (lineNo lineNum = 0; lineNum != linesOfText.size(); ++lineNum)
	{
		istringstream line(linesOfText[lineNum]);
		//	cout << linesOfText[lineNum] << endl;
		string word;
		while (line >> word)
		{
			if (wordMap.count(word) == 0)
			{
				wordMap[word].push_back(lineNum);
				//	cout << lineNum<<endl;
			}
			else
			{
				if (lineNum != wordMap[word].back())
				{
					wordMap[word].push_back(lineNum);
					//	cout << lineNum<<endl;
				}
			}
		}
	}
}

void GoodsInfoText::buildTypeMap()
{
	for (lineNo lineNum = 0; lineNum != linesOfText.size(); ++lineNum)
	{
		istringstream line(linesOfText[lineNum]);
		//	cout << linesOfText[lineNum] << endl;
		string id;
		line >> id;
		
		string idOneToTwo(id.begin(), id.begin() + 2);

		typeMap[idOneToTwo].push_back(lineNum);
	}
}

void GoodsInfoText::buildIdMap()
{
	for (lineNo lineNum = 0; lineNum != linesOfText.size(); ++lineNum) {
		istringstream line(linesOfText[lineNum]);
		//	cout << linesOfText[lineNum] << endl;
		string id;
		line >> id;

		idMap[id] = lineNum;

	}
}

vector<GoodsInfoText::lineNo> GoodsInfoText::runQueryWord(const string& queryWord) const
{
	map<string, vector<lineNo> >::const_iterator loc = wordMap.find(queryWord);

	if (loc == wordMap.end())
		return vector<lineNo>();
	else
		return loc->second;
}

vector<GoodsInfoText::lineNo> GoodsInfoText::runQueryId(const string& queryId) const
{
	map<string, vector<lineNo> >::const_iterator loc = typeMap.find(queryId);

	if (loc == typeMap.end())
		return vector<lineNo>();
	else
		return loc->second;
}

GoodsInfoText::lineNo GoodsInfoText::getProductIndex(string & id) 
{
	return idMap[id];
}

string GoodsInfoText::textLine(lineNo line) const
{
	if (line < linesOfText.size())
		return linesOfText[line];
	throw out_of_range("Line number out of range");
}

string GoodsInfoText::getName(const string&line) const
{
	istringstream in(line);
	string name;

	in >> name;
	in >> name;
	return name;
}

string GoodsInfoText::getId(const string&line) const
{
	istringstream in(line);
	string id;

	in >> id;
	return id;
}

void GoodsInfoText::updateProductInfo(Product * prod)
{
	lineNo lineNum = idMap[prod->getId()];

	string prodStr, unitPrice, inventory;

	stringstream ss1, ss2;
	ss1 << prod->getUnitPrice();
	ss2 << prod->getInventory();
	unitPrice = ss1.str();
	inventory = ss2.str();

	prodStr += prod->getId() + " " + prod->getName() + " "
		+ unitPrice + " " + inventory + " " + prod->getDetails();

	linesOfText[lineNum] = prodStr;

}
