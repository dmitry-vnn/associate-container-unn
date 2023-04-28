#pragma once

#include "table.h"

template<class K, class V>
struct Node
{
	Record<K, V>* record;
	Node* nextNode;

	Node(Record<K, V>* record, Node* nextNode = nullptr):
		record(record), nextNode(nextNode) {}
};

template<class K, class V>
class HashTableIterator: VirtualTableIterator<K, V>
{
public:



	explicit HashTableIterator(Node<K, V>* node):
			VirtualTableIterator<K, V>(node->record) {}

	VirtualTableIterator<K, V>& operator++() override;
	std::unique_ptr<VirtualTableIterator<K, V>> Copy() override;
};

template <class K, class V>
VirtualTableIterator<K, V>& HashTableIterator<K, V>::operator++()
{
	
}

template <class K, class V>
std::unique_ptr<VirtualTableIterator<K, V>> HashTableIterator<K, V>::Copy()
{
}

template<class K, class V>
class HashTable: public Table<K, V>
{
private:
	size_t _size;
	size_t _capacity;



public:
	explicit HashTable(size_t capacity):
		_size(0), _capacity(capacity) {}


};
