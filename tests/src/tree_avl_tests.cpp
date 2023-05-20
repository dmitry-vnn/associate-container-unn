#include <gtest/gtest.h>

#include "tree_table.h"


TEST(TreeAvlTable, init)
{
	TreeTable<int, int> table;

	table.Add(1, 0);
	table.Add(5, 0);

	EXPECT_EQ(table.Size(), 2);
}

TEST(TreeAvlTable, iter)
{
	TreeTable<std::string, int> table;

	table.Add("x", 0);
	table.Add("y", 0);
	table.Add("a", 0);
	table.Add("m", 0);
	table.Add("l", 0);
	table.Add("b", 0);
	table.Add("w", 0);
	table.Add("z", 0);
	table.Add("d", 0);

	std::vector<std::string> correct = {
		"a",
		"b",
		"d",
		"l",
		"m",
		"w",
		"x",
		"y",
		"z"
	};

	auto correctIterator = correct.begin();

	for (auto iterator = table.Begin(); iterator != table.End(); ++iterator)
	{
		EXPECT_EQ(iterator->key, *(correctIterator++));
	}
}

TEST(TreeAvlTable, remove_iter)
{
	TreeTable<int, int> table;

	table.Add(1, 0);
	table.Add(5, 0);

	auto i = table.Begin();
	while (i != table.End())
	{
		i = table.Remove(i->key);
	}

	EXPECT_EQ(table.Size(), 0);
}

TEST(TreeAvlTable, find)
{
	TreeTable<int, int> table;

	std::vector keys = { 1, 5, -4, 3 };

	for (int key : keys)
	{
		table.Add(key, 10 * key);
	}

	for (int key : keys)
	{
		EXPECT_EQ(table.Find(key)->value, 10 * key);
	}
}

TEST(TreeAvlTable, remove_not_exists)
{
	TreeTable<int, int> table;

	table.Remove(1);
	table.Remove(2);
}
