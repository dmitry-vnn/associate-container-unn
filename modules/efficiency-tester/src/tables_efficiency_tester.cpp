#include "tables_efficiency_tester.h"

#include "literary_text_parser.h"

#include <chrono>

#include "ordered_table.h"
#include "scan_table.h"
#include "hash_table.h"
#include "tree_table.h"

using Time = std::chrono::time_point<std::chrono::system_clock>;

Time TimeNow()
{
	return std::chrono::system_clock::now();
}

double DurationTimeSeconds(const Time& t1, const Time& t2)
{
	std::chrono::duration
		<double, std::chrono::seconds::period>

	duration = t2 - t1;

	return duration.count();
}

void TablesEfficiencyTester::Parse()
{
	auto startTime = TimeNow();

	LiteraryTextParser parser(
		R"(..\..\..\..\..\text.txt)", 
		[this] (const std::wstring& word)
		{
			_parsedWords.push_back(word);
		}
	);

	parser.Parse();

	auto endTime = TimeNow();

	_parsingWordsFromFileDurationSeconds = 
		DurationTimeSeconds(startTime, endTime);
}

void TablesEfficiencyTester::TestTable(
	const std::pair<const TableType, std::unique_ptr<Table<std::wstring, size_t>>>& record)
{

	auto& statistic = _statistic[record.first];
	auto& table = record.second;



	auto startTime = TimeNow();

	size_t iterations = 0;

	for (const auto & word : _parsedWords)
	{
		auto iterator = table->Find(word);

		iterations += table->GetLastIterationCount();

		if (iterator == table->End())
		{
			table->Add(word, 1);
			iterations += table->GetLastIterationCount();
		} else
		{
			iterator->value++;
		}
	}

	auto endTime = TimeNow();

	statistic.groupRecordsTimeMillis = 
		DurationTimeSeconds(startTime, endTime);

	statistic.groupRecordsIterations = iterations;






	_uniqueWordsCount = table->Size();

	iterations = 0;

	startTime = TimeNow();

	for (const auto& word : _parsedWords)
	{
		auto iterator = table->Find(word);
		iterations += table->GetLastIterationCount();
	}

	endTime = TimeNow();

	statistic.findRecordsTimeMillis =
		DurationTimeSeconds(startTime, endTime);

	statistic.findRecordsIterations = iterations;




	iterations = 0;

	startTime = TimeNow();

	for (const auto & word : _parsedWords)
	{
		table->Remove(word);
		iterations += table->GetLastIterationCount();
	}

	endTime = TimeNow();

	statistic.removeRecordsTimeMillis =
		DurationTimeSeconds(startTime, endTime);

	statistic.removeRecordsIterations = iterations;




	iterations = 0;

	startTime = TimeNow();

	for (const auto& word : _parsedWords)
	{
		table->Add(word, 0);
		iterations += table->GetLastIterationCount();
	}

	endTime = TimeNow();

	statistic.addRecordsTimeMillis =
		DurationTimeSeconds(startTime, endTime);

	statistic.addRecordsIterations = iterations;
	
}

void TablesEfficiencyTester::TestTables()
{

	using K = std::wstring;
	using V = size_t;


	size_t initialSize = 30000;

	_tables.insert({
		TableType::SCAN_TABLE,
		std::make_unique<ScanTable<K, V>>(initialSize)
	});
	
	_tables.insert({
		TableType::ORDERED_TABLE,
		std::make_unique<OrderedTable<K, V>>(initialSize)
	});

	_tables.insert({
		TableType::HASH_TABLE,
		std::make_unique<HashTable<K, V>>(initialSize)
	});

	_tables.insert({
		TableType::TREE_TABLE,
		std::make_unique<TreeTable<K, V>>()
	});


	auto iterator = _tables.begin();

	while (iterator != _tables.end())
	{
		_statistic.insert({iterator->first, {}});

		++iterator;
	}


	for (auto& record : _tables)
	{
		TestTable(record);
	}
}

void TablesEfficiencyTester::Test()
{
	TestTables();
}
