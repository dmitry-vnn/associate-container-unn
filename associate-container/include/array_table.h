#pragma once

//#ifdef DEBB

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

	ArrayTableIterator& operator++() override
	{
		//Increment the record pointer (pointer arithmetic)
		++base::_currentRecord;

		return *this;
	}

	static TableIterator<K, V> Create(typename base::RecordPointer currentRecord)
	{
		return TableIterator<K, V>(std::move(std::make_unique<ArrayTableIterator>(currentRecord)));
	}

};

template<class K, class V>
class ArrayTable : public Table<K, V> {

private:

	using base = Table<K, V>;

protected:

	using TypedRecord = typename base::TypedRecord;
	using RecordPointer = typename base::RecordPointer;
	using RecordReference = typename base::RecordReference;
	using ConstRecordReference = const RecordReference;

private:
	size_t _ensureMultiplier;

protected:

	size_t _size;
	size_t _capacity;
	RecordPointer _data;


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
	typename base::ConstIterator Begin() const override { return ArrayTableIterator<K, V>::Create(_data); }
	typename base::ConstIterator End() const override { return ArrayTableIterator<K, V>::Create(_data + _size); }
	typename base::Iterator Begin() override { return const_cast<const ArrayTable*>(this)->Begin(); } //DELEGATE TO CONST METHOD INVOCATION
	typename base::Iterator End() override { return const_cast<const ArrayTable*>(this)->End(); }	 //

public:
	void Remove(const K& key) override;
	typename base::ConstIterator Find(const K& key) const override;

	~ArrayTable() override { delete[] _data; }

protected:
	/*
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
	std::pair<RecordPointer, size_t> AllocateEnsuredCapacity() const;

protected:
	void EnsureCapacity();

private:
	void EnsureCapacityIfDataIsFull();

protected:
	void PushBack(TypedRecord record);
	void Insert(TypedRecord record, size_t position);
	void RemoveByPosition(size_t position);


};

template <class K, class V>
void ArrayTable<K, V>::Remove(const K& key)
{
	auto position = FindPosition(key);

	if (position != -1)
	{
		RemoveByPosition(position);
	}
	
}

template <class K, class V>
typename Table<K, V>::ConstIterator ArrayTable<K, V>::Find(const K& key) const
{
	int position = FindPosition(key);

	auto dataOffset = _data + _size;

	if (position != -1)
	{
		dataOffset = _data + position;
	}

	return ArrayTableIterator<K, V>::Create(dataOffset);
}

template <class K, class V>
std::pair<typename ArrayTable<K, V>::RecordPointer, size_t> ArrayTable<K, V>::AllocateEnsuredCapacity() const
{
	auto ensuredCapacity = _capacity * _ensureMultiplier;
	auto ensuredData = new Record<K, V>[ensuredCapacity];

	return { ensuredData, ensuredCapacity };
}

template <class K, class V>
void ArrayTable<K, V>::EnsureCapacity()
{
	auto pair = AllocateEnsuredCapacity();

	auto ensuredData = pair.first;
	auto ensuredCapacity = pair.second;

	std::move(_data, _data + _capacity, ensuredData);

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
void ArrayTable<K, V>::PushBack(TypedRecord record)
{
	EnsureCapacityIfDataIsFull();
	_data[_size++] = std::move(record);
}

template <class K, class V>
void ArrayTable<K, V>::Insert(TypedRecord record, size_t position)
{
	if (position < _size)	//OR ELSE DISCONTINUOUS INSERTION DISALLOWED
	{
		if (_size < _capacity)
		{
			for (size_t i = _size; i > position; i++)
			{
				_data[i] = std::move(_data[i - 1]);
			}

			_data[position] = std::move(record);

		} else
		{
			auto [ensuredData, ensuredCapacity] = 
				AllocateEnsuredCapacity();

			std::move(
				_data,
				_data + position,
				ensuredData
			);

			_data[position] = std::move(record);

			std::move(
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
void ArrayTable<K, V>::RemoveByPosition(size_t position)
{
	for (size_t i = position; i < _size - 1; i++)
	{
		_data[i] = std::move(_data[i + 1]);
	}

	_size--;
}

//#endif