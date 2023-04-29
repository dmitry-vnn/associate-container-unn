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
		base(list.size())
	{
		std::move(list.begin(), list.end(), base::Begin());
		std::sort(base::_data, base::_data + base::_size, TypedRecord::Compare);
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

	auto pair = BinarySearchRecord(key);

	auto isFound = pair.first;
	auto position = pair.second;

	if (isFound)
	{
		base::_data[position].value = std::move(value);
		return;
	}

	auto record = TypedRecord(key, std::move(value));
	
	base::Insert(std::move(record), position);
		
}

template <class K, class V>
int OrderedTable<K, V>::FindPosition(const K& key) const
{
	auto pair = BinarySearchRecord(key);

	auto isFound = pair.first;
	auto position = pair.second;

	return isFound ? position : -1;

}

template <class K, class V>
std::pair<bool, size_t> OrderedTable<K, V>::BinarySearchRecord(const K& key) const
{

	if (base::_size == 0)
	{
		return { false, 0 };
	}

	int first = 0;
	int last = base::_size - 1;

	while (first <= last)
	{
		size_t mid = (first + last) / 2;

		auto& currentRecord = base::_data[mid];
		auto& currentKey = currentRecord.key;

		if (currentKey == key)
		{
			return { true, mid };
		}

		if (TypedRecord::CompareKeys(currentKey, key))
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
