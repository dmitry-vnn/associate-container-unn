

#include <gtest/gtest.h>

#include "ordered_table.h"

template<class K, class V>
void PrintTable(const Table<K, V>& table)
{
	for (auto iterator = table.Begin(); iterator != table.End(); ++iterator)
	{
		std::cout << iterator->key << ": " << iterator->value << std::endl;
	}

	std::cout << std::endl;
}

TEST(OrderedTable, remove)
{
	OrderedTable<int, int> table;

	table.Add(52, 0);
	table.Add(5, 0);
	table.Add(10, 0);
	table.Add(155, 0);
	table.Add(5, 0);
	table.Add(38, 0);
	table.Add(47, 0);
	table.Add(21, 0);

	table.Remove(5);

	PrintTable(table);

	table.Remove(155);
	table.Remove(10);

	PrintTable(table);

	table.Remove(5);

	PrintTable(table);

}

#ifdef __

TEST(order_table_tests, init)
{

	OrderedTable<char, int> t1;

	t1.Add('a', 5);
	t1.Add('d', 6);
	t1.Add('c', 10);
	t1.Add('z', 105);
	t1.Add('f', 105);
	t1.Add('i', 104);
	t1.Add('x', 104);

	auto iterator = t1.Begin();
	iterator = t1.End();

	std::cout << iterator->key;

	OrderedTable<char, int> t2 = {
		{'a', 1},
		{'i', 1},
		{'f', 1},
		{'y', 1},
		{'c', 1},
		{'d', 1},
		{'x', 1}
	};

	auto i1 = t1.Begin();
	auto i2 = t2.Begin();

	while (i1 != t1.End())
	{
		std::cout << i1->key << " vs " << i2->key << std::endl; 
		//EXPECT_EQ(i1->key, i2->key);
		++i1;
		++i2;
	}
}

TEST(ORDER_TABLE, XUI) {

	OrderedTable<int, int> t(3);

	t.Add(10, 1);
	t.Add(14, 1);
	t.Add(16, 1);
	t.Add(34, 1);
	t.Add(8, 1);
	t.Add(7, 1);
	t.Add(5, 1);
	t.Add(6, 1);
	t.Add(6, 2);

	auto iterator = t.Begin();

	while (iterator != t.End())
	{
		std::cout << iterator->key << std::endl;
		++iterator;
	}
}

TEST(OrderTable, ManyInsertion)
{

	srand(time(nullptr));

	int min = -99999;
	int max = 99999;

	std::set<int> set;
	OrderedTable<int, char> table;


	for (size_t i = 0; i < 10000; i++)
	{
		int randomKey = rand() % (max - min + 1) + min;

		int chance = rand() % 10;

		if (chance == 0 || chance == 1)
		{
			int randomIndex = rand() % table.Size();
			int key = table._data[randomIndex].key;

			table.Remove(key);
			set.erase(key);
		} 

		set.insert(randomKey);
		table.Add(randomKey, false);
	}

	auto setIterator = set.rbegin();
	auto tableIterator = table.Begin();
	
	while (setIterator != set.rend())
	{
		EXPECT_TRUE(*setIterator == tableIterator->key);
	
	
		++setIterator;
		++tableIterator;
	}
}

#endif
