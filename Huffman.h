#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include "HTree.h"

class Huffman
{
public:
	Huffman();
	Huffman(const string &, bool); // името на файла и режима на работа компресация - 0 и декомпресация - 1

	void compressInFile(const string &, bool); // приема името на файла където да запази компресираната информация на текущия зареден файл,
									           // също записва дървото в отделен файл
	void decompressFile(const string &); // приема името на файла където да запази декомпресираната информация на текущия зареден файл
	void loadFile(const string &, bool, const string = ""); // зарежда файл и задава режима на работа,
															// като при декомпресация приема и файл съдържащ дървото
	bool getMode() const;
private:
	HTree tree;
	size_t dataSize;
	ifstream file; // отворения файл
	bool mode; // дали е в режим на компресация - 0 или декомпресация - 1

	uint8_t stringToByte(string); // взима стринг от 0-ли и 1-ци с дължина 8 и връща байта, съответстващ на този стринг
};

#endif
