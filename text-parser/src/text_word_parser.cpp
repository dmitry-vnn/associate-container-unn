#include "text_word_parser.h"

#include <fstream>
#include <string>

std::wstring SelectWordFromLineBySplitterBound(const std::wstring& line, int splitterStartIndex, int splitterEndIndex)
{
	int wordStartIndex = splitterStartIndex + 1;
	int wordEndIndex = splitterEndIndex - 1;

	if (wordStartIndex >= wordEndIndex)
	{
		return line.substr(wordStartIndex,
			wordEndIndex - wordStartIndex + 1
		);
	}

	return L"";
}


void TextWordParser::Parse() const
{
	std::wifstream stream(_filePath);

	std::wstring line;

	while (std::getline(stream, line))
	{
		int splitterStartIndex = -1;
		int splitterEndIndex = -1;

		for (size_t i = 0; i < line.size(); i++)
		{
			wchar_t c = line[i];

			if (c == _wordSplitter)
			{
				if (splitterStartIndex == -1)
				{
					splitterStartIndex = i;
				} else if (splitterEndIndex == -1)
				{
					splitterEndIndex = i;
				} else
				{
					auto str = SelectWordFromLineBySplitterBound(
						line,
						splitterStartIndex,
						splitterEndIndex
					);

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

					_wordHandler(std::wstring(buffer,  buffer + index));
					delete[] buffer;

					splitterStartIndex = splitterEndIndex = -1;
				}
			}
		}
	}
}
