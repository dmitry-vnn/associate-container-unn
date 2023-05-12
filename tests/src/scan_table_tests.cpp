#ifdef ___DISABLE___

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

TEST(SCAN_TABLE, CheckEnsure)
{
	struct Load
	{

		std::string
			firstName,
			lastName;

		size_t age;

		Load(std::string firstName, std::string lastName, size_t age)
			: firstName(std::move(firstName)),
			  lastName(std::move(lastName)),
			  age(age)	{}

		~Load()
		{
			std::cout << "Load destroyed" << std::endl;
		}
	};

	ScanTable<int, std::unique_ptr<Load>> table(5);

	table.Add(-29, std::make_unique<Load>("A1", "B", 44));
	table.Add(-21, std::make_unique<Load>("A2", "D", 44));
	table.Add(-255, std::make_unique<Load>("A3", "F", 44));
	table.Add(555, std::make_unique<Load>("A4", "H", 44));
	table.Add(5, std::make_unique<Load>("A5", "J", 4554));

	table.Remove(555);

	table.Add(324, std::make_unique<Load>("A6", "L", 4554));
	table.Add(200, std::make_unique<Load>("A7", "N", 4554));
	
	table.Remove(-29);
	
	table.Add(250, std::make_unique<Load>("A8", "P", 4554));
	table.Add(260, std::make_unique<Load>("A9", "R", 4554));
	
	table.Remove(250);

	auto iterator = table.Begin();

	//while (iterator != table.End())
	//{
	//	std::cout << iterator->key << std::endl;
	//	++iterator;
	//}

	EXPECT_EQ(    iterator->value->firstName, "A2");
	EXPECT_EQ((++iterator)->value->firstName, "A3");
	EXPECT_EQ((++iterator)->value->firstName, "A5");
	EXPECT_EQ((++iterator)->value->firstName, "A6");
	EXPECT_EQ((++iterator)->value->firstName, "A7");
	EXPECT_EQ((++iterator)->value->firstName, "A9");
	EXPECT_EQ((++iterator), table.End());

	iterator = table.Begin();

	while (iterator != table.End())
	{
		table.Remove(iterator->key);
	}

	EXPECT_EQ(table.Size(), 0);
}

#endif