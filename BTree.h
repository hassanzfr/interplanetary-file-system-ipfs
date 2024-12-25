#define BTREE_H
#include <iostream>
#include <string.h>
#include <string>
#include <cstring>
#include "bigint_function_definitions.h"
using namespace std;

class KeyNode {
public:
	bigint key;
	KeyNode* nextKeyNode;
	KeyNode* prevKeyNode;

	KeyNode(bigint a = -1) : nextKeyNode(nullptr), prevKeyNode(nullptr) {
		key = a;
	}
};

class KeyNodeList {
public:
	KeyNode* head;

	KeyNodeList() : head(nullptr) {

	}

	void insert(bigint x) {
		KeyNode* newNode = new KeyNode(x);
		if (head == NULL) {
			head = newNode;
			return;
		}

		KeyNode* temp1 = head;
		KeyNode* temp2 = NULL;

		while (temp1 != NULL) {
			temp2 = temp1;
			temp1 = temp1->nextKeyNode;
		}

		newNode->prevKeyNode = temp2;
		temp2->nextKeyNode = newNode;
		newNode->nextKeyNode = NULL;
	}

	bool isEmpty() {
		if (head == NULL) {
			return true;
		}
		return false;
	}

	void print() {
		KeyNode* temp1 = head;
		if (head == NULL) {
			cout << "List empty" << endl;
			return;
		}

		while (temp1 != NULL) {
			cout << temp1->key << endl;
			temp1 = temp1->nextKeyNode;
		}
	}

	bool search(bigint x) {
		KeyNode* temp1 = head;
		while (temp1 != NULL) {
			if (temp1->key == x) {
				return true;
			}
			temp1 = temp1->nextKeyNode;
		}
		return false;
	}

};

class BTree;

struct keyNode {

	string key;
	string data;
	bigint machineKey;

	keyNode(string a, string d, int m) : key(a), data(d), machineKey(m) {}

	keyNode() : key("0"), data(""), machineKey(0) {}

	bool operator<(string& x) {
		return(key < x);
	}

	bool operator>(string& x) {
		return(key > x);
	}

	bool operator>(keyNode& x) {
		return(key > x.key);
	}

	bool operator<(keyNode& x) {
		return(key < x.key);
	}

	bool operator==(keyNode& x) {
		return(key == x.key);
	}

	bool operator==(string& x) {
		return(key == x);
	}

	bool operator<=(string& x) {
		return (key <= x);
	}

	bool operator>=(string& x) {
		return (key >= x);
	}

	bool operator>=(keyNode& x) {
		return (key >= x.key);
	}

	bool operator<=(keyNode& x) {
		return (key <= x.key);
	}

	void operator=(keyNode& x) {

		key = x.key;
		data = x.data;
		machineKey = x.machineKey;

	}
};

class BTreeNode {
public:
	keyNode* keys;
	int t;
	BTreeNode** C;
	int n;
	bool leaf;

	BTreeNode(int _t, bool _leaf);
	void traverse();
	string search(bigint k);
	void extractRecursive(KeyNodeList* l, BTree*& newTree);
	int findKey(string k);
	void insertNonFull(string k, string d, bigint m);
	void splitChild(int i, BTreeNode* y);
	void deletion(string k);
	void removeFromLeaf(int idx);
	void removeFromNonLeaf(int idx);
	keyNode getPredecessor(int idx);
	keyNode getSuccessor(int idx);
	void fill(int idx);
	void borrowFromPrev(int idx);
	void borrowFromNext(int idx);
	void merge(int idx);
	void mergeRecursive(BTree* Tree);
	friend class BTree;
};

class BTree {

public:
	BTreeNode* root;
	int t;

	BTree(int _t = 3) {
		root = NULL;
		t = _t;
	}

	void traverse() {
		if (root != NULL)
			root->traverse();
	}

	string search(bigint k) {
		if (root != NULL)
			return root->search(k);
	}

	BTree* extract(KeyNodeList* l);

	void insertion(string k, string data, bigint machineKey);

	void deletion(string k);

	void merge(BTree* treeToBeMerged);
};

