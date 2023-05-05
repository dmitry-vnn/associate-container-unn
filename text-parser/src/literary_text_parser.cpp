#include "literary_text_parser.h"

LiteraryTextParser::LiteraryTextParser(std::string filePath, WordHandler wordHandler): _parser(nullptr)
{
	std::function<bool(wchar_t)> charFilter = [](wchar_t c)
	{
		return
			c == L'\''
			|| c >= L'а' && c <= L'я'
			|| c >= L'А' && c <= L'Я'
			|| c >= L'a' && c <= L'z'
			|| c >= L'A' && c <= L'Z'
			|| c >= L'0' && c <= L'9';
	};

	std::vector<auto> splitters = {L' '};

	_parser = new auto(
		std::move(filePath), std::move(charFilter),
		std::move(wordHandler), std::move(splitters)
	);
}

LiteraryTextParser::~LiteraryTextParser()
{
	delete _parser;
}
