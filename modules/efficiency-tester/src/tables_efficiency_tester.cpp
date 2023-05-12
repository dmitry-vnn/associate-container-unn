#include "tables_efficiency_tester.h"

#include "hash_table.h"

void TablesEfficiencyTester::DoIt()
{
	HashTable<int, int> table(1);

	table.Print();
}
