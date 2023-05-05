#pragma once

#include <functional>
#include <string>

#include "text_word_parser.h"

class LiteraryTextParser
{

private:
	using WordHandler = TextWordParser::WordHandler;

private:
	TextWordParser* _parser;

public:
	LiteraryTextParser(std::string filePath, WordHandler wordHandler);
	~LiteraryTextParser();


public:
	void Parse() const { _parser->Parse(); }
};
