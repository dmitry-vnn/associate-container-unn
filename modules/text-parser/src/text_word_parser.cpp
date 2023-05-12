#include "text_word_parser.h"

#include <codecvt>
#include <fstream>
#include <string>

std::wstring SelectWordFromLineBySplitterBound(const std::wstring& line, int wordStartIndex, int wordEndIndex)
{
	if (wordStartIndex >= 0 && wordEndIndex >= wordStartIndex)
	{
		return line.substr(wordStartIndex,
			wordEndIndex - wordStartIndex + 1
		);
	}

	return L"";
}


bool TextWordParser::IsSplitter(wchar_t c) const
{
	return
		std::find(
			_wordSplitters.begin(),
			_wordSplitters.end(),
			c
		) != _wordSplitters.end();
}

void TextWordParser::Parse() const
{
	std::wifstream wif(_filePath);

	if (!wif.is_open())
	{
		throw std::logic_error("File not found!");
	}

	wif.imbue(
		std::locale(std::locale::empty(), 
			new std::codecvt_utf8<wchar_t>)
	);

	std::wstring line;

	while (std::getline(wif, line))
	{
		int wordStartIndex = -1;

		for (int i = 0; i < line.size(); i++)
		{
			wchar_t c = line[i];

			bool isLastCharInLine = i == static_cast<int>(line.size()) - 1;

			if (isLastCharInLine || IsSplitter(c))
			{
				int wordEndIndex = i - (isLastCharInLine ? 0 : 1);

				auto str = SelectWordFromLineBySplitterBound(
					line,
					wordStartIndex,
					wordEndIndex
				);

				if (!str.empty())
				{
					auto* buffer = new wchar_t[str.size() + 1];
					size_t index = 0;

					for (auto symbol : str)
					{
						if (_charFilter(symbol))
						{
							buffer[index++] = symbol;
						}
					}

					buffer[index++] = L'\0';

					_wordHandler(std::wstring(buffer, buffer + index));
					delete[] buffer;
				}


				wordStartIndex = wordEndIndex = -1;
			}
			else 
			{
				if (wordStartIndex == -1) {
					wordStartIndex = i;
				}
			}
		}
	}

	wif.close();
}
