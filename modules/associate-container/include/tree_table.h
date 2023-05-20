#pragma once

#include <utility>
#include <string>
#include <algorithm>
#include <memory>

#include <table.h>

template<class K, class V>
struct TreeNode {

	Record<K, V> record;

	short depth = 1;
	size_t size = 1;

	TreeNode* parent = nullptr;
	TreeNode* leftChild = nullptr;
	TreeNode* rightChild = nullptr;


public:

	explicit TreeNode(Record<K, V> record = Record<K, V>()): record(record) {}

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

template<class K, class V>
class TreeTable;

template<class K, class V>
class TreeTableIterator : public VirtualTableIterator<K, V> {

	friend TreeTable<K, V>;

private:
	using TreeNode = TreeNode<K, V>;
	using TypedRecord = Record<K, V>;

private:
	TreeNode* _nodeCursor;

public:

	explicit TreeTableIterator(TreeNode* node = nullptr) : _nodeCursor(node) {}

public:

	bool operator!=(const VirtualTableIterator<K, V>& other) const override
	{
		return !operator==(other);
	}

	bool operator==(const VirtualTableIterator<K, V>& other) const override;

	TreeTableIterator& operator++() override;



	Record<K, V>& operator*() const override
	{
		return _nodeCursor->record;
	}

	std::unique_ptr<VirtualTableIterator<K, V>> Copy() override;

	template<class... Args>
	static TableIterator<K, V> Create(Args&&... args)
	{
		return TableIterator<K, V>(std::move(std::make_unique<TreeTableIterator>(
			std::forward<Args>(args)...)
		));
	}



	/*

	explicit TreeTableIterator(TypedNode* node = nullptr): _nodeCursor(node) {}

	TreeTableIterator(const TreeTableIterator& other) = default;
	TreeTableIterator& operator=(const TreeTableIterator& other) = default;

	bool operator==(const TreeTableIterator& other) const {
		return _nodeCursor == other._nodeCursor;
	}

	bool operator!=(const TreeTableIterator& other) const {
		return !operator==(other);
	}

	TreeTableIterator& operator++();

	TypedRecord& operator*() const {
		return _nodeCursor->record;
	}

	TypedRecord* operator->() const {
		return &_nodeCursor->record;
	} */
};

template<class K, class V>
class TreeTable : public Table<K, V> {

private:
	using TreeNode = TreeNode<K, V>;
	using Iterator = TreeTableIterator<K, V>;
	using ConstIterator = Iterator;

private:
	TreeNode* _root;

public:

	TreeTable(): _root(new TreeNode) { _root->size = 0; }

	~TreeTable() override;

	TreeTable(const TreeTable& other) = delete;
	TreeTable(TreeTable&& other) noexcept = delete;
	TreeTable& operator=(const TreeTable& other) = delete;
	TreeTable& operator=(TreeTable&& other) noexcept = delete;

public:

	typename Table<K, V>::Iterator Begin() override;
	typename Table<K, V>::ConstIterator Begin() const override;

	typename Table<K, V>::Iterator End() override;
	typename Table<K, V>::ConstIterator End() const override;

	typename Table<K, V>::ConstIterator Find(const K& key) const override;

	void Add(const K& key, V value) override;
	typename Table<K, V>::ConstIterator Remove(const K& key) override;


	size_t Size() const override { return _root->size; }


private:

	TreeNode* GetNodeCursor(typename Table<K, V>::ConstIterator iterator) const;

	void RotateLeft(TreeNode* node);
	void RotateRight(TreeNode* node);

