#include "compressor.h"

using namespace std;

void getchars(ifstream& fin, vector<long>& charcounts) {
	unsigned char temp;
	while (fin.read((char*)&temp, sizeof(temp)))
		++charcounts[temp];
}

huffmantree::huffmantree(const vector<long>& charcount) {
	tree.resize(0);
	for (int i = 0; i < charcount.size(); ++i)
		if (charcount[i] != 0)
			tree.push_back({ char(i), charcount[i], -1, -1, -1, none });
	int leefsum = tree.size();
	int current{ leefsum };
	set<int> deleted_number;
	int k;
	for (k = 0; k < leefsum - 1; ++k) {
		long min1, min2;
		findmin(*this, deleted_number, min1, min2);
		tree.push_back({ char(0), tree[min1].weight + tree[min2].weight, -1, min1, min2, none });
		tree[min1].parent = tree[min2].parent = current++;
		tree[min1].dir = direction::left;
		tree[min2].dir = direction::right;
	}
}

void huffmantree::findmin(huffmantree& htree, set<int>& deleted_number, long& min1, long& min2) {
	int j;
	int size = htree.tree.size();
	for (j = 0; j < size; ++j)
		if (deleted_number.count(j) == 0) {
			min1 = j;
			break;
		}
	for (int i = min1; i < size; ++i)
		if (deleted_number.count(i) == 0 && htree[i].weight < htree[min1].weight)
			min1 = i;
	deleted_number.insert(min1);
	for (j = 0; j < size; ++j)
		if (deleted_number.count(j) == 0) {
			min2 = j;
			break;
		}
	for (int i = min2; i < size; ++i)
		if (deleted_number.count(i) == 0 && htree[i].weight < htree[min2].weight)
			min2 = i;
	deleted_number.insert(min2);
	/*cout << "min1 : " << deleted_number.count(min1) << " "
		<< "min2 : " << deleted_number.count(min2) << endl;*/
}

void huffmantree::show() {
	cout << "num   "
		<< "char  "
		<< "weights  "
		<< "parent  "
		<< "lchild  "
		<< "rchild  "
		<< "direction"
		<< endl;
	for (int i = 0; i < tree.size(); ++i)
		cout << i << "\t"
		<< tree[i].ch << "\t"
		<< tree[i].weight << "\t"
		<< tree[i].parent << "\t"
		<< tree[i].lchild << "\t"
		<< tree[i].rchild << "\t"
		<< tree[i].dir << "\t"
		<< endl;
}

void init_table(huffmantree& htree, vector<huffmancode>& table) {
	int leefsum = htree.tree.size() / 2 + 1;
	for (int i = 0; i < leefsum; ++i) {
		stack<bool> bitstr;
		int current{ i };
		huffmancode temp;
		while (htree[current].parent != -1) {
			bitstr.push(htree[current].dir);
			current = htree[current].parent;
		}
		int count{ 0 };
		while (!bitstr.empty()) {
			bool bit = bitstr.top();
			bitstr.pop();
			int index{ count / 64 }, offset{ count % 64 };
			if (bit)
				temp.code[index] |= (0x1 << offset);
			else
				temp.code[index] &= ~(0x1 << offset);
			++count;
		}
		temp.bits = count;
		table[htree[i].ch] = temp;
	}
}

unsigned long long get_allbits(ifstream& fin, const vector<huffmancode>& table) {
	unsigned long long total{ 0 };
	unsigned char temp;
	while (fin.read((char*)&temp, sizeof(temp)))
		total += table[temp].bits;
	return total;
}

void write2wzz(std::ifstream& fin, std::ofstream& fout, unsigned long long allbits, const std::vector<huffmancode>& table) {
	unsigned char ch{ 0 }, ch2wzz{ 0 };
	int ch2wzz_cur{ allbits % 8 == 0 ? 0 : 8 - allbits % 8 };//现在写入文件的字符下一次写在的位数
	while (fin.read((char*)&ch, sizeof(ch))) {
		int table_cur{ 0 };//现在写的哈夫曼码在的位置
		while (table_cur < table[ch].bits) {
			int index{ table_cur / 64 }, offset{ table_cur % 64 };
			huffmancode temp = table[ch];
			if ((temp.code[index] >> offset) & 0x1)
				ch2wzz |= (0x1 << ch2wzz_cur);
			else
				ch2wzz &= ~(0x1 << ch2wzz_cur);
			++ch2wzz_cur;
			++table_cur;
			if (ch2wzz_cur == 8) {
				fout.write((char*)&ch2wzz, sizeof(ch2wzz));
				ch2wzz = 0;
				ch2wzz_cur = 0;
			}
		}
	}
}

string get_filename(string filename) {
	for (int i = 0; i < filename.size(); ++i)
		if (filename[i] == '.')
			return filename.substr(0, i);
}