void BTreeNode::extractRecursive(KeyNodeList* l, BTree*& newTree) {
	if (this == nullptr) {
		return;
	}
	int i;
	for (i = 0; i < n; ++i) {
		if (!leaf) {
			C[i]->extractRecursive(l, newTree);
		}

		cout << " " << keys[i].key << "(" << keys[i].data << ", " << keys[i].machineKey << ")" << endl;

		if (l->search(keys[i].machineKey)) {
			newTree->insertion(keys[i].key, keys[i].data, keys[i].machineKey);
			this->deletion(keys[i].key);
			--i; // Adjust i to handle the deleted key
		}
	}

	if (!leaf) {
		C[i]->extractRecursive(l, newTree);
	}
}

BTree* BTree::extract(KeyNodeList* l) {
	BTree* newTree = new BTree;

	root->extractRecursive(l, newTree);

	return newTree;
}

BTreeNode::BTreeNode(int t1, bool leaf1) {
	t = t1;
	leaf = leaf1;

	keys = new keyNode[2 * t - 1];
	C = new BTreeNode * [2 * t];

	n = 0;
}

int BTreeNode::findKey(string k) {
	int idx = 0;
	while (idx < n && keys[idx] < k)
		++idx;
	return idx;
}

void BTreeNode::deletion(string k) {
	int idx = findKey(k);

	if (idx < n && keys[idx] == k) {
		if (leaf)
			removeFromLeaf(idx);
		else
			removeFromNonLeaf(idx);
	}
	else {
		if (leaf) {
			cout << "The key " << k << " is does not exist in the tree\n";
			return;
		}

		bool flag = ((idx == n) ? true : false);

		if (C[idx]->n < t)
			fill(idx);

		if (flag && idx > n)
			C[idx - 1]->deletion(k);
		else
			C[idx]->deletion(k);
	}
	return;
}

void BTreeNode::removeFromLeaf(int idx) {
	for (int i = idx + 1; i < n; ++i)
		keys[i - 1] = keys[i];

	n--;

	return;
}

void BTreeNode::removeFromNonLeaf(int idx) {

	keyNode k = keys[idx];

	if (C[idx]->n >= t) {
		keyNode pred = getPredecessor(idx);
		keys[idx] = pred;
		C[idx]->deletion(pred.key);
	}

	else if (C[idx + 1]->n >= t) {
		keyNode succ = getSuccessor(idx);
		keys[idx] = succ;
		C[idx + 1]->deletion(succ.key);
	}

	else {
		merge(idx);
		C[idx]->deletion(k.key);
	}
	return;
}

keyNode BTreeNode::getPredecessor(int idx) {
	BTreeNode* cur = C[idx];
	while (!cur->leaf)
		cur = cur->C[cur->n];

	return cur->keys[cur->n - 1];
}

keyNode BTreeNode::getSuccessor(int idx) {
	BTreeNode* cur = C[idx + 1];
	while (!cur->leaf)
		cur = cur->C[0];

	return cur->keys[0];
}

void BTreeNode::fill(int idx) {
	if (idx != 0 && C[idx - 1]->n >= t)
		borrowFromPrev(idx);

	else if (idx != n && C[idx + 1]->n >= t)
		borrowFromNext(idx);

	else {
		if (idx != n)
			merge(idx);
		else
			merge(idx - 1);
	}
	return;
}

void BTreeNode::borrowFromPrev(int idx) {
	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx - 1];

	for (int i = child->n - 1; i >= 0; --i)
		child->keys[i + 1] = child->keys[i];

	if (!child->leaf) {
		for (int i = child->n; i >= 0; --i)
			child->C[i + 1] = child->C[i];
	}

	child->keys[0] = keys[idx - 1];

	if (!child->leaf)
		child->C[0] = sibling->C[sibling->n];

	keys[idx - 1] = sibling->keys[sibling->n - 1];

	child->n += 1;
	sibling->n -= 1;

	return;
}

void BTreeNode::borrowFromNext(int idx) {
	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx + 1];

	child->keys[(child->n)] = keys[idx];

	if (!(child->leaf))
		child->C[(child->n) + 1] = sibling->C[0];

	keys[idx] = sibling->keys[0];

	for (int i = 1; i < sibling->n; ++i)
		sibling->keys[i - 1] = sibling->keys[i];

	if (!sibling->leaf) {
		for (int i = 1; i <= sibling->n; ++i)
			sibling->C[i - 1] = sibling->C[i];
	}

	child->n += 1;
	sibling->n -= 1;

	return;
}