	void FreeChildren(TreeNode* node);
};


template<class K, class V>
TreeTableIterator<K, V>& TreeTableIterator<K, V>::operator++()
{
	if (_nodeCursor->rightChild != nullptr) {
		_nodeCursor = _nodeCursor->rightChild;
		while (_nodeCursor->leftChild != nullptr) {
			_nodeCursor = _nodeCursor->leftChild;
		}
	}
	else {
		TreeNode* before;
		do {
			before = _nodeCursor;
			_nodeCursor = _nodeCursor->parent;
		} while (_nodeCursor && before == _nodeCursor->rightChild);
	}
	return *this;
}

template <class K, class V>
bool TreeTableIterator<K, V>::operator==(const VirtualTableIterator<K, V>& other) const
{
	auto* otherHashTableIterator =
		dynamic_cast<const TreeTableIterator*>(&other);

	return otherHashTableIterator != nullptr
		&& _nodeCursor == otherHashTableIterator->_nodeCursor;
}

template <class K, class V>
std::unique_ptr<VirtualTableIterator<K, V>> TreeTableIterator<K, V>::Copy()
{
	return std::make_unique<TreeTableIterator>(_nodeCursor);
}

template<class K, class V>
TreeTable<K, V>::~TreeTable()
{
	FreeChildren(_root);
	delete _root;
}

template<class K, class V>
typename Table<K, V>::Iterator TreeTable<K, V>::Begin()
{
	return static_cast<const TreeTable*>(this)->Begin();
}

template<class K, class V>
typename Table<K, V>::ConstIterator TreeTable<K, V>::Begin() const
{
	TreeNode* node = _root;

	while (node->leftChild != nullptr) {
		node = node->leftChild;
	}

	return Iterator::Create(node);
}

template <class K, class V>
typename Table<K, V>::Iterator TreeTable<K, V>::End()
{
	return static_cast<const TreeTable*>(this)->End();
}

template <class K, class V>
typename Table<K, V>::ConstIterator TreeTable<K, V>::End() const
{
	return Iterator::Create(_root);
}

template<class K, class V>
void TreeTable<K, V>::Add(const K& key, V value)
{

	auto& iterations = const_cast<size_t&>(Table<K, V>::_lastIterationCount);
	iterations = 0;

	// descent the search tree
	TreeNode* parent = _root;
	while (true) {

		++iterations;

		++parent->size;
		if (parent == _root || key < parent->record.key) {
			if (parent->leftChild != nullptr) {
				parent = parent->leftChild;
			}
			else {
				parent->leftChild = new TreeNode({key, value});
				parent->leftChild->parent = parent;
				break;
			}
		}
		else {
			if (parent->rightChild != nullptr) {
				parent = parent->rightChild;
			}
			else {
				parent->rightChild = new TreeNode({ key, value });
				parent->rightChild->parent = parent;
				break;
			}
		}
	}

	short branchDepth = 1;
	do {
		++iterations;
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

template <class K, class V>
typename Table<K, V>::ConstIterator TreeTable<K, V>::Remove(const K& key)
{

	auto iterator = Find(key);

	auto endIterator = End();

	if (iterator == End())
	{
		return endIterator;
	}

	auto& iterations = const_cast<size_t&>(Table<K, V>::_lastIterationCount);

	auto iteratorCopy = iterator;
	++iteratorCopy;

	TreeNode* currentNode = GetNodeCursor(iterator);

	TreeNode* q;
	if (currentNode->leftChild == nullptr
		|| currentNode->rightChild == nullptr) {
		q = currentNode;
	} else {
		q = GetNodeCursor(iteratorCopy);
	}

	TreeNode* s;
	if (q->leftChild != nullptr) {
		s = q->leftChild;
		q->leftChild = nullptr;
	} else {
		s = q->rightChild;
		q->rightChild = nullptr;
	}
	if (s != nullptr) {
		s->parent = q->parent;
	}
	if (q == q->parent->leftChild) {
		q->parent->leftChild = s;
	} else {
		q->parent->rightChild = s;
	}
	TreeNode* qParent = q->parent;
	if (q != currentNode) {
		q->parent = currentNode->parent;
		if (q->parent->leftChild == currentNode) {
			q->parent->leftChild = q;
		} else {
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
	TreeNode* parent;
	for (parent = qParent; parent != nullptr; parent = parent->parent) {
		++iterations;
		--parent->size;
	}
	for (parent = qParent; parent != nullptr; parent = parent->parent) {
		++iterations;
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

template <class K, class V>
typename TreeTable<K, V>::TreeNode* TreeTable<K, V>::GetNodeCursor(typename Table<K, V>::ConstIterator iterator) const
{
	return dynamic_cast<TreeTableIterator<K, V>*>(iterator._iterator.get())->_nodeCursor;
}

template<class K, class V>
void TreeTable<K, V>::RotateLeft(TreeNode* node)
{
	auto& iterations = const_cast<size_t&>(Table<K, V>::_lastIterationCount);

	TreeNode* tempNode = node->rightChild->leftChild;
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
		++iterations;
		node->UpdateDepth();
		node = node->parent;
	} while (node != nullptr);
}

template<class K, class V>
void TreeTable<K, V>::RotateRight(TreeNode* node)
{
	auto& iterations = const_cast<size_t&>(Table<K, V>::_lastIterationCount);

	TreeNode* tempNode = node->leftChild->rightChild;
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
		++iterations;
		node->UpdateDepth();
		node = node->parent;
	} while (node != nullptr);
}

template<class K, class V>
void TreeTable<K, V>::FreeChildren(TreeNode* node)
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

template<class K, class V>
typename Table<K, V>::ConstIterator TreeTable<K, V>::Find(const K& key) const
{
	auto& iterations = const_cast<size_t&>(Table<K, V>::_lastIterationCount);
	iterations = 0;

	TreeNode* node = _root->leftChild;

	while (node != nullptr) {

		++iterations;

		if (key == node->record.key) {
			return Iterator::Create(node);
		}
		if (key < node->record.key) {
			node = node->leftChild;
		} else {
			node = node->rightChild;
		}
	}

	return End();
}