string get_fileformat(string filename) {
	for (int i = filename.size() - 1; i >= 0; --i)
		if (filename[i] == '.')
			return filename.substr(i + 1, filename.size() - i);
}

void compress(string file) {
	ifstream fin(file.c_str(), ios::binary);
	if (!fin.is_open()) {
		cout << "Error : File open failed !" << endl;
		return;
	}
	vector<long> charcounts(256);
	getchars(fin, charcounts);//统计各个字符的出现频次
	fin.close();
	huffmantree htree(charcounts);
	vector<huffmancode> table(256);
	init_table(htree, table);//创建哈夫曼编码表
	fin.open(file.c_str(), ios::binary);
	unsigned long long allbits = get_allbits(fin, table);//获得压缩后的总位数
	fin.close();
	fin.open(file.c_str(), ios::binary);

	//写入文件：

	string filename{ get_filename(file) };
	string fileformat{ get_fileformat(file) };
	ofstream fout((filename + ".wzz").c_str(), ios::binary);
	const char* filename2 = filename.c_str();
	const char* fileformat2 = fileformat.c_str();
	int length1 = strlen(filename2) + 1, length2 = strlen(fileformat2) + 1;
	fout.write((char*)&length1, sizeof(length1));//写入文件名大小
	fout.write(filename2, length1);//写入文件名
	fout.write((char*)&length2, sizeof(length2));//写入文件后缀大小
	fout.write(fileformat2, length2);//写入文件后缀名
	int size{ htree.getsize() };
	vector<simplified_node> simplified_tree(size);
	for (int i = 0; i < simplified_tree.size(); ++i)
		simplified_tree[i] = simplified_node(htree.getnode(i));
	fout.write((char*)&size, sizeof(size));//写入简化哈夫曼树大小
	for (int i = 0; i < size; ++i) {
		fout.write((char*)&simplified_tree[i].ch, sizeof(unsigned char));
		fout.write((char*)&simplified_tree[i].lchild, sizeof(int));
		fout.write((char*)&simplified_tree[i].rchild, sizeof(int));
	}//写入简化哈夫曼树
	fout.write((char*)&allbits, sizeof(allbits));//写入总的压缩文件位数
	write2wzz(fin, fout, allbits, table);//写入转换的哈夫曼编码

	fout.close();
	fin.close();
}

void writefromwzz(ifstream& fin, ofstream& fout, unsigned long long allbits, const vector<simplified_node>& simplified_tree) {
	unsigned char ch{ 0 };
	int ch_cur{ allbits % 8 == 0 ? 0 : 8 - allbits % 8 };
	int curnode = simplified_tree.size() - 1;
	fin.read((char*)&ch, sizeof(ch));
	while (true) {
		if (ch >> ch_cur & 0x1)
			curnode = simplified_tree[curnode].rchild;
		else
			curnode = simplified_tree[curnode].lchild;
		if (simplified_tree[curnode].lchild == -1 || simplified_tree[curnode].rchild == -1) {
			unsigned char temp{ simplified_tree[curnode].ch };
			fout.write((char*)&temp, sizeof(temp));
			curnode = simplified_tree.size() - 1;
		}
		++ch_cur;
		if (ch_cur == 8) {
			ch_cur = 0;
			if (fin.read((char*)&ch, sizeof(ch)));
			else
				return;
		}
	}
}

void decompress(string file) {
	ifstream fin(file.c_str(), ios::binary);
	if (!fin.is_open()) {
		cout << "Error : File open failed !" << endl;
		return;
	}
	//读文件：
	string filename, fileformat;
	int length1, length2;
	fin.read((char*)&length1, sizeof(int));//读出文件名大小
	char* filename2 = new char[length1];
	fin.read(filename2, length1 * sizeof(char));//读出文件名
	filename = filename2;
	fin.read((char*)&length2, sizeof(int));//读出文件后缀大小
	char* fileformat2 = new char[length2];
	fin.read(fileformat2, length2 * sizeof(char));//读出文件格式
	fileformat = fileformat2;
	ofstream fout((filename + "(1)." + fileformat).c_str(), ios::binary);
	int size;
	fin.read((char*)&size, sizeof(size));//读出简化哈夫曼树大小
	vector<simplified_node> simplified_tree(size);
	for (int i = 0; i < size; ++i) {
		fin.read((char*)&simplified_tree[i].ch, sizeof(unsigned char));
		fin.read((char*)&simplified_tree[i].lchild, sizeof(int));
		fin.read((char*)&simplified_tree[i].rchild, sizeof(int));
	}//读出简化哈夫曼树
	unsigned long long allbits;
	fin.read((char*)&allbits, sizeof(allbits));//读出压缩文件位数；
	writefromwzz(fin, fout, allbits, simplified_tree);//写入解压缩文件

	fin.close();
	fout.close();
}