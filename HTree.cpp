#include "HTree.h"

unsigned int HTree::Node::maxId = 0;

HTree::Node::Node(int _count, Node *_left, Node *_right, char _symbol)
		: count(_count), left(_left), right(_right), symbol(_symbol), id(maxId++)
{
}

HTree::HTree() : root(nullptr)
{
}

HTree::HTree(ifstream &file)
{
	loadFile(file);
}

HTree::~HTree()
{
	if (root)
		del(root);
}

void HTree::loadFile(ifstream &file)
{
	del(root); // ако преди това е бил зареден файл, трие предишното дърво

	map<char, unsigned int> frequencyTable; // на всеки символ съпоставя брой срещания
	char next;
	file.seekg(0);
	while (file && file.peek() != -1)
	{
		next = file.get();
		(frequencyTable.count(next) > 0) ? frequencyTable[next]++ : frequencyTable[next] = 1;
	}

	vector<Node *> nodes; // в началото слагам всички листа на дървото и сортирам по брой срещания, 
						  // за да взимам винаги първите два (тези с най-малък брой срещания) при изграждане на дървото
	for (pair<char, unsigned int> letter : frequencyTable)
		nodes.push_back(new Node(letter.second, nullptr, nullptr, letter.first));

	std::sort(nodes.begin(), nodes.end(), [](Node *a, Node *b) {return a->count < b->count;});

	while (nodes.size() > 1) // на всяка итерация взима първите два и ги изтрива от вектора като прави нов Node oт тях,
						     // който поставя на позиция обратно във вектора така че да остане сортиран
	{
		Node *left = nodes[0];
		Node *right = nodes[1];
		nodes.erase(nodes.begin(), nodes.begin() + 2);
		Node *newNode = new Node(left->count + right->count, left, right);
		int i = 0;
		while (i < nodes.size() && nodes[i]->count < newNode->count)
			i++;
		nodes.insert(nodes.begin() + i, newNode);
	}
	root = nodes[0]; // накрая има само един Node, който е корена на крайното дърво
}

void HTree::printTreeDotty(ostream &os)
{
	os << "digraph G {\n";
	printTreeDotty(os, root);
	os << "}";
}

void HTree::printTreeDotty(ostream &os, Node *curr)
{
	if (!curr)
		return;

	if (curr->symbol != '\0')
		os << curr->id << "[label=\"" << curr->symbol << " - " << curr->count << "\"];\n";
	else
		os << curr->id << "[label=\"" << curr->count << "\"];\n";

	if (curr->left)
		os << curr->id << "->" << curr->left->id << "\n";
	if (curr->right)
		os << curr->id << "->" << curr->right->id << "\n";

	printTreeDotty(os, curr->left);
	printTreeDotty(os, curr->right);
}

void HTree::del(Node *curr)
{
	if (!curr)
		return;
	del(curr->left);
	del(curr->right);
	delete curr;
}

map<char, string> HTree::codingTable() 
{
	// обхожда със стек дървото и мап-ва за всеки символ поредицата от битове която му съответства
	map<char, string> result;
	stack<pair<Node *, string>> s;

	if (root)
		s.push(std::make_pair(root, ""));
	while (!s.empty())
	{
		pair<Node *, string> curr = s.top();
		s.pop();

		if (!curr.first->left && !curr.first->right)
			result[curr.first->symbol] = curr.second;

		if (curr.first->right)
			s.push(std::make_pair(curr.first->right, curr.second + '1'));
		if (curr.first->left)
			s.push(std::make_pair(curr.first->left, curr.second + '0'));
	}

	return  result;
}

void HTree::serialize(ofstream &os)
{
	serialize(os, root);
}

void HTree::serialize(ofstream &os, Node *curr)
{
	if (curr == nullptr)
	{
		os << "()";
		return;
	}
	os << "( " << curr->count << " " << curr->symbol;
	serialize(os, curr->left);
	serialize(os, curr->right);
	os << ")";
}

void HTree::deserialize(ifstream &is)
{
	del(root);
	deserialize(is, root);
}

void HTree::deserialize(ifstream &is, Node *&curr)
{
	unsigned int count;
	char next, symbol;
	next = is.get();
	assert(next == '(');
	next = is.get();
	if (next == ')')
	{
		curr = nullptr;
		return;
	}
	assert(next == ' ');
	is >> count;
	next = is.get();
	assert(next == ' ');
	symbol = is.get();
	Node *left, *right;
	deserialize(is, left);
	deserialize(is, right);
	next = is.get();
	assert(next == ')');
	curr = new Node(count, left, right, symbol);
}

void HTree::decompressInFile(const string &fileName, unsigned int dataSize, ifstream &file)
{
	ofstream outFile(fileName, std::ios::binary);
	string text = "";
	file.seekg(0);
	Node *currNode = root;
	int lettersRead = 0;
	while (file) // минава през целия файл компресирана информация и на всеки байт съпоставя съотвената поредицата от битове
	{
		text = byteToString(file.get());
		for (size_t i = 0; i < text.size(); i++) // обхожда дървото и всеки път като стигне листо (което значи че е декодиран символ),
											    //  го записва във файла и започва да търси следващия символ от корена
		{
			if (lettersRead < dataSize) // заради последния байт, да не продължава да обхожда след като достигне броя символи
			{
				if (currNode->left == nullptr && currNode->right == nullptr)
				{
					outFile << currNode->symbol;
					currNode = root;
					lettersRead++;
				}
				if (text[i] == '0')
					currNode = currNode->left;
				if (text[i] == '1')
					currNode = currNode->right;
			}
		}
	}
}

string HTree::byteToString(uint8_t data)
{
	std::bitset<8> result(data);
	return result.to_string();
}
