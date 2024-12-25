#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <string.h>
#include <cstring>
#include "sha1.hpp"
#include "bigint_function_definitions.h"
#include "BTree.h"
#include <direct.h>
#include <cstdio>
using namespace std;

const bigint MAX_KEYS = 4;
const bigint MAX_CHILDREN = 5;

bigint hexToBigInt(const std::string& hex) {

	bigint decimal("0");
	for (int i = 0; i < hex.length(); ++i) {
		if (hex[i] >= '0' && hex[i] <= '9') {

			int asci = (int)hex[i];
			bigint asci2(asci);
			bigint asci4(48);
			bigint no10("10");
			bigint no16("16");
			decimal = decimal * no16 + (asci2 - asci4);
		}
		else if (hex[i] >= 'A' && hex[i] <= 'F') {

			int asci = (int)hex[i];
			int asci3 = (int)"A";
			bigint asci2(asci);
			bigint asci4(65);
			bigint no10("10");
			bigint no16("16");

			decimal = (decimal * no16) + ((asci2 - asci4) + no10);
		}
		else if (hex[i] >= 'a' && hex[i] <= 'f') {

			int asci = (int)hex[i];
			bigint asci2(asci);
			bigint asci4(97);
			bigint no10("10");
			bigint no16("16");
			decimal = decimal * no16 + (asci2 - asci4 + no10);
		}
		else {
			return -1;
		}
	}
	return decimal;
}

class DoublyLinkedList;
class Node;
class DadNode;

class CircularLinkedList;

class Dadnode {
public:
	bigint id;
	Node* node;
	Dadnode* next;
	Dadnode* prev;


	Dadnode(bigint x = 0, Node* p = nullptr) : next(NULL), prev(NULL), node(p) {
		id = x;
	}
};

class Node {
public:
	bigint data;
	bigint max;
	KeyNodeList* keys;
	BTree* dataTree;
	bool isActive;
	DoublyLinkedList* FT;
	Node* next;

	Node() {
		max = 0;
		data = 0;
		next = NULL;
		keys = new KeyNodeList;
		FT = nullptr;
		isActive = true;
		dataTree = new BTree;
	}

	Node(bigint d, bigint idSpace) {
		max = d;
		data = d;
		keys = new KeyNodeList;
		next = NULL;
		FT = nullptr;
		isActive = true;
		dataTree = new BTree;
	}

};

class DoublyLinkedList {
public:
	Dadnode* head;

	DoublyLinkedList() : head(NULL) {

	}

	Dadnode* getHead() {
		return head;
	}

	void insert(bigint x, Node* p) {
		Dadnode* newNode = new Dadnode(x, p);
		if (head == NULL) {
			head = newNode;
			return;
		}

		Dadnode* temp1 = head;
		Dadnode* temp2 = NULL;

		while (temp1 != NULL) {
			temp2 = temp1;
			temp1 = temp1->next;
		}

		newNode->prev = temp2;
		temp2->next = newNode;
		newNode->next = NULL;
	}

	bool isEmpty() {
		if (head == NULL) {
			return true;
		}
		return false;
	}

	Dadnode* search(bigint x) {
		Dadnode* temp1 = head;
		while (temp1 != NULL) {
			if (temp1->id == x) {
				return temp1;
			}
			temp1 = temp1->next;
		}
		return nullptr;
	}

	void print() {
		Dadnode* temp1 = head;
		if (head == NULL) {
			cout << "List empty" << endl;
			return;
		}

		while (temp1 != NULL) {
			cout << temp1->id << " " << temp1->node->data << endl;
			temp1 = temp1->next;
		}
	}
};

class CircularLinkedList {
public:
	Node* head;
	Node* tail;

	CircularLinkedList() : head(NULL), tail(NULL) {

	}

	void insert(bigint x, bigint y) {
		Node* newNode = new Node(x, y);

		if (head == NULL) {
			head = newNode;
			head->next = head;
			return;
		}

		Node* temp1 = head;

		while (temp1->next != head) {
			temp1 = temp1->next;
		}
		temp1->next = newNode;
		newNode->next = head;
		tail = newNode;
	}

	bool search(bigint x) {
		Node* temp1 = head;
		while (temp1->next != head) {
			if (temp1->data == x) {
				return true;
			}
			temp1 = temp1->next;
		}
		return false;
	}

	int searchIndex(bigint x) {
		int index = 0;

		Node* temp1 = head;
		while (temp1->next != head) {
			if (temp1->data == x) {
				return index;
			}
			temp1 = temp1->next;
			index += 1;
		}
	}

