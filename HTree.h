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
		unsigned int count; // ���� �������� �� ������� ��� �����
		char symbol; // � '\0' ��� Node-� �� � ����� � ���� ������ 
		Node *left, *right;
		Node(int, Node * = nullptr, Node * = nullptr, char = '\0');

		static unsigned int maxId; // �� ���������� �� ������� �� Dotty
		const unsigned int id; // �� ���������� �� ������� �� Dotty
	};

	HTree();
	HTree(ifstream &);
	~HTree();

	void loadFile(ifstream &); // ������ ���� � ��������� ������� �� ����
	void printTreeDotty(ostream &);
	map<char, string> codingTable(); // �� ���������� �����
	void serialize(ofstream &); // ����������� ������� ��� ����
	void deserialize(ifstream &); // ������������� ������� �� ����
	void decompressInFile(const string &, unsigned int, ifstream &); // ������ ��� �� ���� ������ �� ������ ���������������� ����������, 
																	// ���� ������� ����� ������ �� ��� � ��������� ���� � ���� � �������������� ����������
private:
	Node *root; // ����� �� �������

	void serialize(ofstream &, Node *);
	void deserialize(ifstream &, Node *&);
	void printTreeDotty(ostream &, Node *);
	string byteToString(uint8_t); // ����� ���� ���� � ����� ������ �� 0-�� � 1-�� � ������� 8, ��������� �� ���� ����
	void del(Node *); // �������� ������� ������� �� �������
};

#endif