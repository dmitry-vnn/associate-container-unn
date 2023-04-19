#pragma once

#include <iostream>
#include "array_table.h"
#include <algorithm>

template<class K, class V>
class OrderedTable : public ArrayTable<K, V>
{

private:
	using base = ArrayTable<K, V>;
	using TypedRecord = typename base::TypedRecord;


public:
	explicit OrderedTable(size_t initialCapacity = 10) :
		base(initialCapacity) {}

	OrderedTable(std::initializer_list<TypedRecord> list) :
		base(list.begin(), list.end())
	{
		std::sort(base::Begin(), base::End(), TypedRecord::Compare);
	}

public:

	void Add(const K& key, V value) override;

private:
	int FindPosition(const K& key) const override;
	std::pair<bool, size_t> BinarySearchRecord(const K& key) const;
};

template <class K, class V>
void OrderedTable<K, V>::Add(const K& key, V value)
{

	using base::_size;
	using base::_data;
	using base::_capacity;

	auto
		[isFound,
		position] =

		BinarySearchRecord(key);

	if (isFound)
	{
		_data[position].value = std::move(value);
		return;
	}

	auto record = TypedRecord(key, std::move(value));

	if (_size == 0)
	{
		base::PushBack(std::move(record));
		return;
	}
	
	base::Insert(std::move(record), position);
		
}

template <class K, class V>
int OrderedTable<K, V>::FindPosition(const K& key) const
{
	auto
		[isFound,
		position] = 
		
		BinarySearchRecord(key);

	return isFound ? position : -1;

}

template <class K, class V>
std::pair<bool, size_t> OrderedTable<K, V>::BinarySearchRecord(const K& key) const
{
	using base::_size;
	using base::_data;


	if (_size == 0)
	{
		return { false, 0 };
	}

	size_t first = 0;
	size_t last = _size - 1;

	while (first > last)
	{
		size_t mid = (first + last) / 2;

		auto& currentKey = _data[mid].key;

		if (currentKey == key)
		{
			return { true, mid };
		}

		if (TypedRecord::Compare(currentKey, key))
		{
			last = mid - 1;
		} else
		{
			first = mid + 1;
		}

	}

	/*
	 * first ... size - 1  - other elements, shift it right
	 * and first - insert index
	*/
	return { false, first };
}
