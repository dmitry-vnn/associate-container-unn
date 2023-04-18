#pragma once

#include "table.h"

template<class K, class V>
class FakeIterator final : public VirtualTableIterator<K, V>
{
public:
	explicit FakeIterator(typename VirtualTableIterator<K, V>::RecordPointer currentRecord)
		: VirtualTableIterator<K, V>(currentRecord)
	{}

	VirtualTableIterator<K, V>& operator++() override
	{
		return *this;
	}
};

template<class K, class V>
class FakeTable final : public Table<K, V>
{

public:
	void Add(const K& key, V value) override;
	typename Table<K, V>::ConstIterator Find(const K& key) const override;
	void Remove(const K& key) override;
	typename Table<K, V>::Iterator Begin() override;
	typename Table<K, V>::Iterator End() override;
	typename Table<K, V>::ConstIterator Begin() const override;
	typename Table<K, V>::ConstIterator End() const override;
};

template <class K, class V>
void FakeTable<K, V>::Add(const K& key, V value)
{
}

template <class K, class V>
typename Table<K, V>::ConstIterator FakeTable<K, V>::Find(const K& key) const
{
	return End();
}

template <class K, class V>
void FakeTable<K, V>::Remove(const K& key)
{

}

template <class K, class V>
typename Table<K, V>::Iterator FakeTable<K, V>::Begin()
{
	return const_cast<const FakeTable*>(this)->Begin();
}

template <class K, class V>
typename Table<K, V>::Iterator FakeTable<K, V>::End()
{
	return const_cast<const FakeTable*>(this)->End();
}

template <class K, class V>
typename Table<K, V>::ConstIterator FakeTable<K, V>::Begin() const
{
	return TableIterator<K, V>(std::make_unique<FakeIterator<K, V>>(nullptr));
}

template <class K, class V>
typename Table<K, V>::ConstIterator FakeTable<K, V>::End() const
{
	return Begin();
}
