#pragma once

#include <iostream>
#include <map>
#include <vector>

#include "table.h"

struct TableStatistic
{
	double groupRecordsTimeMillis = 0;

	double addRecordsTimeMillis = 0;
	double removeRecordsTimeMillis = 0;
	double findRecordsTimeMillis = 0;



	size_t groupRecordsIterations = 0;

	size_t addRecordsIterations = 0;
	size_t removeRecordsIterations = 0;
	size_t findRecordsIterations = 0;

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

	std::map<
		TableType,
		std::unique_ptr<Table<std::wstring, size_t>>
	> _tables;

private:
	std::map<TableType, TableStatistic> _statistic;

private:
	double _parsingWordsFromFileDurationSeconds;

	size_t _uniqueWordsCount;

public:
	TablesEfficiencyTester() = default;

	void Parse();
	void Test();

private:

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

	size_t GetWordsCount() const
	{
		return _parsedWords.size();
	}

	size_t GetUniqueWordsCount() const
	{
		return _uniqueWordsCount;
	}
};
