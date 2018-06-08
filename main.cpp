#include "Huffman.h"

void openFile(string &fileOpened, bool &mode, Huffman &huffman)
{
	ifstream file;
	getline(cin, fileOpened);
	file.open(fileOpened);
	while (!file.is_open()) // проверява дали файла е коректен
	{
		file.close();
		cout << "Invalid file name!\n"
			<< "File name:";
		getline(cin, fileOpened);
		file.open(fileOpened);
	}

	cout << "0.Compress.\n"
		<< "1.Decompress.\n";
	cin >> mode;
	cin.ignore();
	if (mode == 1)
	{
		string newFileName = "";
		cout << "File with Huffman tree:";
		getline(cin, newFileName);
		file.open(newFileName);
		while (!file.is_open())
		{
			file.close();
			cout << "Invalid file name!\n"
				<< "File with coding table:";
			getline(cin, newFileName);
			file.open(newFileName);
		}
		cout << newFileName << endl;
		huffman.loadFile(fileOpened, mode, newFileName);
	}
	else huffman.loadFile(fileOpened, mode);
}

int main()
{
	bool on = true;
	string fileOpened = "";
	Huffman huffman;
	int action;
	bool mode;
	string newFileName;

	while (on)
	{
		if (fileOpened != "")
		{
			cout << "Loaded file: " << fileOpened << "\n\n";

			if (huffman.getMode() == 0) 
				cout << "1.Compress file.\n";
			else cout << "1.Decompress file.\n";
			cout << "2.Load different file\n"
				<< "3.Close file.\n"
				<< "4.Exit.\n\n";

			cout << "Action:";
			cin >> action;
			cin.ignore();
			switch (action)
			{
			case 1:
				cout << "Output file name:";
				getline(cin, newFileName);
				if (huffman.getMode() == 0)
					huffman.compressInFile(newFileName, false);
				else huffman.decompressFile(newFileName);
				cout << "\nFile created:" << newFileName << "\n"
					<< "\nPress any key to continue.";
				cin.get();
				break;
			case 2:
				cout << "File name:";
				openFile(fileOpened, mode, huffman);
				break;
			case 3:
				fileOpened = "";
				break;
			case 4:
				on = false;
				break;
			}	
		}
		else
		{
			cout << "File to open:";
			openFile(fileOpened, mode, huffman);
		}
		system("cls");
	}
	system("pause");
	return 0;
}