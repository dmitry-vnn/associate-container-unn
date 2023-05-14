#pragma once

#include <utility>
#include <string>
#include <algorithm>
#include <memory>
#include <iterator>
#include <stdexcept>

template<class T>
class Node {

public:
	T data;

	short depth;
	size_t n;

	Node* parent = nullptr;
	Node* leftChild = nullptr;
	Node* rightChild = nullptr;

	Node(): depth(1), n(1) {}

	explicit Node(const T& t) noexcept
		: data(t)
		, depth(1)
		, n(1)
		, parent(nullptr)
		, leftChild(nullptr)
		, rightChild(nullptr) {}

	explicit Node(T&& t) noexcept
		: data(std::move(t))
		, depth(1)
		, n(1)
		, parent(nullptr)
		, leftChild(nullptr)
		, rightChild(nullptr) {}


	void UpdateDepth() {
		depth = 1 + std::max(leftChild ? leftChild->depth : 0,
			rightChild ? rightChild->depth : 0);
	}

	void UpdateN() {
		n = 1 + (leftChild ? leftChild->n : 0)
			+ (rightChild ? rightChild->n : 0);
	}

	short Imbalance() {
		return (rightChild ? rightChild->depth : 0)
			- (leftChild ? leftChild->depth : 0);
	}
};




template<class T>
class Iterator {

	//template <typename U, typename V>
	//friend class tree::const_iterator;

	using TypedNode = Node<T>;
public:

	typedef std::bidirectional_iterator_tag iterator_category;

	Iterator() {
		_nodeCursor = nullptr;
	}

	explicit Iterator(TypedNode* p) {
		_nodeCursor = p;
	}

	Iterator(const Iterator& it) {
		_nodeCursor = it._nodeCursor;
	}

	Iterator& operator=(const Iterator& it) {
		_nodeCursor = it._nodeCursor;
		return *this;
	}

	bool operator==(const Iterator& it) const {
		return _nodeCursor == it._nodeCursor;
	}

	bool operator!=(const Iterator& it) const {
		return _nodeCursor != it._nodeCursor;
	}

	bool operator<(const Iterator& it) const {
		return **this < *it;
	}

	bool operator>(const Iterator& it) const {
		return **this > *it;
	}

	bool operator<=(const Iterator& it) const {
		return **this <= *it;
	}

	bool operator>=(const Iterator& it) const {
		return **this >= *it;
	}