	Node* getHead() {
		return head;
	}

	bool isEmpty() {
		if (head == NULL) {
			return true;
		}
		return false;
	}

	void insertUsingId(bigint x, bigint idSpace) {
		Node* newNode = new Node(x, idSpace);
		if (head == NULL) {
			head = newNode;
			head->next = head;
			return;
		}

		Node* p = head;	//current
		Node* q = p;	//prev

		if (x < p->max) {
			newNode->next = p;
			while (p->next != head) {
				p = p->next;
			}
			p->next = newNode;
			head = newNode;
			return;
		}

		p = p->next;
		while (p != head) {
			if (q->max < x && x < p->max) {
				break;
			}
			q = p;
			p = p->next;
		}

		newNode->next = p;
		q->next = newNode;
		this->setKeys(idSpace);
		this->initializeFT(idSpace);

		newNode->dataTree = p->dataTree->extract(newNode->keys);
		return;
		//here also set keys list
		//then extract btree from successor of current node and place in current node
		//based on keys of current node and delete those from successor node's tree

	}

	void deleteUsingId(bigint x, bigint idSpace) {
		Node* p = head;	//current
		Node* q = p;	//prev

		while (q->data != x) {
			q = p;
			p = p->next;
		}

		q->dataTree->merge(p->dataTree);

		p = head;
		q = p;
		while (p->data != x) {
			q = p;
			p = p->next;
		}

		q->next = p->next;
		delete p;
		this->setKeys(idSpace);
		this->initializeFT(idSpace);
	}

	void print()
	{
		if (head == nullptr) {
			cout << "list empty" << endl;
			return;
		}
		Node* p = head;
		cout << p->data << " ";
		p = p->next;
		while (p != head) {
			cout << p->data << " ";
			p = p->next;
		}
	}

	void printKeys() {
		if (head == nullptr) {
			cout << "list empty" << endl;
			return;
		}
		Node* p = head;
		cout << "Keys of machine " << p->data << " : " << endl;
		p->keys->print();
		p = p->next;
		while (p != head) {
			cout << "Keys of machine " << p->data << " : " << endl;
			p->keys->print();
			p = p->next;
		}
	}

	void setKeys(bigint idSpace) {
		Node* p = head;
		Node* q = p;
		bigint min, max;
		bigint x1 = 2;
		p = p->next;
		while (p != head) {
			min = q->data + bigint(1);
			max = p->data;
			while (max >= min) {
				p->keys->insert(min);
				min++;
			}
			q = p;
			p = p->next;
		}

		//For Head Node
		min = q->data + bigint(1);
		bigint x = bigint::_big_pow(x1,idSpace);
		x = x - min;
		max = p->data;
		while (bigint(0) < x) {
			p->keys->insert(min);
			min++;
			x--;
		}

		while (max >= x) {
			p->keys->insert(x);
			x++;
		}
	}

	Node*& closestActiveNode(Node* p, bigint x) {
		bigint temp = x;
		Node* p1 = head;
		Node* p2 = head;
		if (p1->max >= temp) {
			return p1;
		}
		else {
			p1 = p1->next;
			while (p1 != head) {
				if (p2->data < temp && p1->data >= temp) {
					break;
				}
				p2 = p1;
				p1 = p1->next;
			}
		}
		return p1;
	}

	Node* returnNode(bigint x) {
		Node* p = head;
		if (p->data == x) {
			return p;
		}

		p = p->next;
		while (p != head) {
			if (p->data == x) {
				return p;
			}
			p = p->next;
		}

		return nullptr;
	}

	void initializeFT(bigint y) {
		int i = 0;
		Node* p = head;
		bigint x = 2;
		bigint divider = bigint::_big_pow(x,y);
		p->FT = new DoublyLinkedList;
		for (bigint i = 0; i < y; i++) {
			bigint z = (p->max + bigint::_big_pow(x,i));
			bigint z1 = z%divider;
			Node* temp = closestActiveNode(p, z1);
			p->FT->insert(i + bigint(1), temp);
		}

		p = p->next;
		while (p != head) {
			p->FT = new DoublyLinkedList;
			for (bigint i = 0; i < y; i++) {
				bigint z = (p->max + bigint::_big_pow(x, i));
				bigint z1 = z % divider;
				Node* temp = closestActiveNode(p, z1);
				p->FT->insert(i + bigint(1), temp);
			}
			p = p->next;
		}
	}

