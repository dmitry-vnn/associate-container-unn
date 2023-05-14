#pragma once

#include <utility>
#include <string>
#include <algorithm>
#include <memory>
#include <stdexcept>

template<class T>
struct Node {

	T data;

	short depth = 1;
	size_t size = 1;

	Node* parent = nullptr;
	Node* leftChild = nullptr;
	Node* rightChild = nullptr;


public:

	explicit Node(T data = T()): data(data) {}

	void UpdateDepth() {
		depth = 1 + std::max(leftChild ? leftChild->depth : 0,
			rightChild ? rightChild->depth : 0);
	}

	void UpdateSize() {
		size = 1 + (leftChild ? leftChild->size : 0)
			+ (rightChild ? rightChild->size : 0);
	}

	short DepthsDifferent() {
		return (rightChild ? rightChild->depth : 0)
			- (leftChild ? leftChild->depth : 0);
	}
};


template<class T>
class Iterator {

private:
	using TypedNode = Node<T>;

public:
	TypedNode* _nodeCursor;

public:

	explicit Iterator(TypedNode* node = nullptr): _nodeCursor(node) {}

	Iterator(const Iterator& other) = default;
	Iterator& operator=(const Iterator& other) = default;

	bool operator==(const Iterator& other) const {
		return _nodeCursor == other._nodeCursor;
	}

	bool operator!=(const Iterator& other) const {
		return !operator==(other);
	}

	Iterator& operator++();

	T& operator*() const {
		return _nodeCursor->data;
	}

	T* operator->() const {
		return &_nodeCursor->data;
	}
};

template <typename T>
class Tree {

private:
	using Node = Node<T>;
	using Iterator = Iterator<T>;
	using ConstIterator = Iterator;

private:
	Node* _root;

public:

	Tree(): _root(new Node) { _root->size = 0; }

	~Tree();

	Tree(const Tree& other) = delete;
	Tree(Tree&& other) noexcept = delete;
	Tree& operator=(const Tree& other) = delete;
	Tree& operator=(Tree&& other) noexcept = delete;

public:

	Iterator Begin();
	ConstIterator Begin() const;

	Iterator End() { return Iterator(_root); }
	ConstIterator End() const { return ConstIterator(_root); }

	Iterator Find(const T& element);
	void Insert(const T& element);
	Iterator Erase(Iterator iterator);

	size_t Size() const { return _root->size; }


private:
	void RotateLeft(Node* node);
	void RotateRight(Node* node);

	void FreeChildren(Node* node);
};


template <class T>
Iterator<T>& Iterator<T>::operator++()
{
	if (_nodeCursor->rightChild != nullptr) {
		_nodeCursor = _nodeCursor->rightChild;
		while (_nodeCursor->leftChild != nullptr) {
			_nodeCursor = _nodeCursor->leftChild;
		}
	}
	else {
		TypedNode* before;
		do {
			before = _nodeCursor;
			_nodeCursor = _nodeCursor->parent;
		} while (_nodeCursor && before == _nodeCursor->rightChild);
	}
	return *this;
}

template <typename T>
Tree<T>::~Tree()
{
	FreeChildren(_root);
	delete _root;
}

template <typename T>
typename Tree<T>::Iterator Tree<T>::Begin()
{
	Node* node = _root;

	while (node->leftChild != nullptr) {
		node = node->leftChild;
	}

	return Iterator(node);
}

template <typename T>
typename Tree<T>::ConstIterator Tree<T>::Begin() const
{
	const Node* node = _root;

	while (node->leftChild != nullptr) {
		node = node->leftChild;
	}

	return ConstIterator(node);
}

template <typename T>
void Tree<T>::Insert(const T& element)
{

	// descent the search tree
	Node* parent = _root;
	while (true) {
		++parent->size;
		if (parent == _root || element < parent->data) {
			if (parent->leftChild) {
				parent = parent->leftChild;
			}
			else {
				parent->leftChild = new Node(element);
				parent->leftChild->parent = parent;
				break;
			}
		}
		else {
			if (parent->rightChild) {
				parent = parent->rightChild;
			}
			else {
				parent->rightChild = new Node(element);
				parent->rightChild->parent = parent;
				break;
			}
		}
	}

	short branchDepth = 1;
	do {
		if (parent->depth > branchDepth)
			break;
		parent->depth = 1 + branchDepth;
		if (parent == _root)
			break;
		if (parent->DepthsDifferent() < -1) {
			// check for double-rotation case
			if (parent->leftChild->DepthsDifferent() > 0) {
				RotateLeft(parent->leftChild);
			}
			RotateRight(parent);
			break;
		}
		if (parent->DepthsDifferent() > 1) {
			// check for double-rotation case
			if (parent->rightChild->DepthsDifferent() < 0) {
				RotateRight(parent->rightChild);
			}
			RotateLeft(parent);
			break;
		}
		branchDepth = parent->depth;
		parent = parent->parent;
	} while (parent);
}

