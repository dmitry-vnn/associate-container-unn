#pragma once
#include <iterator>
#include <memory>


template<class K, class V>
struct Record {

	K key;
	V value;	//value must be movable

	Record(const K& key, V value) : key(key), value(std::move(value)) {}

	Record(const Record&) = delete;
	Record(Record&&) noexcept = default;


	Record& operator=(const Record&) = delete;
	Record& operator=(Record&&) noexcept = default;

	~Record() = default;
};

template<class K, class V>
class VirtualTableIterator {

public:

	using TypedRecord = Record<K, V>;
	using RecordPointer = TypedRecord*;
	using RecordReference = TypedRecord&;

protected:

	RecordPointer _currentRecord;
	explicit VirtualTableIterator(RecordPointer currentRecord) : _currentRecord(currentRecord) {}

public:

	virtual ~VirtualTableIterator() = default;

	VirtualTableIterator(const VirtualTableIterator&) = default;
	VirtualTableIterator& operator=(const VirtualTableIterator&) = default;

public:
	virtual bool operator!=(const VirtualTableIterator& other) const
	{
		return _currentRecord != other._currentRecord;
	}

	virtual VirtualTableIterator& operator++() = 0;
	virtual RecordReference operator*() const { return *_currentRecord; }

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

protected:
	using TypedRecord = Record<K, V>;

public:

	virtual void Add(const K& key, V* value) = 0;
	virtual V* Find(const K& key) const = 0;
	virtual void Remove(const K& key) = 0;

public:


	using Iterator = TableIterator<K, V>;
	using ConstIterator = TableIterator<const K, V>;


	virtual Iterator Begin() = 0;
	virtual Iterator End() = 0;
	
	virtual ConstIterator Begin() const = 0;
	virtual ConstIterator End() const = 0;

public:

	virtual ~Table() = default;
};


