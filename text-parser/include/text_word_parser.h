#pragma once

#include <iostream>
#include <functional>
#include <vector>

class TextWordParser
{
	
private:
	using WordHandler = std::function<void(std::wstring)>;
	using CharPredicate = std::function<bool(wchar_t)>;

private:
	std::string _filePath;

	std::vector<wchar_t> _wordSplitters;

	CharPredicate _charFilter;
	WordHandler _wordHandler;

public:

	TextWordParser(
		std::string filePath,
		CharPredicate charFilter,
		WordHandler wordHandler,
		std::vector<wchar_t> wordSplitters

	):
		_filePath(std::move(filePath)),
		_wordSplitters(std::move(wordSplitters)),
		_charFilter(std::move(charFilter)),
		_wordHandler(std::move(wordHandler))
	{ }

public:
	void Parse() const;

private:
	bool IsSplitter(wchar_t c) const;
};
