#pragma once

#include "array_table.h" 

template<class K, class V>
class ScanTable final : public ArrayTable<K, V>
{

private:
	typedef typename Table<K, V>::TRecord TRecord;
	typedef ArrayTable<K, V> base;

public:
	explicit ScanTable(size_t initialCapacity):
		base(initialCapacity) {}

	ScanTable(std::initializer_list<TRecord> list):
		base(list.begin(), list.end()) {}

public:
	void Add(const K& key, V* value) override;

protected:
	int FindPosition(const K& key) override;

public:
	~ScanTable() override = default;
};

template <class K, class V>
void ScanTable<K, V>::Add(const K& key, V* value)
{
	base::PushBack("");
}

template <class K, class V>
V* ScanTable<K, V>::Find(const K& key) const
{

	auto iterator = base::Begin();

	while (iterator != base::End())
	{
		auto record = *iterator;

		if (record.key == key)
		{
			return record.value;
		}
	}

	return nullptr;

}
