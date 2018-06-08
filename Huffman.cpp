#include "Huffman.h"

Huffman::Huffman()
{
}

Huffman::Huffman(const string &fileName, bool _mode)
{
	loadFile(fileName, _mode);
}

void Huffman::compressInFile(const string &outFileName, bool numbers) // numbers задава дали в компресирания файл да са числа от 0 до 255 или символи
{
	if (mode == 1) return; // не прави нищо при режим на декомпресация

	map<char, string> codingTable = tree.codingTable();
	ofstream outFile(outFileName, std::ios::binary);
	file.seekg(0);
	string tmp = ""; // стринг от битовете съответстващи на символите от файла
	while (file && file.peek() != -1) // минава през целия файл
	{
		tmp += codingTable[file.get()];
		if (tmp.size() > 8) // на всеки 8 събрани 0-ли и 1-ци прави от тях един байт и го записва във компресирания файл
		{
			if (numbers == false)
				outFile << stringToByte(tmp.substr(0, 8));
			else
				outFile << (int)stringToByte(tmp.substr(0, 8));
			tmp.erase(tmp.begin(), tmp.begin() + 8);
		}
	}
	if (tmp.size() != 0) // за последния байт, допълва до дължина 8 за да стане цял байт
	{
		while (tmp.size() < 8)
			tmp += '0';
		outFile << stringToByte(tmp);
	}
	// запазване на дървото в отделен файл
	string treeFileName = outFileName.substr(0, outFileName.size() - 4) + "_tree"
		+ outFileName.substr(outFileName.size() - 4);
	ofstream treeFile(treeFileName);
	treeFile << dataSize << " "; // преди дървото записваме и броят символи в текста,
								  // за да няма двусмилици на последния бит в компресирания файл
	tree.serialize(treeFile);
	cout << "\nFile created:" << treeFileName << "\n";
}

void Huffman::decompressFile(const string &decompressedFileName)
{
	if (mode == 0) return; // ако е в режим на компресация не прави нищо

	tree.decompressInFile(decompressedFileName, dataSize, file);
}

void Huffman::loadFile(const string &fileName, bool _mode, const string treeFileName)
{
	mode = _mode;
	file.close(); // ако преди това е имало отворен файл
	file.open(fileName, std::ios::binary | std::ios::ate);
	dataSize = file.tellg();
	file.seekg(0);
	if (mode == 0) // при режим на компресация зарежда дървото нормално, за да може да се построй codingTable-ла
	{
		tree.loadFile(file);
		return;
	}
	// при режим на декомпресация зарежда дървото от другия файл, за да може да го обхожда, за да намира символите
	ifstream readTreeFile(treeFileName);
	readTreeFile >> dataSize; // първо прочита броя символи, който трябва да бъде в декомпресирания файл
	readTreeFile.ignore(); // след това във файла следва дървото
	tree.deserialize(readTreeFile); // чете дървото от файла
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
