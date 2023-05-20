#include <iomanip>

#include "tables_efficiency_tester.h"


int main()
{

	TablesEfficiencyTester tester;

	tester.Parse();

	printf(
		"parsing file time (s): %.3f\n",

		tester.GetParsingWordsFromFileDurationSeconds()
	);

	printf(
		"total words count: %llu\n",

		tester.GetWordsCount()
	);

	tester.Test();

	printf(
		"unique words count: %llu\n",

		tester.GetUniqueWordsCount()
	);

	auto totalStatistic = tester.GetStatistic();

	printf(
		"%-15s%-15s%-20s%-15s%-20s%-15s%-20s%-15s%-20s\n",

		"TABLE",
		"GROUP TIME",
		"GROUP ITERS",
		"FIND TIME",
		"FIND ITERS",
		"REMOVE TIME",
		"REMOVE ITERS",
		"ADD TIME",
		"ADD ITERS"
	);

	printf("%s\n", std::string(15 * 5 + 20 * 4, '-').c_str());

	for (const auto & record : totalStatistic)
	{
		auto tableType = record.first;
		auto& statistic = record.second;

		std::string tableName;
		switch (tableType)
		{
			case TableType::SCAN_TABLE:
				tableName = "ScanTable";
				break;

			case TableType::ORDERED_TABLE:
				tableName = "OrderedTable";
				break;

			case TableType::HASH_TABLE:
				tableName = "HashTable";
				break;

			case TableType::TREE_TABLE:
				tableName = "TreeTable";
				break;
		}

		printf(
			"%-15s%-15.3f%-20llu%-15.3f%-20llu%-15.3f%-20llu%-15.3f%-20llu\n",
			
			tableName.c_str(),
			statistic.groupRecordsTimeMillis,
			statistic.groupRecordsIterations,
			statistic.findRecordsTimeMillis,
			statistic.findRecordsIterations,
			statistic.removeRecordsTimeMillis,
			statistic.removeRecordsIterations,
			statistic.addRecordsTimeMillis,
			statistic.addRecordsIterations
		);
	}

	return 0;
}