	void displayFT() {
		Node* p = head;
		cout << "Table of " << p->data << ":" << endl;
		p->FT->print();
		cout << endl;
		p = p->next;
		while (p != head)
		{
			cout << "Table of " << p->data << ":" << endl;
			p->FT->print();
			cout << endl;
			p = p->next;
		}
	}

	void displayBTree() {
		Node* p = head;
		cout << "BTree of " << p->data << ":" << endl;
		p->dataTree->traverse();
		cout << endl;
		p = p->next;
		while (p != head)
		{
			cout << "BTree of " << p->data << ":" << endl;
			p->dataTree->traverse();
			cout << endl;
			p = p->next;
		}
	}

	Node*& searchKey(Node* p, bigint key, bigint identifier) {
		bool found = false;
		bool found1 = false;
		bool found2 = false;
		Node* temp = p;
		while (!found) {
			if (p->keys->search(key)) {
				cout << "Key Found in machine no " << p->data << endl;
				found = true;
				return p;
			}
			else if (p->max < key && p->FT->search(1)->node->max >= key) {
				cout << "Jumped from node " << p->data << " to ";
				p = p->FT->search(1)->node;
				cout << p->data << endl;
				found1 = true;
			}
			else {
				bigint j = 0;
				for (bigint i = 1; i < identifier; i++) {
					j = i;
					if (p->FT->search(i)->node->data < key && p->FT->search(i + bigint(1))->node->max >= key) {
						cout << "Jumped from node " << p->data << " to ";
						p = p->FT->search(i)->node;
						cout << p->data << endl;
						found2 = true;
						break;
					}
				}
			}

			if (!found && !found1 && !found2) {
				cout << "Jumped from node " << p->data << " to ";
				p = p->next;
				cout << p->data << endl;
			}

		}
	}

	void insertData(CircularLinkedList& list,string x, string y, bigint key, bigint idSpace) {
		bigint x1;
		cout << "Enter machine to start from: " << endl;
		cin >> x1;
		Node* temp1 = list.returnNode(x1);
		Node* temp = searchKey(temp1, key, idSpace);
		temp->dataTree->insertion(y, x, key);
	}

	void deleteDataFromBTree(CircularLinkedList& list,bigint key, string hashKey, bigint idSpace) {
		bigint x;
		cout << "Enter machine to start from: " << endl;
		cin >> x;
		Node* temp1 = list.returnNode(x);
		Node* temp = searchKey(temp1, key, idSpace);
		temp->dataTree->deletion(hashKey);
	}

	string ReadFile(string fileName) {
		fstream file(fileName);
		if (!file.is_open()) {
			cout << "Error: Could Not Open File" << endl;
		}

		string retString;
		getline(file, retString);
		file.close();
		return retString;
	}

	void displayFTUsingID(bigint key) {
		Node* temp = head;
		while (temp->data != key) {
			temp = temp->next;
		}
		temp->FT->print();
	}

	void displayBTreeUsingID(bigint key) {
		Node* temp = head;
		while (temp->data != key) {
			temp = temp->next;
		}
		temp->dataTree->traverse();
	}

	void writeFile(string fileName1,string retString) {
		string fileName = "E:/Project4/Project4/Data_Storage_Folder/";
		fileName = fileName + fileName1;
		fstream file;
		file.open(fileName,ios::out);
		if (!file.is_open()) {
			cout << "Error: Could Not Open File" << endl;
		}
		file << retString;
		file.close();
	}

	void deleteFile(string fileName1) {
		string fileName = "E:/Project4/Project4/Data_Storage_Folder/";
		fileName = fileName + fileName1;
		const int length = fileName.length();

		// declaring character array (+1 for null terminator) 
		char* fileName2 = new char[length + 1];

		for (int i = 0; i < length; i++) {
			fileName2[i] = fileName[i];
		}

		int result = remove(fileName2);
	}

};

void manuallyAssignIds(CircularLinkedList& list, int num, int identifier) {
	bigint x1 = 2;
	bigint y = identifier;
	bigint no_nodes = bigint::_big_pow(x1, y);
	bigint x;
	for (bigint i = 0; i < bigint(num); i++) {
		cout << "Enter Node Ids In Ascending Order (0 - " << no_nodes << "): " << endl;
		cin >> x;
		list.insert(x, identifier);
	}
	list.print();
	cout << endl;
	list.setKeys(identifier);
	list.initializeFT(identifier);
}

