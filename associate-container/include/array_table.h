#pragma once

#include "table.h"

template<class K, class V>
class ArrayTableIterator final : public TableIterator<K, V>
{

private:
	typedef TableIterator<K, V> base;

public:
	explicit ArrayTableIterator(typename TableIterator<K, V>::TRecord* record):
		TableIterator<K, V>(record) {}

	TableIterator<K, V>& operator++() override
	{
		++base::_recordCursor;
		return *this;
	}
};

template<class K, class V>
class ArrayTable : public Table<K, V> {

private:
	typedef typename Table<K, V>::TRecord TRecord;
	typedef ArrayTableIterator<K, V> Iterator;

private:
	size_t _ensureMultiplier;

protected:

	size_t _recordsCount;
	size_t _capacity;
	Record<K, V>* _data;


protected:

	ArrayTable(size_t initialCapacity = 10, size_t ensureMultiplier = 2) :
		_ensureMultiplier(ensureMultiplier), _recordsCount(0),
		_capacity(initialCapacity), _data(new TRecord[_capacity])
	{}

	template<class IteratorType>
	ArrayTable(IteratorType first, IteratorType last, size_t ensureMultiplier = 2):
		_ensureMultiplier(ensureMultiplier), _recordsCount(last - first),
		_capacity(_recordsCount), _data(new TRecord[_capacity])
	{
		std::copy(first, last, _data);
	}

public:
	typename Table<K, V>::Iterator Begin() override { return Iterator(_data); }
	typename Table<K, V>::Iterator End() override { return Iterator(_data + _capacity); }
	typename Table<K, V>::ConstIterator Begin() const override { return const_cast<ArrayTable*>(this)->Begin(); }
	typename Table<K, V>::ConstIterator End() const override { return const_cast<ArrayTable*>(this)->End(); }

	~ArrayTable() override { delete[] _data; }

protected:


	void EnsureCapacity(size_t multiplier);
	void PushBack(const TRecord& record);

};

template <class K, class V>
void ArrayTable<K, V>::EnsureCapacity(size_t multiplier)
{
	auto ensuredCapacity = _capacity * multiplier;

	auto* ensuredData = new Record<K, V>[ensuredCapacity];
	std::copy(_data, _data + _capacity, ensuredData);

	delete[] _data;

	_data = ensuredData;
	_capacity = ensuredCapacity;
}

template <class K, class V>
void ArrayTable<K, V>::PushBack(const TRecord& record)
{
	if (_capacity == _recordsCount)
	{
		EnsureCapacity()
	}
}
