#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "table.h"

struct TableStatistic
{
	double addRecordsTotalDurationTimeMillis = 0;
	double removeRecordsTotalDurationTimeMillis = 0;
	double findRecordTotalDurationTimeMillis = 0;

	size_t totalRecordsCount = 0;

	TableStatistic() = default;
};

enum class TableType
{

	SCAN_TABLE,
	ORDERED_TABLE,
	HASH_TABLE,
	TREE_TABLE


};

class TablesEfficiencyTester
{

private:
	std::vector<std::wstring> _parsedWords;

private:
	std::map<TableType, TableStatistic> _statistic;

private:
	double _parsingWordsFromFileDurationSeconds;

public:
	TablesEfficiencyTester() = default;

	void Test();

private:
	void Parse();

	void TestTable(
		const std::pair<
			const TableType, 
			std::unique_ptr<Table<std::wstring, size_t>>
		>& record);

	void TestTables();

public:

	std::map<TableType, TableStatistic> GetStatistic() const
	{
		return _statistic;
	}

	double GetParsingWordsFromFileDurationSeconds() const
	{
		return _parsingWordsFromFileDurationSeconds;
	}
};
