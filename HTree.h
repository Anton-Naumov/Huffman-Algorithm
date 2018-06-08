#ifndef __HTREE_H__
#define __HTREE_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <stack>
#include <algorithm>
#include <cassert>
#include <bitset>

using std::cin;
using std::cout;
using std::endl;
using std::ostream;
using std::ofstream;
using std::ifstream;
using std::vector;
using std::string;
using std::map;
using std::pair;
using std::stack;

class HTree
{
public:
	struct Node
	{
		unsigned int count; // брой срещания на символа във файла
		char symbol; // е '\0' ако Node-а не е листо и няма символ 
		Node *left, *right;
		Node(int, Node * = nullptr, Node * = nullptr, char = '\0');

		static unsigned int maxId; // за принтиране на дървото на Dotty
		const unsigned int id; // за принтиране на дървото на Dotty
	};

	HTree();
	HTree(ifstream &);
	~HTree();

	void loadFile(ifstream &); // приема файл и построява дървото от него
	void printTreeDotty(ostream &);
	map<char, string> codingTable(); // за зареденото дърво
	void serialize(ofstream &); // сериализира дървото във файл
	void deserialize(ifstream &); // десериализира дървото от файл
	void decompressInFile(const string &, unsigned int, ifstream &); // приема име на файл където да запише декомпресираната информация, 
																	// броя символи който трябва да има в получения файл и файл с компресираната информация
private:
	Node *root; // корен на дървото

	void serialize(ofstream &, Node *);
	void deserialize(ifstream &, Node *&);
	void printTreeDotty(ostream &, Node *);
	string byteToString(uint8_t); // взима един байт и връща стринг от 0-ли и 1-ци с дължина 8, отговарящ на този байт
	void del(Node *); // изчиства паметта заемана от дървото
};

#endif