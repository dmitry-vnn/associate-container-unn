#pragma once
#include <iterator>
#include <memory>

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


template<class K, class V>
class VirtualTableIterator {

private:

	typedef Record<K, V> TRecord;

protected:

	TRecord* _recordCursor;
	explicit VirtualTableIterator(TRecord* record) : _recordCursor(record) {}

public:

	virtual ~VirtualTableIterator() = default;

	VirtualTableIterator(const VirtualTableIterator&) = default;
	VirtualTableIterator& operator=(const VirtualTableIterator&) = default;

public:
	virtual bool operator!=(VirtualTableIterator const& other) const
	{
		return _recordCursor != other._recordCursor;
	}
	virtual VirtualTableIterator& operator++() = 0;
	virtual TRecord operator*() const { return *_recordCursor; }

};

template<class K, class V>
class TableIterator final : public std::iterator<std::input_iterator_tag, Record<K, V>> {

private:
	using base = std::iterator<std::input_iterator_tag, Record<K, V>>;

private:
	std::unique_ptr<VirtualTableIterator<K, V>> _iterator;

public:
	explicit TableIterator(
		std::unique_ptr<VirtualTableIterator<K, V>> iteratorImplement
	): _iterator(std::move(iteratorImplement)) {}

	TableIterator(const TableIterator&) = delete;
	TableIterator& operator=(const TableIterator&) = delete;

	TableIterator(TableIterator&& other) noexcept :

		//Base move calling
		base(std::move(other)),

		//Fields move calling
		_iterator(std::move(other._iterator))

	{}


	TableIterator& operator=(TableIterator&& other) noexcept
	{
		if (this == &other)
			return *this;

		base::operator=(std::move(other));

		_iterator = std::move(other._iterator);
		return *this;
	}

	~TableIterator() = default;

public:
	bool operator!=(TableIterator const& other) const
		{ return _iterator != other._iterator; }

	TableIterator& operator++()
	{
		++_iterator;
		return *this;
	}
	typename TableIterator::reference operator*() const
	{
		return *_iterator.get();
	}

};


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

	typedef TableIterator<K, V> Iterator;
	typedef TableIterator<const K, V> ConstIterator;
	
	virtual Iterator Begin() = 0;
	virtual Iterator End() = 0;
	
	virtual ConstIterator Begin() const = 0;
	virtual ConstIterator End() const = 0;

public:

	virtual ~Table() = default;
};