template <typename T>
typename Tree<T>::Iterator Tree<T>::Erase(Iterator iterator)
{
	Iterator iteratorCopy(iterator);
	++iteratorCopy;

	Node* currentNode = iterator._nodeCursor;

	Node* q;
	if (currentNode->leftChild == nullptr 
		|| currentNode->rightChild == nullptr) {
		q = currentNode;
	}
	else {
		q = iteratorCopy._nodeCursor;
	}

	Node* s;
	if (q->leftChild != nullptr) {
		s = q->leftChild;
		q->leftChild = nullptr;
	}
	else {
		s = q->rightChild;
		q->rightChild = nullptr;
	}
	if (s != nullptr) {
		s->parent = q->parent;
	}
	if (q == q->parent->leftChild) {
		q->parent->leftChild = s;
	}
	else {
		q->parent->rightChild = s;
	}
	Node* qParent = q->parent;
	if (q != currentNode) {
		q->parent = currentNode->parent;
		if (q->parent->leftChild == currentNode) {
			q->parent->leftChild = q;
		}
		else {
			q->parent->rightChild = q;
		}
		q->leftChild = currentNode->leftChild;
		if (q->leftChild != nullptr)
			q->leftChild->parent = q;
		q->rightChild = currentNode->rightChild;
		if (q->rightChild != nullptr)
			q->rightChild->parent = q;
		q->size = currentNode->size;
		q->depth = currentNode->depth;
		currentNode->leftChild = nullptr;
		currentNode->rightChild = nullptr;
	}
	if (qParent == currentNode) {
		qParent = q;
	}
	Node* parent;
	for (parent = qParent; parent != nullptr; parent = parent->parent) {
		--parent->size;
	}
	for (parent = qParent; parent != nullptr; parent = parent->parent) {
		parent->UpdateDepth();
		if (parent == _root)
			break;
		if (parent->DepthsDifferent() < -1) {
			// check for double-rotation case
			if (parent->leftChild->DepthsDifferent() > 0) {
				RotateLeft(parent->leftChild);
			}
			RotateRight(parent);
			break;
		}
		if (parent->DepthsDifferent() > 1) {
			// check for double-rotation case
			if (parent->rightChild->DepthsDifferent() < 0) {
				RotateRight(parent->rightChild);
			}
			RotateLeft(parent);
			break;
		}
	}
	delete currentNode;
	return iteratorCopy;
}

template <typename T>
void Tree<T>::RotateLeft(Node* node)
{
	Node* tempNode = node->rightChild->leftChild;
	if (node == node->parent->leftChild) {
		node->parent->leftChild = node->rightChild;
	}
	else {
		node->parent->rightChild = node->rightChild;
	}
	node->rightChild->parent = node->parent;
	node->rightChild->leftChild = node;
	node->parent = node->rightChild;
	node->rightChild = tempNode;

	if (tempNode != nullptr)
		tempNode->parent = node;

	// update ns
	node->UpdateSize();
	node->parent->UpdateSize();

	// update depths
	do {
		node->UpdateDepth();
		node = node->parent;
	} while (node != nullptr);
}

template <typename T>
void Tree<T>::RotateRight(Node* node)
{
	Node* tempNode = node->leftChild->rightChild;
	if (node == node->parent->leftChild) {
		node->parent->leftChild = node->leftChild;
	}
	else {
		node->parent->rightChild = node->leftChild;
	}
	node->leftChild->parent = node->parent;
	node->leftChild->rightChild = node;
	node->parent = node->leftChild;
	node->leftChild = tempNode;
	if (tempNode != nullptr)
		tempNode->parent = node;

	// update ns
	node->UpdateSize();
	node->parent->UpdateSize();

	// update depths
	do {
		node->UpdateDepth();
		node = node->parent;
	} while (node != nullptr);
}

template <typename T>
void Tree<T>::FreeChildren(Node* node)
{
	if (node->leftChild != nullptr) {
		FreeChildren(node->leftChild);

		delete node->leftChild;

	}
	if (node->rightChild != nullptr) {
		FreeChildren(node->rightChild);

		delete node->rightChild;
	}
}

template <typename T>
typename Tree<T>::Iterator Tree<T>::Find(const T& element)
{
	Node* node = _root->leftChild;

	while (node != nullptr) {
		if (element == node->data) {
			return Iterator(node);
		}
		if (element < node->data) {
			node = node->leftChild;
		}
		else {
			node = node->rightChild;
		}
	}

	return End();
}