void BTreeNode::merge(int idx) {
	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx + 1];

	child->keys[t - 1] = keys[idx];

	for (int i = 0; i < sibling->n; ++i)
		child->keys[i + t] = sibling->keys[i];

	if (!child->leaf) {
		for (int i = 0; i <= sibling->n; ++i)
			child->C[i + t] = sibling->C[i];
	}

	for (int i = idx + 1; i < n; ++i)
		keys[i - 1] = keys[i];

	for (int i = idx + 2; i <= n; ++i)
		C[i - 1] = C[i];

	child->n += sibling->n + 1;
	n--;

	delete (sibling);
	return;
}

void BTree::insertion(string k, string dat, bigint machineK) {
	if (root == NULL) {
		root = new BTreeNode(t, true);
		root->keys[0].key = k;
		root->keys[0].data = dat;
		root->keys[0].machineKey = machineK;
		root->n = 1;
	}
	else {
		if (root->n == 2 * t - 1) {
			BTreeNode* s = new BTreeNode(t, false);

			s->C[0] = root;

			s->splitChild(0, root);

			int i = 0;
			if (s->keys[0] < k)
				i++;
			s->C[i]->insertNonFull(k, dat, machineK);

			root = s;
		}
		else
			root->insertNonFull(k, dat, machineK);
	}
}

void BTreeNode::insertNonFull(string k, string d, bigint m) {
	int i = n - 1;

	if (leaf == true) {
		while (i >= 0 && keys[i] > k) {
			keys[i + 1] = keys[i];
			i--;
		}

		keys[i + 1].key = k;
		keys[i + 1].data = d;
		keys[i + 1].machineKey = m;
		n = n + 1;
	}
	else {
		while (i >= 0 && keys[i] > k)
			i--;

		if (C[i + 1]->n == 2 * t - 1) {
			splitChild(i + 1, C[i + 1]);

			if (keys[i + 1] < k)
				i++;
		}
		C[i + 1]->insertNonFull(k, d, m);
	}
}

void BTreeNode::splitChild(int i, BTreeNode* y) {
	BTreeNode* z = new BTreeNode(y->t, y->leaf);
	z->n = t - 1;

	for (int j = 0; j < t - 1; j++)
		z->keys[j] = y->keys[j + t];

	if (y->leaf == false) {
		for (int j = 0; j < t; j++)
			z->C[j] = y->C[j + t];
	}

	y->n = t - 1;

	for (int j = n; j >= i + 1; j--)
		C[j + 1] = C[j];

	C[i + 1] = z;

	for (int j = n - 1; j >= i; j--)
		keys[j + 1] = keys[j];

	keys[i] = y->keys[t - 1];

	n = n + 1;
}

void BTreeNode::traverse() {
	int i;
	for (i = 0; i < n; i++) {
		if (leaf == false)
			C[i]->traverse();

		cout << " " << keys[i].key << "(" << keys[i].data << ", " << keys[i].machineKey << ")" << endl;
	}

	if (leaf == false)
		C[i]->traverse();
}

string BTreeNode::search(bigint k) {
	int i;
	for (i = 0; i < n; i++) {
		if (leaf == false)
			C[i]->search(k);
		if (keys[i].machineKey == k) {
			return keys[i].data;
		}
		
	}

	if (leaf == false)
		C[i]->traverse();
}

void BTree::deletion(string k) {
	if (!root) {
		cout << "The tree is empty\n";
		return;
	}

	root->deletion(k);

	if (root->n == 0) {
		BTreeNode* tmp = root;
		if (root->leaf)
			root = NULL;
		else
			root = root->C[0];

		delete tmp;
	}
	return;
}

void BTreeNode::mergeRecursive(BTree* Tree) {
	int i;
	for (i = 0; i < n; ++i) {
		if (!leaf) {
			C[i]->mergeRecursive(Tree);
		}

		Tree->insertion(keys[i].key, keys[i].data, keys[i].machineKey);

	}

	if (!leaf) {
		C[i]->mergeRecursive(Tree);
	}
}

void BTree::merge(BTree* treeToBeMergedOnto) {
	root->mergeRecursive(treeToBeMergedOnto);
}