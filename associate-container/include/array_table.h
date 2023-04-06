#pragma once

#include "table.h"

template<class K, class V>
class ArrayTable : public Table<K, V> {

protected:

	size_t _recordsCount;
	size_t _capacity;
	detail::Record<K, V>* _data;

public:

	ArrayTable(size_t initialCapacity = 10) : _recordsCount(0), _capacity(initialCapacity), _data(new detail::Record<K, V>[_capacity]) {}
	~ArrayTable() override { delete[] _data; }
	/*ArrayTable(std::initializer_list<std::pair<K, V*>> list): recordsCount(list.size()), capacity(list.size()), data(new Record[capacity])
	{
		auto iter = list.begin();
		for (size_t i = 0; i < capacity; i++)
		{
			auto pair = list.begin()[i];
			data[i] = Record(pair.first(), pair.second());
		}
	}*/

protected:

	void EnsureCapacity(size_t multiplier);

};

template <class K, class V>
void ArrayTable<K, V>::EnsureCapacity(size_t multiplier)
{
	using namespace detail;

	auto ensuredCapacity = _capacity * multiplier;
	auto* ensuredData = new Record<K, V>[ensuredCapacity];
	std::copy(_data, _data + _capacity, ensuredData);

	delete[] _data;

	_data = ensuredData;
	_capacity = ensuredCapacity;
}
