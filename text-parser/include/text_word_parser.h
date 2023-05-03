#pragma once

#include <iostream>
#include <vector>
#include <functional>

class TextWordParser
{
	
private:
	using WordHandler = std::function<void(std::wstring)>;
	using WordPredicate = std::function<bool(std::wstring)>;

private:
	std::string _filePath;
	WordPredicate _wordFilter;
	char _wordSplitter;

public:
	TextWordParser(
		std::string filePath,
		WordPredicate wordFilter,
		char wordSplitter
	):
		_filePath(std::move(filePath)),
		_wordFilter(std::move(wordFilter)),
		_wordSplitter(wordSplitter) {}
};
