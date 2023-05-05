#include <codecvt>
#include <gtest/gtest.h>

#include "text_word_parser.h"

TEST(TextParser, test_file_input)
{

	std::locale system("");
	std::locale::global(system);

	std::function predicate =
		[] (wchar_t) { return true; };

	std::function handler =
		[] (const std::wstring& str)
		{
			std::wcout << str << std::endl;
		};

	std::vector splitters = { L' ', L'\n', L'\0'};

	TextWordParser parser(
		R"(..\..\..\text.txt)", 
		predicate, handler,
		std::move(splitters)
	);
}