﻿#pragma once

#include "table.h"
#include <iostream>

template<class K, class V>
class ArrayTableIterator final : public VirtualTableIterator<K, V>
{

private:
	using base = VirtualTableIterator<K, V>;

public:
	explicit ArrayTableIterator(typename base::RecordPointer currentRecord):
		VirtualTableIterator<K, V>(currentRecord) {}

	TableIterator<K, V>& operator++() override
	{
		//Increment the record pointer (pointer arithmetic)
		++base::_currentRecord;

		return *this;
	}
};

template<class K, class V>
class ArrayTable : public Table<K, V> {

private:
	using TypedRecord = typename Table<K, V>::TypedRecord;

private:
	size_t _ensureMultiplier;

protected:

	size_t _size;
	size_t _capacity;
	Record<K, V>* _data;


protected:

	explicit ArrayTable(size_t initialCapacity = 10, size_t ensureMultiplier = 2) :
		_ensureMultiplier(ensureMultiplier), _size(0),
		_capacity(initialCapacity), _data(new TypedRecord[_capacity])
	{}

	template<class IteratorType>
	ArrayTable(IteratorType first, IteratorType last, size_t ensureMultiplier = 2):
		_ensureMultiplier(ensureMultiplier), _size(last - first),
		_capacity(_size), _data(new TypedRecord[_capacity])
	{
		size_t index = 0;
		while (first != last)
		{
			_data[index] = std::move( *(first++) );
		}
	}

public:
	//typename Table<K, V>::Iterator Begin() override { return Iterator(_data); }
	//typename Table<K, V>::Iterator End() override { return Iterator(_data + _capacity); }
	//typename Table<K, V>::ConstIterator Begin() const override { return const_cast<ArrayTable*>(this)->Begin(); }
	//typename Table<K, V>::ConstIterator End() const override { return const_cast<ArrayTable*>(this)->End(); }

public:
	void Remove(const K& key) override;
	V* Find(const K& key) const override;

	~ArrayTable() override { delete[] _data; }

protected:
	/**
	 * \brief
	 *
	 * Inherits return key data index if it present
	 * Otherwise, return -1;
	 *
	 * \param key - element key
	 * \return index
	 */
	virtual int FindPosition(const K& key) const = 0;

private:
	std::pair<TRecord*, size_t> AllocateEnsuredCapacity() const;

protected:
	void EnsureCapacity();

private:
	void EnsureCapacityIfDataIsFull();

protected:
	void PushBack(const TRecord& record);
	void Insert(const TRecord& record, size_t position);
	void Remove(size_t position);


};

template <class K, class V>
void ArrayTable<K, V>::Remove(const K& key)
{
	auto position = FindPosition(key);

	if (position >= 0)
	{
		Remove(position);
	}
	
}

template <class K, class V>
V* ArrayTable<K, V>::Find(const K& key) const
{
	int position = FindPosition(key);

	if (position >= 0)
	{
		return _data[position].value;
	}

	return nullptr;
}

template <class K, class V>
std::pair<typename ArrayTable<K, V>::TRecord*, size_t> ArrayTable<K, V>::AllocateEnsuredCapacity() const
{
	auto ensuredCapacity = _capacity * _ensureMultiplier;
	auto* ensuredData = new Record<K, V>[ensuredCapacity];

	return { ensuredData, ensuredCapacity };
}

template <class K, class V>
void ArrayTable<K, V>::EnsureCapacity()
{
	auto pair = AllocateEnsuredCapacity();

	auto ensuredData = pair.first;
	auto ensuredCapacity = pair.second;

	std::copy(_data, _data + _capacity, ensuredData);

	delete[] _data;

	_data = ensuredData;
	_capacity = ensuredCapacity;
}

template <class K, class V>
void ArrayTable<K, V>::EnsureCapacityIfDataIsFull()
{
	if (_size == _capacity)
	{
		EnsureCapacity();
	}
}

template <class K, class V>
void ArrayTable<K, V>::PushBack(const TRecord& record)
{
	EnsureCapacityIfDataIsFull();
	_data[_size++] = record;
}

template <class K, class V>
void ArrayTable<K, V>::Insert(const TRecord& record, size_t position)
{
	if (position < _size)	//OR ELSE DISCONTINUOUS INSERTION DISALLOWED
	{
		if (_size < _capacity)
		{
			for (size_t i = _size; i > position; i++)
			{
				_data[i] = _data[i - 1];
			}

			_data[position] = record;

		} else
		{
			auto [ensuredData, ensuredCapacity] = 
				AllocateEnsuredCapacity();

			std::copy(
				_data,
				_data + position,
				ensuredData
			);

			_data[position] = record;

			std::copy(
				_data + position + 1, 
				_data + _size,
				ensuredData + position + 1
			);

			delete[] _data;

			_data = ensuredData;
			_capacity = ensuredCapacity;
		}

		_size++;
	}
}

template <class K, class V>
void ArrayTable<K, V>::Remove(size_t position)
{
	if (position >= _size)
	{
		return;
	}

	for (size_t i = position; i < _size - 1; i++)
	{
		_data[i] = _data[i + 1];
	}

	_size--;
}

