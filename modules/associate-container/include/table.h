#pragma once

#include <iterator>
#include <memory>

template <class K, class V>
struct Record {

	K key;		//key can be movable
	V value;	//value can be movable

	Record(K key, V value): key(std::move(key)), value(std::move(value)) {}
	Record(): key(), value() {}


	Record(Record&&) noexcept = default;
	Record& operator=(Record&&) noexcept = default;


	Record(const Record&) = default;
	Record& operator=(const Record&) = default;

	~Record() = default;

	static bool Compare(const Record& r1, const Record& r2);
	static bool CompareKeys(const K& k1, const K& k2);
};

template <class K, class V>
bool Record<K, V>::Compare(const Record& r1, const Record& r2)
{
	return CompareKeys(r1.key, r2.key);
}

template <class K, class V>
bool Record<K, V>::CompareKeys(const K& k1, const K& k2)
{
	return k1 < k2;
}


template<class K, class V>
class TableIterator;

template<class K, class V>
class VirtualTableIterator {

	//friend class TableIterator<K, V>;

public:

	//using TypedRecord = Record<K, V>;
	//using RecordPointer = TypedRecord*;
	//using RecordReference = TypedRecord&;

protected:

	/*RecordPointer _currentRecord;
	explicit VirtualTableIterator(RecordPointer currentRecord):
		_currentRecord(currentRecord) {} */

public:
	VirtualTableIterator() = default;
	virtual ~VirtualTableIterator() = default;

	VirtualTableIterator(const VirtualTableIterator&) = default;
	VirtualTableIterator& operator=(const VirtualTableIterator&) = default;

public:
	virtual bool operator!=(const VirtualTableIterator& other) const = 0;

	virtual bool operator==(const VirtualTableIterator& other) const = 0;

	virtual Record<K, V>& operator*() const = 0;

	virtual VirtualTableIterator& operator++() = 0;

	virtual std::unique_ptr<VirtualTableIterator> Copy() = 0;

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


	

	TableIterator(const TableIterator& other)
		: std::iterator<std::input_iterator_tag, Record<K, V>>(other),
		  _iterator(std::move(other._iterator->Copy()))
	{
	}
	
	TableIterator& operator=(const TableIterator& other)
	{
		if (this == &other)
			return *this;
		std::iterator<std::input_iterator_tag, Record<K, V>>::operator =(other);
		_iterator = std::move(other._iterator->Copy());
		return *this;
	}


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

		base::operator=(other);

		_iterator = std::move(other._iterator);
		return *this;
	}

	~TableIterator() = default;

public:

	bool operator!=(const TableIterator& other) const
	{
		return !operator==(other); 
	}

	bool operator==(const TableIterator& other) const
	{
		return _iterator->operator==(*other._iterator.get());
	}

	typename TableIterator::reference operator*() const
	{
		return _iterator->operator*();
	}

	typename TableIterator::pointer operator->()
	{
		return const_cast<const TableIterator*>(this)->operator->();
	}

	
	const typename TableIterator::pointer operator->() const 
	{
		return &operator*();
	}

	TableIterator& operator++()
	{
		_iterator->operator++();
		return *this;
	}

};


template<class K, class V>
class Table
{

protected:
	using TypedRecord = Record<K, V>;
	using RecordPointer = TypedRecord*;
	using RecordReference = TypedRecord&;

	using Iterator = TableIterator<K, V>;
	using ConstIterator = Iterator;

public:

	virtual void Add(const K& key, V value) = 0;
	virtual ConstIterator Find(const K& key) const = 0;
	virtual ConstIterator Remove(const K& key) = 0;
	virtual size_t Size() = 0;

public:

	virtual Iterator Begin() = 0;
	virtual Iterator End() = 0;
	
	virtual ConstIterator Begin() const = 0;
	virtual ConstIterator End() const = 0;

public:

	virtual ~Table() = default;
};