void automaticallyAssignIds(CircularLinkedList& list, int num, int identifier) {
	bigint x = 2;
	bigint y = identifier;
	bigint no_nodes = bigint::_big_pow(x,y);
	bigint choose;
	bigint* prevChoose = new bigint[num];
	for (int i = 0; i < num; i++) {
		prevChoose[i] = -1;
	}
	int i = 0;
	bigint loopCounter = 0;
	bool check = true;
	bigint* machineId = new bigint[num];
	while (loopCounter < bigint(num)) {
		long long int var1 = pow(2, identifier);
		long long int var2 = rand() % var1;
		choose = var2;
		for (int j = 0; j < i; j++) {
			if (choose == prevChoose[j]) {
				check = false;
				break;
			}
			check = true;
		}
		if (check) {
			machineId[i] = choose;
			prevChoose[i] = choose;
			i++;
			loopCounter++;
		}
		else {
			continue;
		}
	}

	for (int i = 0; i < num - 1; i++) {
		for (int j = 0; j < num - i - 1; j++) {
			if (machineId[j + 1] < machineId[j]) {
				swap(machineId[j], machineId[j + 1]);
			}
		}
	}
	cout << endl;
	for (int i = 0; i < num; i++) {
		list.insert(machineId[i], identifier);
	}
	list.setKeys(identifier);
	list.initializeFT(identifier);
}

void insertDataToSystem(CircularLinkedList& list, int identifier) {
	int input;
	char ch = 'y';
	cout << "1. Manually Insert File Name and Content " << endl;
	cout << "2. Automatically Insert File using File Name " << endl;
	cin >> input;
	if (input == 1) {
		while (ch != 'n') {
			string content, fileName, hash;
			bigint machineKey;
			cin.ignore();
			cout << "Enter file name: " << endl;
			getline(cin, fileName);
			cout << "Enter content of file: " << endl;
			getline(cin, content);
			SHA1 contentHash;
			contentHash.update(content);
			hash = contentHash.final();
			machineKey = hexToBigInt(hash);
			machineKey = machineKey % bigint(identifier);
			list.insertData(list,fileName, hash, machineKey, bigint(identifier));
			list.writeFile(fileName, content);
			cout << "Do you want to continue inserting data? (y/n)" << endl;
			cin >> ch;
		}
	}
	else if (input == 2) {
		int noOfFiles;
		string fileNames[20] = { "File1.txt", "File2.txt", "File3.txt", "File4.txt", "File5.txt",
								 "File6.txt", "File7.txt", "File8.txt", "File9.txt", "File10.txt",
								 "File11.txt", "File12.txt", "File13.txt", "File14.txt", "File15.txt",
								 "File16.txt", "File17.txt", "File18.txt", "File19.txt", "File20.txt"};
		cout << "Enter how many files you want to insert: " << endl;
		cin >> noOfFiles;

		for (int i = 0; i < noOfFiles; i++) {
			string content, hash;
			content = list.ReadFile(fileNames[i]);
			SHA1 contentHash;
			contentHash.update(content);
			hash = contentHash.final();
			bigint machineKey;
			machineKey = hexToBigInt(hash);
			machineKey = machineKey % bigint(identifier);
			list.insertData(list,fileNames[i], hash, machineKey, bigint(identifier));
			list.writeFile(fileNames[i], content);
		}
	}
}

void deleteDataFromSystem(CircularLinkedList& list, int identifier) {
	int input;
	char ch = 'y';
	cout << "1. Manually Delete by Entering File Name and Content " << endl;
	cout << "2. Automatically Insert File using File Name " << endl;
	cin >> input;
	if (input == 1) {
		while (ch != 'n') {
			string content, fileName, hash;
			bigint machineKey;
			cin.ignore();
			cout << "Enter file name: " << endl;
			getline(cin, fileName);
			cout << "Enter content of file: " << endl;
			getline(cin, content);
			SHA1 contentHash;
			contentHash.update(content);
			hash = contentHash.final();
			machineKey = hexToBigInt(hash);
			machineKey = machineKey % bigint(identifier);
			list.deleteDataFromBTree(list,machineKey, hash, bigint(identifier));
			list.deleteFile(fileName);
			cout << "Do you want to continue deleting data? (y/n)" << endl;
			cin >> ch;
		}
	}
	else if (input == 2) {
		int noOfFiles;
		string fileNames[20] = { "File1.txt", "File2.txt", "File3.txt", "File4.txt", "File5.txt",
								 "File6.txt", "File7.txt", "File8.txt", "File9.txt", "File10.txt",
								 "File11.txt", "File12.txt", "File13.txt", "File14.txt", "File15.txt",
								 "File16.txt", "File17.txt", "File18.txt", "File19.txt", "File20.txt" };
		
		cout << "Enter how many files you want to delete: " << endl;
		cin >> noOfFiles;

		for (int i = 0; i < noOfFiles; i++) {
			string content, hash;
			content = list.ReadFile(fileNames[i]);
			SHA1 contentHash;
			contentHash.update(content);
			hash = contentHash.final();
			bigint machineKey;
			machineKey = hexToBigInt(hash);
			machineKey = machineKey % bigint(identifier);
			list.deleteDataFromBTree(list,machineKey, hash, bigint(identifier));
			list.deleteFile(fileNames[i]);
		}
	}
}

