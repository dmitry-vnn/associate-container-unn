#include <gtest/gtest.h>

#include "fake_table.h"
#include "scan_table.h"

#include <iostream>

#include "array_table.h"

template<class K, class V>
void F(ArrayTable<K, V>* table)
{
	if (table == nullptr)
	{
		return;
	}
	auto e = table->Find(0);
}

TEST(FAKE_TABLE, TT)
{
	FakeTable<int, int> f;
	std::vector<int> v;
	v.push_back(1);

	F<int, int>(nullptr);

	EXPECT_NO_THROW(f.Add(1, 1));

	std::map<int, int> m;


	auto iterator = m.find(60);

}

TEST(SCAN_TABLE, InitAndCheck)
{
	ScanTable<std::string, int> table;
	
	table.Add("Dmitry", 19);

	auto iterator = table.Find("Dmitry");

	EXPECT_NE(iterator, table.End());

	table.Add("Alex", 20);
	table.Add("Lexus", 33);
	table.Add("John", 33);
	table.Add("Carl", 32);
	table.Add("Morgan", 53);
	
	table.Remove("John");

	EXPECT_EQ(
		table.Find("John"),
		table.End()
	);

	auto i = table.Find("Carl");

	EXPECT_TRUE(
		i != table.End() && i->value == 32
	);



	/*
	 *	MEMORY LEAK
	 *	MAKE RECORD VALUE AS UNIQUE_PTR
	 *	AND EVEN BETTER TO MAKE VALUE AS ANY MOVED TYPE
	*/
}