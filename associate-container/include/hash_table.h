#pragma once

#include <functional>

#include "table.h"
#include <iostream>

template<class K, class V>
struct Node
{
	Record<K, V>* record;
	Node* nextNode;

	Node(Record<K, V>* record = nullptr, Node* nextNode = nullptr):
		record(record), nextNode(nextNode) {}
};

template<class K, class V>
class HashTableIterator: public VirtualTableIterator<K, V>
{
private:
	using base = VirtualTableIterator<K, V>;

	using TypedNode = Node<K, V>;

private:


	TypedNode* _currentNodeByMemoryOffset;
	TypedNode* _currentNodeByDepth;

	TypedNode* _endNode;

	
public:
	HashTableIterator(TypedNode* currentNodeByMemoryOffset, 
		TypedNode* currentNodeByDepth, TypedNode* endNode):

		_currentNodeByMemoryOffset(currentNodeByMemoryOffset),
		_currentNodeByDepth(currentNodeByDepth),
		_endNode(endNode) {}

	bool operator!=(const VirtualTableIterator<K, V>& other) const override
	{
		return !operator==(other);
	}

	bool operator==(const VirtualTableIterator<K, V>& other) const override
	{
		auto* otherHashTableIterator =
			dynamic_cast<const HashTableIterator*>(&other);

		return otherHashTableIterator != nullptr
			&& _currentNodeByDepth == otherHashTableIterator->_currentNodeByDepth;
	}

	Record<K, V>& operator*() const override
	{
		return *_currentNodeByDepth->record;
	}

	VirtualTableIterator<K, V>& operator++() override;
	std::unique_ptr<VirtualTableIterator<K, V>> Copy() override;

	template<class... Args>
	static TableIterator<K, V> Create(Args&&... args)
	{
		return TableIterator<K, V>(std::move(std::make_unique<HashTableIterator>(
			std::forward<Args>(args)...)
		));
	}
};

template <class K, class V>
VirtualTableIterator<K, V>& HashTableIterator<K, V>::operator++()
{
	if (_currentNodeByMemoryOffset != _endNode)
	{
		auto* nextNodeByDepth = _currentNodeByDepth->nextNode;

		if (nextNodeByDepth != nullptr)
		{
			_currentNodeByDepth = nextNodeByDepth;
		} else
		{
			while (++_currentNodeByMemoryOffset != _endNode
				&& _currentNodeByMemoryOffset->record == nullptr) {}

			_currentNodeByDepth = _currentNodeByMemoryOffset;
		}
	}

	return *this;
}

template <class K, class V>
std::unique_ptr<VirtualTableIterator<K, V>> HashTableIterator<K, V>::Copy()
{
	return std::make_unique<HashTableIterator>(
		_currentNodeByMemoryOffset, _currentNodeByDepth, _endNode
	);
}

template<class K, class V> 
class HashTable final : public Table<K, V>
{

private:
	using Iterator = HashTableIterator<K, V>;
	using TypedNode = Node<K, V>;
	using TypedRecord = Record<K, V>;

private:
	//hash function: K -> size_t
	using HashCodeFunction = std::function<size_t(K)>;

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

	~HashTable() override;

public:

	void Add(const K& key, V value) override;
	typename Table<K, V>::ConstIterator Find(const K& key) const override;
	typename Table<K, V>::ConstIterator Remove(const K& key) override;

	size_t Size() override { return _elementsCount; }

	typename Table<K, V>::Iterator Begin() override;
	typename Table<K, V>::Iterator End() override;

	typename Table<K, V>::ConstIterator Begin() const override;
	typename Table<K, V>::ConstIterator End() const override;


	void Print() const;

private:

	size_t Hash(const K& key) const;

	bool NeedRehashing() const;
	void IncreaseAndRehashData();

	/*
	* Return
	* memory offset node,
	* previous depth node, 
	* current depth node
	*/
	std::tuple<TypedNode*, TypedNode*, TypedNode*> FindNode(const K& key) const;

	TypedNode* EndNode() const
	{
		return _data + _size;
	}
};

template <class K, class V>
HashTable<K, V>::~HashTable()
{
	for (size_t i = 0; i < _size; i++)
	{
		auto* node = _data + i;

		delete node->record;

		if (node->nextNode != nullptr)
		{
			node = node->nextNode;

			while (node != nullptr)
			{
				auto* nextNode = node->nextNode;

				delete node->record;
				delete node;

				node = nextNode;
			}
		}

	}

	delete[] _data;
}

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
		currentNode->record = new TypedRecord(key, std::move(value));
		_elementsCount++;
	} else
	{
		bool isKeyExists = false;
		//auto* previousNode = currentNode;

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

		if (!isKeyExists)
		{
			currentNode->nextNode = new TypedNode(
				new TypedRecord(key, std::move(value))
			);

			_elementsCount++;
		}

	}


}

