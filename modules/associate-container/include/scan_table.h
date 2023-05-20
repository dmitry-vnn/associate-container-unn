#pragma once


#include <iostream>
#include "array_table.h" 

template<class K, class V>
class ScanTable final : public ArrayTable<K, V>
{

private:
	using base = ArrayTable<K, V>;
	using TypedRecord = typename base::TypedRecord;
	

public:
	explicit ScanTable(size_t initialCapacity = 10):
		base(initialCapacity) {}

	ScanTable(std::initializer_list<TypedRecord> list):
		base(list.begin(), list.end()) {}

public:
	void Add(const K& key, V value) override;

private:
	int FindPosition(const K& key) const override;
	void RemoveByPosition(size_t position) override;

public:
	~ScanTable() override = default;
};

template <class K, class V>
void ScanTable<K, V>::Add(const K& key, V value)
{
	auto position = FindPosition(key);
	if (position == -1)
	{
		base::PushBack(std::move(TypedRecord{ key, std::move(value) }));
	} else
	{
		base::_data[position].value = std::move(value);
	}
}

template <class K, class V>
int ScanTable<K, V>::FindPosition(const K& key) const
{

	auto& iterations = const_cast<size_t&>(base::_lastIterationCount);

	iterations = 0;
	
	for (size_t i = 0; i < base::_size; i++)
	{
		++iterations;
		if (base::_data[i].key == key)
		{
			return i;
		}
	}

	return -1;

}

template <class K, class V>
void ScanTable<K, V>::RemoveByPosition(size_t position)
{
	//auto& iterations = base::_lastIterationCount;
	//iterations = 0;

	auto size = base::_size;
	auto data = base::_data;

	if (position != size - 1)
	{
		data[position] = std::move(data[size - 1]);
	}
}
