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

protected:
	int FindPosition(const K& key) const override;

public:
	~ScanTable() override = default;
};

template <class K, class V>
void ScanTable<K, V>::Add(const K& key, V value)
{
	base::PushBack(std::move(TypedRecord{key, value}));
}

template <class K, class V>
int ScanTable<K, V>::FindPosition(const K& key) const
{

	for (size_t i = 0; i < base::_size; i++)
	{
		if (base::_data[i].key == key)
		{
			return i;
		}
	}

	return -1;

}
