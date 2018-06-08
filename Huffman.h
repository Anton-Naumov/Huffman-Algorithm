#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include "HTree.h"

class Huffman
{
public:
	Huffman();
	Huffman(const string &, bool); // ����� �� ����� � ������ �� ������ ����������� - 0 � ������������� - 1

	void compressInFile(const string &, bool); // ������ ����� �� ����� ������ �� ������ �������������� ���������� �� ������� ������� ����,
									           // ���� ������� ������� � ������� ����
	void decompressFile(const string &); // ������ ����� �� ����� ������ �� ������ ���������������� ���������� �� ������� ������� ����
	void loadFile(const string &, bool, const string = ""); // ������� ���� � ������ ������ �� ������,
															// ���� ��� ������������� ������ � ���� �������� �������
	bool getMode() const;
private:
	HTree tree;
	size_t dataSize;
	ifstream file; // ��������� ����
	bool mode; // ���� � � ����� �� ����������� - 0 ��� ������������� - 1

	uint8_t stringToByte(string); // ����� ������ �� 0-�� � 1-�� � ������� 8 � ����� �����, ������������ �� ���� ������
};

#endif
