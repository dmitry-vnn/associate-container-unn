#include "tables_efficiency_tester.h"

#include "hash_table.h"
#include "literary_text_parser.h"

#include <chrono>

#include "ordered_table.h"
#include "scan_table.h"

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

	for (const auto & word : _parsedWords)
	{
		auto iterator = table->Find(word);
		if (iterator == table->End())
		{
			table->Add(word, 1);
		} else
		{
			iterator->value++;
		}
	}

	auto endTime = TimeNow();

	statistic.addRecordsTotalDurationTimeMillis = 
		DurationTimeSeconds(startTime, endTime);

	startTime = TimeNow();


	statistic.totalRecordsCount = table->Size();


	for (const auto& word : _parsedWords)
	{
		auto iterator = table->Find(word);
	}

	endTime = TimeNow();

	statistic.findRecordTotalDurationTimeMillis =
		DurationTimeSeconds(startTime, endTime);

	startTime = TimeNow();

	for (const auto & word : _parsedWords)
	{
		table->Remove(word);
	}

	endTime = TimeNow();

	statistic.removeRecordsTotalDurationTimeMillis =
		DurationTimeSeconds(startTime, endTime);

}

void TablesEfficiencyTester::TestTables()
{

	using K = std::wstring;
	using V = size_t;

	std::map<
		TableType,
		std::unique_ptr<Table<K, V>>
	> tables;

	size_t initialSize = 30000;

	tables.insert({
		TableType::SCAN_TABLE,
		std::make_unique<ScanTable<K, V>>(initialSize)
	});
	
	tables.insert({
		TableType::ORDERED_TABLE,
		std::make_unique<OrderedTable<K, V>>(initialSize)
	});

	tables.insert({
		TableType::HASH_TABLE,
		std::make_unique<HashTable<K, V, true>>(initialSize)
	});


	auto iterator = tables.begin();

	while (iterator != tables.end())
	{
		_statistic.insert({iterator->first, {}});

		++iterator;
	}


	for (auto& record : tables)
	{
		TestTable(record);
	}
}

void TablesEfficiencyTester::Test()
{
	Parse();

	TestTables();
}
