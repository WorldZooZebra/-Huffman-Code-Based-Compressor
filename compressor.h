#pragma once
#pragma once
#include "MyWindow.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <set>
#include <stack>

enum direction { left, right, none };

struct node {
	unsigned char ch;
	long weight;
	int parent;
	int lchild;
	int rchild;
	direction dir;
	node() :ch(0), weight(0), parent(-1), lchild(-1), rchild(-1), dir(none) {}
	node(const char t_ch, const long t_weight, const int t_parent,
		const int t_lchild, const int t_rchild, const direction t_dir)
		:ch(t_ch), weight(t_weight), parent(t_parent), lchild(t_lchild), rchild(t_rchild), dir(t_dir) {}
};

struct simplified_node {
	unsigned char ch;
	int lchild;
	int rchild;
	simplified_node() :ch(0), lchild(-1), rchild(-1) {}
	simplified_node(const node& node) :
		ch(node.ch), lchild(node.lchild), rchild(node.rchild) {}
};

struct huffmancode {
	int64_t code[4] = { 0 };
	int bits{ 0 };
	huffmancode& operator=(const huffmancode& sourse) {
		for (int i = 0; i < 4; ++i)
			code[i] = sourse.code[i];
		bits = sourse.bits;
		return *this;
	}
};

class huffmantree {
private:
	std::vector<node> tree;
public:
	int getsize() const { return tree.size(); }
	node getnode(const int i) const { return tree[i]; }
	huffmantree(const std::vector<long>& charcount);
	node& operator[](const int index) { return tree[index]; }
	void findmin(huffmantree& htree, std::set<int>& deleted_number, long& min1, long& min2);
	void show();
	friend void init_table(huffmantree& htree, std::vector<huffmancode>& table);
};

void getchars(std::ifstream& fin, std::vector<long>& charcounts);
unsigned long long get_allbits(std::ifstream& fin, const std::vector<huffmancode>& table);
void write2wzz(std::ifstream& fin, std::ofstream& fout, unsigned long long allbits, const std::vector<huffmancode>& table);
std::string get_fileformat(std::string filename);
std::string get_filename(std::string filename);
void compress(std::string file);
void writefromwzz(std::ifstream& fin, std::ofstream& fout,
	unsigned long long allbits, const std::vector<simplified_node>& simplified_tree);
void decompress(std::string file);