template <class K, class V>
typename Table<K, V>::ConstIterator HashTable<K, V>::Find(const K& key) const
{
	auto tuple = FindNode(key);

	return Iterator::Create(
		std::get<0>(tuple),
		std::get<2>(tuple),
		EndNode()
	);
}

template <class K, class V>
typename Table<K, V>::ConstIterator HashTable<K, V>::Remove(const K& key)
{
	auto tuple = FindNode(key);

	TypedNode* memoryOffsetNode = std::get<0>(tuple);

	if (memoryOffsetNode == EndNode())
	{
		return End();
	}

	_elementsCount--;

	TypedNode* previousDepthNode = std::get<1>(tuple);
	TypedNode* currentDepthNode = std::get<2>(tuple);


	if (memoryOffsetNode == currentDepthNode)
	{
		delete memoryOffsetNode->record;

		auto* nextDepthNode = memoryOffsetNode->nextNode;

		if (nextDepthNode == nullptr)
		{
			memoryOffsetNode->record = nullptr;

			return
				++
				Iterator::Create(memoryOffsetNode, memoryOffsetNode, EndNode());
		}

		memoryOffsetNode->record = nextDepthNode->record;
		memoryOffsetNode->nextNode = nextDepthNode->nextNode;

		delete nextDepthNode;

		return Iterator::Create(
			memoryOffsetNode, 
			memoryOffsetNode, 
			EndNode()
		);

	}

	previousDepthNode->nextNode = currentDepthNode->nextNode;

	delete currentDepthNode->record;
	delete currentDepthNode;

	return Iterator::Create(
		memoryOffsetNode, 
		previousDepthNode->nextNode,
		EndNode()
	);
}

template <class K, class V>
typename Table<K, V>::Iterator HashTable<K, V>::Begin()
{
	return static_cast<const HashTable*>(this)->Begin();
}

template <class K, class V>
typename Table<K, V>::Iterator HashTable<K, V>::End()
{
	return static_cast<const HashTable*>(this)->End();
}

template <class K, class V>
typename Table<K, V>::ConstIterator HashTable<K, V>::Begin() const
{
	auto iterator = Iterator::Create(_data, _data, EndNode());

	if (_data[0].record == nullptr)
	{
		return ++iterator;
	}

	return iterator;
}

template <class K, class V>
typename Table<K, V>::ConstIterator HashTable<K, V>::End() const
{
	return Iterator::Create(EndNode(), EndNode(), EndNode());
}

template <class K, class V>
void HashTable<K, V>::Print() const
{
	std::cout << "[" << std::endl;

	for (size_t i = 0; i < _size; i++)
	{
		std::cout << " [";

		auto* node = _data + i;

		if (node->record != nullptr)
		{
			while (node != nullptr)
			{
				std::cout
					<< " (" << node->record->key
					<< " -> " << node->record->value
					<< ")" << ",";

				node = node->nextNode;
			}

			std::cout << " ";
		}

		std::cout << "]";

		if (i < _size - 1)
		{
			std::cout << ",";
		}

		std::cout << std::endl;
	}

	std::cout << "]" << std::endl;
}

template <class K, class V>
void HashTable<K, V>::IncreaseAndRehashData()
{
	//auto increasedSize = _size * 2;
	//auto* increasedData = new TypedNode[increasedSize];

	//TODO rehashing...
	throw std::logic_error("Not implemented yet");
}

template <class K, class V>
std::tuple<Node<K, V>*, Node<K, V>*, Node<K, V>*> HashTable<K, V>::FindNode(const K& key) const
{

	size_t position = Hash(key);

	if (_data[position].record != nullptr)
	{
		bool isKeyFound = false;

		auto* depthNode = _data + position;
		auto* previousDepthNode = _data + position;

		while (true)
		{

			if (depthNode->record->key != key)
			{

				previousDepthNode = depthNode;
				depthNode = _data->nextNode;

				if (depthNode == nullptr)
				{
					break;
				}

			}

			else
			{
				isKeyFound = true;
				break;
			}
		}

		if (isKeyFound)
		{
			return { _data + position, previousDepthNode, depthNode };
		}
	}

	auto* endNode = EndNode();
	return { endNode, endNode, endNode };
}

template <class K, class V>
size_t HashTable<K, V>::Hash(const K& key) const
{
	return _hashCodeFunction(key) % _size;
}

template <class K, class V>
bool HashTable<K, V>::NeedRehashing() const
{
	//return _loadFactor * _size <= _elementsCount;
	return false;
}