void displayRoutingTable(CircularLinkedList& list) {
	int input;
	cout << "1. Display All Routing Tables " << endl;
	cout << "2. Display Specific Routing Table " << endl;
	cin >> input;

	if (input == 1) {
		list.displayFT();
	}
	else if (input == 2) {
		bigint machineKey;
		cout << "Enter machine no to display Routing Table: " << endl;
		cin >> machineKey;
		list.displayFTUsingID(machineKey);
	}
}

void displayBTree(CircularLinkedList& list) {
	int input;
	cout << "1. Display All B Trees " << endl;
	cout << "2. Display Specific B Tree " << endl;
	cin >> input;

	if (input == 1) {
		list.displayBTree();
	}
	else if (input == 2) {
		int machineKey;
		cout << "Enter machine no to display B Tree: " << endl;
		cin >> machineKey;
		list.displayBTreeUsingID(machineKey);
	}
}

void insertNewMachine(CircularLinkedList& list, int idSpace) {
	bigint x;
	bigint y = 2;
	bigint z = idSpace;
	cout << "Enter node number to activate between 0 and " << bigint::_big_pow(y,z) << " : " << endl;
	cin >> x;
	list.insertUsingId(x, bigint(idSpace));
}

void deleteMachineFromList(CircularLinkedList& list, int idSpace) {
	bigint x;
	bigint y = 2;
	bigint z = idSpace;
	cout << "Enter node number to deactivate between 0 and " << bigint::_big_pow(y, z) << " : " << endl;
	cin >> x;
	list.deleteUsingId(x, bigint(idSpace));
}

int main()
{
	CircularLinkedList list;
	if (_mkdir("E:/Disk D Backup/221304_D-221184_D/Project4/Project4/Data_Storage_Folder") == -1)
		cerr << " Error : "<<endl;
	else {

	}
	srand(200);
	int identifier;
	int no_nodes;
	int num;

	cout << "---------------------Welcome To InterPlanetary File System---------------------" << endl;
	cout << "Enter the identifier space of the system: " << endl;
	cin >> identifier;

	cout << "Enter total no of active machines in the system: " << endl;
	cin >> num;

	int input = 0;
	cout << "Enter key 1 or 2 to proceed: " << endl;
	cout << "1. Manually Assign IDs" << endl;
	cout << "2. Automatically Assign IDs" << endl;

	char ch = 'y';
	while (ch != 'n') {
		cin >> input;
		switch (input) {
		case 1:
			manuallyAssignIds(list, num, identifier);
			ch = 'n';
			break;
		case 2:
			automaticallyAssignIds(list, num, identifier);
			ch = 'n';
			break;
		default:
			cout << "Invalid Input" << endl;
			cout << "Try Again " << endl;
			ch = 'y';
			break;
		}
	}

	cout << "List: " << endl;
	list.print();
	ch = 'Y';
	while (ch != 'n') {
		cout << "Enter key between 1-6 to proceed." << endl;
		cout << "1. Insert Data into System" << endl;
		cout << "2. Delete Data from System" << endl;
		cout << "3. Display Routing Table" << endl;
		cout << "4. Display B-Tree" << endl;
		cout << "5. Add New Machine" << endl;
		cout << "6. Delete Machine" << endl;

		cin >> input;
		switch (input) {
		case 1:
			insertDataToSystem(list, identifier);
			break;

		case 2:
			deleteDataFromSystem(list, identifier);
			break;

		case 3:
			displayRoutingTable(list);
			break;

		case 4:
			displayBTree(list);
			break;
		case 5:
			insertNewMachine(list, identifier);
			break;
		case 6:
			deleteMachineFromList(list, identifier);
			break;
		default:
			cout << "Invalid Input" << endl;
			break;
		}
		cout << "Do you want to continue? " << endl;
		cin >> ch;
	}

	return 0;
}
