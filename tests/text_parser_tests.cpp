#include <codecvt>
#include <gtest/gtest.h>

#include "hash_table.h"
#include "literary_text_parser.h"
#include "ordered_table.h"
#include "scan_table.h"
#include "text_word_parser.h"

void Localize()
{
	std::locale system("");
	std::locale::global(system);
}

std::string filePath = R"(..\..\..\text.txt)";


TEST(TextParser, test_file_input)
{
	Localize();

	std::function predicate =
		[] (wchar_t) { return true; };

	std::function handler =
		[] (const std::wstring& str)
		{
			std::wcout << str << std::endl;
		};

	std::vector splitters = { L' ', L'\n', L'\0'};

	TextWordParser parser(
		filePath, 
		predicate, handler,
		std::move(splitters)
	);
}

TEST(LiteraryTextParser, word_handling)
{
	Localize();


	size_t count = 0;

	std::function handler = [&](const std::wstring& word)
	{
		count++;
	};

	LiteraryTextParser parser(filePath, std::move(handler));

	parser.Parse();

	EXPECT_EQ(114140, count);

}