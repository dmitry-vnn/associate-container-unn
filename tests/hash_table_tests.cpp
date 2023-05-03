#include <gtest/gtest.h>

#include <iostream>
#include "hash_table.h"


size_t MyHash(int i)
{
	return 0;
}

TEST(HashTable, init_and_print)
{
	
	HashTable<int, int> table(8);

	table.Add(4, 99);
	table.Add(45, 99);
	table.Add(41, 99);
	table.Add(44, 99);
	table.Add(44, 100);
	table.Add(64, 99);
	table.Add(-34, 99);
	table.Add(256, 99);
	table.Add(257, 99);
	table.Add(255, 99);
	table.Add(535323, 99);
	table.Add(-234523, 99);
	table.Add(-263, 99);
	table.Add(999, 99);
	table.Add(1000 + 1, 99);
	table.Add(1000-7, 99);


	table.Print();

	std::cout << std::endl;

	auto iterator = table.Begin();

	while (iterator != table.End())
	{
		std::cout << iterator->key << " -> " << iterator->value << std::endl;
		++iterator;
	}

	iterator = table.Begin();

	while (iterator != table.End())
	{
		iterator = table.Remove(iterator->key);
	}

	EXPECT_EQ(table.Size(), 0);

	EXPECT_EQ(table.Find(45), table.End());
	EXPECT_EQ(table.Find(1000-7), table.End());
	EXPECT_EQ(table.Find(535323), table.End());
	EXPECT_EQ(table.Find(535323), table.End());
}

TEST(HashTable, little_table_removing)
{
	HashTable<int, int> table(3);

	table.Add(100, 0);
	table.Add(50, 0);
	table.Add(663, 0);
	table.Add(44, 0);
	table.Add(4666, 0);
	table.Add(-525, 0);
	table.Add(641, 0);
	table.Add(14, 0);
	table.Add(94, 0);

	table.Remove(14);
	table.Remove(641);

	EXPECT_EQ(table.Find(14), table.End());
	EXPECT_EQ(table.Find(641), table.End());

	table.Print();
}


TEST(HashTable, remove_first_element_from_chains)
{
	HashTable<std::string, int> table(5);

	std::vector<std::string> v;

	v.emplace_back("abc");			//0
	v.emplace_back("cba");			//1
	v.emplace_back("sgesgsegs");	//2
	v.emplace_back("s385gu38g");	//3
	v.emplace_back("a");			//4
	v.emplace_back("b");			//5
	v.emplace_back("z");			//6
	v.emplace_back("x");			//7
	v.emplace_back("dd");			//8
	v.emplace_back("xx");			//9
	v.emplace_back("xdfgh");		//10
	v.emplace_back("xdf");			//11

	for (auto& s : v)
	{
		table.Add(s, 1);
	}

	table.Print();

	EXPECT_EQ(table.Find(v[0]), table.Remove(v[11]));
	EXPECT_EQ(table.Find(v[1]), table.Remove(v[0]));
	EXPECT_EQ(table.Find(v[2]), table.Remove(v[8]));
	EXPECT_EQ(table.Find(v[3]), table.Remove(v[2]));
	EXPECT_EQ(table.Find(v[6]), table.Remove(v[3]));
	EXPECT_EQ(table.Find(v[5]), table.Remove(v[6]));
	EXPECT_EQ(table.End(), table.Remove(v[10]));
	EXPECT_EQ(table.Find(v[9]), table.Remove(v[5]));

	table.Print();
}

TEST(HashTable, table_increase)
{
	HashTable<int, int, true> table(3);

	table.Add(100, 0);
	table.Add(-100, 0);
	table.Add(50, 0);
	table.Add(-50, 0);
	table.Add(25, 0);
	table.Add(-25, 0);
	table.Add(12, 0);
	table.Add(-12, 0);
	table.Add(6, 0);
	table.Add(-6, 0);
	table.Add(4, 0);
	table.Add(-4, 0);

	table.Print();
}