#include "Huffman.h"

Huffman::Huffman()
{
}

Huffman::Huffman(const string &fileName, bool _mode)
{
	loadFile(fileName, _mode);
}

void Huffman::compressInFile(const string &outFileName, bool numbers) // numbers ������ ���� � ������������� ���� �� �� ����� �� 0 �� 255 ��� �������
{
	if (mode == 1) return; // �� ����� ���� ��� ����� �� �������������

	map<char, string> codingTable = tree.codingTable();
	ofstream outFile(outFileName, std::ios::binary);
	file.seekg(0);
	string tmp = ""; // ������ �� �������� ������������� �� ��������� �� �����
	while (file && file.peek() != -1) // ������ ���� ����� ����
	{
		tmp += codingTable[file.get()];
		if (tmp.size() > 8) // �� ����� 8 ������� 0-�� � 1-�� ����� �� ��� ���� ���� � �� ������� ��� ������������� ����
		{
			if (numbers == false)
				outFile << stringToByte(tmp.substr(0, 8));
			else
				outFile << (int)stringToByte(tmp.substr(0, 8));
			tmp.erase(tmp.begin(), tmp.begin() + 8);
		}
	}
	if (tmp.size() != 0) // �� ��������� ����, ������� �� ������� 8 �� �� ����� ��� ����
	{
		while (tmp.size() < 8)
			tmp += '0';
		outFile << stringToByte(tmp);
	}
	// ��������� �� ������� � ������� ����
	string treeFileName = outFileName.substr(0, outFileName.size() - 4) + "_tree"
		+ outFileName.substr(outFileName.size() - 4);
	ofstream treeFile(treeFileName);
	treeFile << dataSize << " "; // ����� ������� ��������� � ����� ������� � ������,
								  // �� �� ���� ���������� �� ��������� ��� � ������������� ����
	tree.serialize(treeFile);
	cout << "\nFile created:" << treeFileName << "\n";
}

void Huffman::decompressFile(const string &decompressedFileName)
{
	if (mode == 0) return; // ��� � � ����� �� ����������� �� ����� ����

	tree.decompressInFile(decompressedFileName, dataSize, file);
}

void Huffman::loadFile(const string &fileName, bool _mode, const string treeFileName)
{
	mode = _mode;
	file.close(); // ��� ����� ���� � ����� ������� ����
	file.open(fileName, std::ios::binary | std::ios::ate);
	dataSize = file.tellg();
	file.seekg(0);
	if (mode == 0) // ��� ����� �� ����������� ������� ������� ��������, �� �� ���� �� �� ������� codingTable-��
	{
		tree.loadFile(file);
		return;
	}
	// ��� ����� �� ������������� ������� ������� �� ������ ����, �� �� ���� �� �� �������, �� �� ������ ���������
	ifstream readTreeFile(treeFileName);
	readTreeFile >> dataSize; // ����� ������� ���� �������, ����� ������ �� ���� � ��������������� ����
	readTreeFile.ignore(); // ���� ���� ��� ����� ������ �������
	tree.deserialize(readTreeFile); // ���� ������� �� �����
}

bool Huffman::getMode() const
{
	return mode;
}

uint8_t Huffman::stringToByte(string str)
{
	uint8_t result = 0;
	while (str.size() > 0)
	{
		if (str[0] == '1')
			result += 1 << str.size() - 1;
		str.erase(str.begin());
	}

	return result;
}
