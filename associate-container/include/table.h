#pragma once

template<class K, class V>
class Table
{

public:

	virtual void Add(const K& key, const V* value) = 0;
	virtual V* Find(const K& key) = 0;

	virtual void Remove(const K& key) = 0;

	virtual ~Table() = default;
};

namespace detail
{
	template<class K, class V>
	struct Record {
		K key;
		V* value;

		Record(const K& key = K(), V* value = nullptr) : key(key), value(value) {}
	};
}
