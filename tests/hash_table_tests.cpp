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

	//auto iterator = table.Begin();

	//while (iterator != table.End())
	//{
	//	std::cout << iterator->key << " -> " << iterator->value << std::endl;
	//
	//	if (iterator->key == 999)
	//	{
	//		std::cout << "";
	//	}
	//	++iterator;
	//}

	

}