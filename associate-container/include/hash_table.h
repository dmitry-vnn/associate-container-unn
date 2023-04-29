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

template<class K, class V> 
class HashTable final : public Table<K, V>
{

private:
	using TypedNode = Node<K, V>;
	using TypedRecord = Record<K, V>;

private:
	using HashCodeFunction = size_t(*)(K); //hash function signature (function ptr)

	HashCodeFunction _hashCodeFunction;

private:
	size_t _elementsCount;
	size_t _size;

	TypedNode* _data;

private:
	double _loadFactor;

public:
	explicit HashTable(
		size_t size, 
		double loadFactor = 0.75,
		HashCodeFunction hashCodeFunction = std::hash<K>()
	):
		_hashCodeFunction(hashCodeFunction), _elementsCount(0), _size(size),
		_data(new TypedNode[size]), _loadFactor(loadFactor)
	{}

	void Add(const K& key, V value) override;

	typename Table<K, V>::ConstIterator Find(const K& key) const override;
	typename Table<K, V>::ConstIterator Remove(const K& key) override;

	size_t Size() override;

	typename Table<K, V>::Iterator Begin() override;
	typename Table<K, V>::Iterator End() override;
	typename Table<K, V>::ConstIterator Begin() const override;
	typename Table<K, V>::ConstIterator End() const override;

private:

	size_t Hash(const K& key) const;

	bool NeedRehashing() const;
	void IncreaseAndRehashData();
};

template <class K, class V>
void HashTable<K, V>::Add(const K& key, V value)
{
	if (NeedRehashing())
	{
		IncreaseAndRehashData();
	}

	size_t position = Hash(key);

	auto* currentNode = _data + position;

	if (currentNode->record == nullptr)
	{
		currentNode->record = std::move(TypedRecord(key, std::move(value)));
	} else
	{
		//auto* previousNode = currentNode;

		bool isKeyExists = false;

		do
		{
			if (currentNode->record->key == key)
			{
				isKeyExists = true;

				//key exists, overwrite value with a new one
				currentNode->record->value = std::move(value);
				break;
			}

			//previousNode = currentNode;
			auto* nextNode = currentNode->nextNode;

			if (nextNode == nullptr)
			{
				break;
			}

			currentNode = nextNode;

		} while (true);

		if (isKeyExists)
		{
			return;
		}
	}

	currentNode->nextNode = new TypedNode(
		new TypedRecord(key, std::move(value))
	);
}

template <class K, class V>
void HashTable<K, V>::IncreaseAndRehashData()
{
	auto increasedSize = _size * 2;
	auto* increasedData = new TypedNode[increasedSize];

	//TODO rehashing...
}

template <class K, class V>
size_t HashTable<K, V>::Hash(const K& key) const
{
	return HashCodeFunction(key) % _size;
}

template <class K, class V>
bool HashTable<K, V>::NeedRehashing() const
{
	return _loadFactor * _size <= _elementsCount;
}
