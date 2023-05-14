#include <iomanip>

#include "tables_efficiency_tester.h"


int main()
{

	TablesEfficiencyTester tester;

	tester.Test();

	auto totalStatistic = tester.GetStatistic();

	printf(
		"parsing file time (s): %.3f\n",

		tester.GetParsingWordsFromFileDurationSeconds()
	);

	printf("tables info:\n");
	printf(
		"%-15s%-15s%-15s%-15s%-15s\n",

		"TABLE",
		"ADD TIME (s)",
		"FIND TIME",
		"REMOVE TIME",
		"RECORDS"
	);

	printf("%s\n", std::string(15 * 5, '-').c_str());

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
			"%-15s%-15.3f%-15.3f%-15.3f%-15llu\n",
			
			tableName.c_str(),
			statistic.addRecordsTotalDurationTimeMillis,
			statistic.findRecordTotalDurationTimeMillis,
			statistic.removeRecordsTotalDurationTimeMillis,
			statistic.totalRecordsCount
		);
	}

	return 0;
}
