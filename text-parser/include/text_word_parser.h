#pragma once

#include <iostream>
#include <functional>

class TextWordParser
{
	
private:
	using WordHandler = std::function<void(std::wstring)>;
	using CharPredicate = std::function<bool(wchar_t)>;

private:
	std::string _filePath;

	wchar_t _wordSplitter;

	CharPredicate _charFilter;
	WordHandler _wordHandler;

public:

	TextWordParser(
		std::string filePath,
		CharPredicate charFilter,
		WordHandler wordHandler,
		wchar_t wordSplitter

	):
		_filePath(std::move(filePath)),
		_wordSplitter(wordSplitter),
		_charFilter(std::move(charFilter)),
		_wordHandler(std::move(wordHandler))
	{ Parse(); }

private:
	void Parse() const;
};
