#include <gtest/gtest.h>

#include "scan_table.h"


TEST(SCAN_TABLE, InitAndCheck)
{
	ScanTable<std::string, int> table;

	table.Add("Dmitry", new int(19));
	table.Add("Alex", new int(20));
	table.Add("Lexus", new int(33));
	table.Add("John", new int(33));
	table.Add("Carl", new int(32));
	table.Add("Morgan", new int(53));

	table.Remove("John");

	EXPECT_EQ(
		table.Find("John"), 
		nullptr
	);

	EXPECT_EQ(
		*table.Find("Carl"),
		32
	);

	std::vector<int, int> v;
	v.begin();



	/*
	 *	MEMORY LEAK
	 *	MAKE RECORD VALUE AS UNIQUE_PTR
	 *	AND EVEN BETTER TO MAKE VALUE AS ANY MOVED TYPE
	*/
}