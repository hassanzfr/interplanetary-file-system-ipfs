# Implementation of a Distributed File System: INFP

## Authors
- Hassan Zafar
- Mesam E Tamaar Khan 

## Overview
This project implements a distributed file system using various data structures including Circular Linked Lists, B-Trees, and custom nodes. The system is designed to efficiently manage and manipulate data across a distributed network.

## Data Structures

### CircularLinkedList
- **Node**
  - `data: string`
  - `keyList: KeyNodeList*`
  - `nextNode: Node*`
  - `prevNode: Node*`
  - `parent: Node*`

### BTree
- **Attributes**
  - `root: BTreeNode*`
  - `t: int`
- **Methods**
  - `BTree(int _t)`
  - `Traverse()`
  - `Extraction()`
  - `Insertion()`
  - `Deletion()`

### DadNode
- **Attributes**
  - `Node: Node*`
  - `Id: BigInt`
  - `Next: DadNode*`
  - `Prev: DadNode*`
- **Methods**
  - `DadNode(BigInt Node*)`

### KeyNode
- **Attributes**
  - `Key: BigInt`
  - `nextKeyNode: KeyNode*`
  - `prevKeyNode: KeyNode*`
- **Methods**
  - `KeyNode(BigInt a)`
  - `insert(BigInt x)`
  - `isEmpty(): bool`
  - `print()`
  - `search(BigInt x)`

### KeyNodeList
- **Attributes**
  - `Head: KeyNode*`
- **Methods**
  - `KeyNodeList()`
  - `insert(BigInt x)`
  - `isEmpty(): bool`
  - `print()`
  - `search(BigInt x)`

### BTreeNode
- **Attributes**
  - `T: int`
  - `C: BTreeNode[]`
  - `Keys: keyNode[]`
  - `n: int`
  - `leaf: bool`
- **Methods**
  - `BTreeNode(int _t, bool _leaf)`
  - `Traverse()`
  - `Extraction()`
  - `Insertion()`
  - `Deletion()`
  - `mergeRecursive()`

### Node
- **Attributes**
  - `data: String`
  - `keyList: KeyNodeList*`
  - `nextNode: Node*`
  - `prevNode: Node*`
- **Methods**
  - `Node(string d = "")`
  - `insert(BigInt x)`
  - `remove(BigInt x)`
  - `search(BigInt x)`
  - `getFirstKey()`
  - `merge(Node* nextNode)`
  - `split()`
  - `getParent()`
  - `setParent(Node* parent)`
  - `findSuccessor()`
  - `findPredecessor()`

## Contribution
- **BigInt, BTree**: Hassan Zafar
- **LinkedList, FT**: Mesam E Tamaar Khan
