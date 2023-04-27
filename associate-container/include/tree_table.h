#pragma once
#include "table.h"

template<class K, class V>
struct Node
{

public:
	Record<K, V> record;

	Node* leftChild;
	Node* rightChild;

public:
	Node(Record<K, V> record, Node* leftChild, Node* rightChild):
		record(std::move(record)),
		leftChild(leftChild),
		rightChild(rightChild) {}

	explicit Node(Record<K, V> record): Node(std::move(record), nullptr, nullptr) {}

	Node(const Node& other) = default;
	Node& operator=(const Node& other) = default;

	Node(Node&& other) noexcept = default;
	Node& operator=(Node&& other) noexcept = default;

	~Node() = default;
};

template<class K, class V>
class TreeIterator: public VirtualTableIterator<K, V>
{
	
};

template<class K, class V>
class TreeTable : public Table<K, V>
{

protected:
	using base = Table<K, V>;

	using TypedRecord = typename base::TypedRecord;

	using TypedNode = Node<K, V>;
	using NodePointer = TypedNode*;

protected:
	TypedNode* _root;
	size_t _size;

protected:
	TreeTable(): _root(nullptr), _size(0) {}

	NodePointer FindNode(const K& key);
};

template <class K, class V>
typename TreeTable<K, V>::NodePointer TreeTable<K, V>::FindNode(const K& key)
{
	auto currentNode = _root;

	while(currentNode != nullptr)
	{
		auto& currentKey = currentNode->record.key;

		if (currentKey == key)
		{
			return currentNode;
		}

		if (TypedRecord::Compare(key, currentKey))
		{
			currentNode = currentNode->leftChild;
		} else
		{
			currentNode = currentNode->rightChild;
		}
	}

	return nullptr;
}
