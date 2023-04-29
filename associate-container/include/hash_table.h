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
private:
	using base = VirtualTableIterator<K, V>;
	using TypedNode = Node<K, V>;

private:
	TypedNode* _currentNode;
	TypedNode* _endNode;

public:

	explicit HashTableIterator(TypedNode* currentNode, size_t size):
		base(), _currentNode(currentNode), _endNode(currentNode + size)
			{}

	bool operator!=(const VirtualTableIterator<K, V>& other) const override
	{
		return !operator==(other);
	}

	bool operator==(const VirtualTableIterator<K, V>& other) const override
	{
		auto* otherHashTableIterator =
			dynamic_cast<const HashTableIterator*>(&other);

		return otherHashTableIterator != nullptr
			&& _currentNode == otherHashTableIterator->_currentNode;
	}

	Record<K, V>& operator*() const override
	{
		return *_currentNode->record;
	}

	VirtualTableIterator<K, V>& operator++() override;
	std::unique_ptr<VirtualTableIterator<K, V>> Copy() override;

	static TableIterator<K, V> Create(TypedNode* currentNode)
	{
		return TableIterator<K, V>(std::move(std::make_unique<HashTableIterator>(currentNode)));
	}
};

template <class K, class V>
VirtualTableIterator<K, V>& HashTableIterator<K, V>::operator++()
{
	if (_currentNode->nextNode != nullptr)
	{
		_currentNode = _currentNode->nextNode;
	} else
	{
		while (_currentNode != _endNode && ++_currentNode != nullptr);
	}

	return *this;
}

template <class K, class V>
std::unique_ptr<VirtualTableIterator<K, V>> HashTableIterator<K, V>::Copy()
{
	return std::make_unique<HashTableIterator>(_currentNode);
}

template<class K, class V, class >
class HashTable final : public Table<K, V>
{
private:
	using TypedNode = Node<K, V>;

private:
	size_t _elementsCount;
	size_t _size;

	TypedNode* _data;

private:
	double _loadFactor;

public:
	explicit HashTable(size_t size, double loadFactor = 0.75) :
		_elementsCount(0), _size(size), _data(new TypedNode[size]),
		_loadFactor(loadFactor)
	{}

	void Add(const K& key, V value) override;
	Table<K, V>::ConstIterator Find(const K& key) const override;
	Table<K, V>::ConstIterator Remove(const K& key) override;
	size_t Size() override;
	Table<K, V>::Iterator Begin() override;
	Table<K, V>::Iterator End() override;
	Table<K, V>::ConstIterator Begin() const override;
	Table<K, V>::ConstIterator End() const override;

private:
	bool NeedRehashing();
	void IncreaseAndRehashData();
};

template <class K, class V>
void HashTable<K, V>::Add(const K& key, V value)
{
	if (NeedRehashing())
	{
		IncreaseAndRehashData();
	}
}

template <class K, class V>
void HashTable<K, V>::IncreaseAndRehashData()
{
	auto increasedSize = _size * 2;
	auto* increasedData = new TypedNode[increasedSize];

	//TODO rehashing...
}

template <class K, class V>
bool HashTable<K, V>::NeedRehashing()
{
	return _loadFactor * _size <= _elementsCount;
}
