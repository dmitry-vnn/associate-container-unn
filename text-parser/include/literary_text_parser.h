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


public:
	~LiteraryTextParser();

	LiteraryTextParser(const LiteraryTextParser& other) = delete;
	LiteraryTextParser(LiteraryTextParser&& other) noexcept = delete;
	LiteraryTextParser& operator=(const LiteraryTextParser& other) = delete;
	LiteraryTextParser& operator=(LiteraryTextParser&& other) noexcept = delete;

public:
	void Parse() const { _parser->Parse(); }
};
