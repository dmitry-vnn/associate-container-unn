#pragma once
#include <iterator>

namespace detail
{
	template<class K, class V>
	struct Record {
		K key;
		V* value;

		Record(const K& key = K(), V* value = nullptr) : key(key), value(value) {}
	};



}

using namespace detail;

/*template<class K, class V>
class TableIterator : public std::iterator<std::input_iterator_tag, Record<K, V>> {

private:

	typedef Record<K, V> TRecord;

protected:

	TRecord* _recordCursor;
	explicit TableIterator(TRecord* record) : _recordCursor(record) {}

public:

	virtual ~TableIterator() = default;

	TableIterator(const TableIterator& other) = default;
	TableIterator& operator=(const TableIterator & other) = default;

public:
	virtual bool operator!=(TableIterator const& other) const { return _recordCursor != other._recordCursor; }
	virtual TableIterator& operator++() = 0;
	virtual typename TableIterator::reference operator*() const { return *_recordCursor; }

}; */

template<class K, class V>
class Table
{

public:
	typedef Record<K, V> TRecord;

public:

	virtual void Add(const K& key, V* value) = 0;
	virtual V* Find(const K& key) const = 0;
	virtual void Remove(const K& key) = 0;

public:

	//typedef TableIterator<K, V> Iterator;
	//typedef TableIterator<const K, V> ConstIterator;
	//
	//virtual Iterator Begin() = 0;
	//virtual Iterator End() = 0;
	//
	//virtual ConstIterator Begin() const = 0;
	//virtual ConstIterator End() const = 0;

public:

	virtual ~Table() = default;
};