	// pre-increment
	Iterator& operator++() {
		if (_nodeCursor->rightChild) {
			_nodeCursor = _nodeCursor->rightChild;
			while (_nodeCursor->leftChild) {
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

	// post-increment
	Iterator operator++(int) {
		Iterator old(*this);
		++(*this);
		return old;
	}

	// pre-decrement
	Iterator& operator--() {
		if (_nodeCursor->leftChild) {
			_nodeCursor = _nodeCursor->leftChild;
			while (_nodeCursor->rightChild) {
				_nodeCursor = _nodeCursor->rightChild;
			}
		}
		else {
			TypedNode* before;
			do {
				before = _nodeCursor;
				_nodeCursor = _nodeCursor->parent;
			} while (_nodeCursor && before == _nodeCursor->leftChild);
		}
		return *this;
	}

	// post-decrement
	Iterator operator--(int) {
		Iterator old(*this);
		--* this;
		return old;
	}

	T& operator*() const {
		return _nodeCursor->data;
	}

	T* operator->() const {
		return &_nodeCursor->data;
	}
public:
	TypedNode* _nodeCursor;
};





template<class T>
class ConstIterator {
public:

	typedef std::bidirectional_iterator_tag iterator_category;

	using TypedNode = Node<T>;
	using Iterator = Iterator<T>;

	ConstIterator() {
		_nodeCursor = nullptr;
	}

	ConstIterator(const TypedNode* p) {
		_nodeCursor = p;
	}

	ConstIterator(const ConstIterator& it) {
		_nodeCursor = it._nodeCursor;
	}

	ConstIterator(const Iterator& it) {
		_nodeCursor = it._nodeCursor;
	}

	ConstIterator& operator=(const ConstIterator& it) {
		_nodeCursor = it._nodeCursor;
		return *this;
	}

	bool operator==(const ConstIterator& it) const {
		return _nodeCursor == it._nodeCursor;
	}

	bool operator!=(const ConstIterator& it) const {
		return _nodeCursor != it._nodeCursor;
	}

	bool operator<(const ConstIterator& it) const {
		return **this < *it;
	}

	bool operator>(const ConstIterator& it) const {
		return **this > *it;
	}

	bool operator<=(const ConstIterator& it) const {
		return **this <= *it;
	}

	bool operator>=(const ConstIterator& it) const {
		return **this >= *it;
	}

	// pre-increment
	ConstIterator& operator++() {
		if (_nodeCursor->rightChild) {
			_nodeCursor = _nodeCursor->rightChild;
			while (_nodeCursor->leftChild) {
				_nodeCursor = _nodeCursor->leftChild;
			}
		}
		else {
			const TypedNode* before;
			do {
				before = _nodeCursor;
				_nodeCursor = _nodeCursor->parent;
			} while (_nodeCursor && before == _nodeCursor->rightChild);
		}
		return *this;
	}

	// post-increment
	ConstIterator operator++(int) {
		ConstIterator old(*this);
		++(*this);
		return old;
	}

	// pre-decrement
	ConstIterator& operator--() {
		if (_nodeCursor->leftChild) {
			_nodeCursor = _nodeCursor->leftChild;
			while (_nodeCursor->rightChild) {
				_nodeCursor = _nodeCursor->rightChild;
			}
		}
		else {
			const TypedNode* before;
			do {
				before = _nodeCursor;
				_nodeCursor = _nodeCursor->parent;
			} while (_nodeCursor && before == _nodeCursor->leftChild);
		}
		return *this;
	}

	// post-decrement
	ConstIterator operator--(int) {
		ConstIterator old(*this);
		--(*this);
		return old;
	}

	const T& operator*() const {
		return static_cast<const T&>(_nodeCursor->data);
	}

	const T* operator->() const {
		return &_nodeCursor->data;
	}
public:
	TypedNode const* _nodeCursor;
};




template <typename T>
class Tree {

private:
	using Node = Node<T>;
	using ConstIterator = ConstIterator<T>;
	using Iterator = Iterator<T>;

private:
	Node* _root;

public:

	Tree(): _root(new Node) { _root->n = 0; }

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

	Iterator Find(const T& t);
	Iterator Insert(const T& t);
	Iterator Erase(Iterator it);

	void Remove(const T& t);

	size_t Size() const { return _root->n; }

	bool Empty() const { return _root->leftChild == nullptr; }


private:
	void RotateLeft(Node* n);
	void RotateRight(Node* n);

	void FreeChildren(Node* nd);
};


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
typename Tree<T>::Iterator Tree<T>::Insert(const T& t)
{
	Iterator res;

	// descent the search tree
	Node* parent = _root;
	while (true) {
		++parent->n;
		if (parent == _root || t < parent->data) {
			if (parent->leftChild) {
				parent = parent->leftChild;
			}
			else {
				parent->leftChild = new Node(t);

				parent->leftChild->parent = parent;
				res = Iterator(parent->leftChild);
				break;
			}
		}
		else {
			if (parent->rightChild) {
				parent = parent->rightChild;
			}
			else {
				parent->rightChild = new Node(t);
				parent->rightChild->parent = parent;
				res = Iterator(parent->rightChild);
				break;
			}
		}
	}
	short branch_depth = 1;
	do {
		if (parent->depth > branch_depth)
			break;
		parent->depth = 1 + branch_depth;
		if (parent == _root)
			break;
		if (parent->Imbalance() < -1) {
			// check for double-rotation case
			if (parent->leftChild->Imbalance() > 0) {
				RotateLeft(parent->leftChild);
			}
			RotateRight(parent);
			break;
		}
		else if (parent->Imbalance() > 1) {
			// check for double-rotation case
			if (parent->rightChild->Imbalance() < 0) {
				RotateRight(parent->rightChild);
			}
			RotateLeft(parent);
			break;
		}
		branch_depth = parent->depth;
		parent = parent->parent;
	} while (parent);
	return res;
}

template <typename T>
typename Tree<T>::Iterator Tree<T>::Erase(Iterator it)
{
	Iterator itn(it);
	++itn;
	Node* ptr = it._nodeCursor;
	Node* q;
	if (!ptr->leftChild || !ptr->rightChild) {
		q = ptr;
	}
	else {
		q = itn._nodeCursor;
	}
	Node* s;
	if (q->leftChild) {
		s = q->leftChild;
		q->leftChild = nullptr;
	}
	else {
		s = q->rightChild;
		q->rightChild = nullptr;
	}
	if (s) {
		s->parent = q->parent;
	}
	if (q == q->parent->leftChild) {
		q->parent->leftChild = s;
	}
	else {
		q->parent->rightChild = s;
	}
	Node* q_parent = q->parent;
	if (q != ptr) {
		q->parent = ptr->parent;
		if (q->parent->leftChild == ptr) {
			q->parent->leftChild = q;
		}
		else {
			q->parent->rightChild = q;
		}
		q->leftChild = ptr->leftChild;
		if (q->leftChild)
			q->leftChild->parent = q;
		q->rightChild = ptr->rightChild;
		if (q->rightChild)
			q->rightChild->parent = q;
		q->n = ptr->n;
		q->depth = ptr->depth;
		ptr->leftChild = nullptr;
		ptr->rightChild = nullptr;
	}
	if (q_parent == ptr) {
		q_parent = q;
	}
	Node* parent;
	for (parent = q_parent; parent; parent = parent->parent) {
		--parent->n;
	}
	for (parent = q_parent; parent; parent = parent->parent) {
		parent->UpdateDepth();
		if (parent == _root)
			break;
		if (parent->Imbalance() < -1) {
			// check for double-rotation case
			if (parent->leftChild->Imbalance() > 0) {
				RotateLeft(parent->leftChild);
			}
			RotateRight(parent);
			break;
		}
		else if (parent->Imbalance() > 1) {
			// check for double-rotation case
			if (parent->rightChild->Imbalance() < 0) {
				RotateRight(parent->rightChild);
			}
			RotateLeft(parent);
			break;
		}
	}
	delete ptr;
	return itn;
}

template <typename T>
void Tree<T>::Remove(const T& t)
{
	Iterator it = find(t);
	if (it == End())
		return;
	do {
		it = erase(it);
	} while (*it == t);
}

template <typename T>
void Tree<T>::RotateLeft(Node* n)
{
	Node* tmp = n->rightChild->leftChild;
	if (n == n->parent->leftChild) {
		n->parent->leftChild = n->rightChild;
	}
	else {
		n->parent->rightChild = n->rightChild;
	}
	n->rightChild->parent = n->parent;
	n->rightChild->leftChild = n;
	n->parent = n->rightChild;
	n->rightChild = tmp;
	if (tmp)
		tmp->parent = n;

	// update ns
	n->UpdateN();
	n->parent->UpdateN();

	// update depths
	do {
		n->UpdateDepth();
		n = n->parent;
	} while (n);
}

template <typename T>
void Tree<T>::RotateRight(Node* n)
{
	Node* tmp = n->leftChild->rightChild;
	if (n == n->parent->leftChild) {
		n->parent->leftChild = n->leftChild;
	}
	else {
		n->parent->rightChild = n->leftChild;
	}
	n->leftChild->parent = n->parent;
	n->leftChild->rightChild = n;
	n->parent = n->leftChild;
	n->leftChild = tmp;
	if (tmp)
		tmp->parent = n;

	// update ns
	n->UpdateN();
	n->parent->UpdateN();

	// update depths
	do {
		n->UpdateDepth();
		n = n->parent;
	} while (n);
}

template <typename T>
void Tree<T>::FreeChildren(Node* nd)
{
	if (nd->leftChild) {
		FreeChildren(nd->leftChild);

		delete nd->leftChild;

	}
	if (nd->rightChild) {
		FreeChildren(nd->rightChild);

		delete nd->rightChild;
	}
}

template <typename T>
typename Tree<T>::Iterator Tree<T>::Find(const T& t)
{
	Node* ptr = _root->leftChild;
	while (ptr) {
		if (t == ptr->data) {
			return Iterator(ptr);
		}
		else if (t < ptr->data) {
			ptr = ptr->leftChild;
		}
		else {
			ptr = ptr->rightChild;
		}
	}
	return End();
}
