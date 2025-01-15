#ifndef TEXT_UTILITY_H_
#define TEXT_UTILITY_H_

#include <string>
#include <vector>

namespace text_utility
{
	template <typename StringType>
	void TextToLines(const StringType& text, std::vector<StringType>& lines)
	{
		StringType temp{};
		for (size_t i = 0; i < text.size(); ++i)
		{
			if (text[i] == static_cast<typename StringType::value_type>('\r') ||
				text[i] == static_cast<typename StringType::value_type>('\n'))
			{
				if (!temp.empty())
				{
					lines.push_back(temp);
					temp.clear();
				}
				continue;
			}
			temp.push_back(text[i]);
		}
		if (!temp.empty())
		{
			lines.push_back(temp);
		}
	}

	template <typename StringType, typename CharType>
	void SplitTextBySeparator(const StringType& text, const CharType separator, std::vector<StringType>& splits)
	{
		for (size_t nRead = 0; nRead < text.size();)
		{
			size_t nPos = text.find(separator, nRead);
			if (nPos == StringType::npos)
			{
				size_t nLen = text.size() - nRead;
				splits.emplace_back(text.substr(nRead, nLen));
				break;
			}

			size_t nLen = nPos - nRead;
			splits.emplace_back(text.substr(nRead, nLen));
			nRead += nLen + 1;
		}
	}

	template <typename StringType>
	void ReplaceAll(StringType& src, const StringType& strOld, const StringType& strNew)
	{
		if (strOld == strNew) return;

		for (size_t nRead = 0;;)
		{
			size_t nPos = src.find(strOld, nRead);
			if (nPos == StringType::npos) break;
			src.replace(nPos, strOld.size(), strNew);
			nRead = nPos + strNew.size();
		}
	}
	inline void ReplaceAll(std::wstring& text, const wchar_t* oldStr, const wchar_t* newStr)
	{
		ReplaceAll<std::wstring>(text, std::wstring(oldStr), std::wstring(newStr));
	}

	inline void ReplaceAll(std::string& text, const char* oldStr, const char* newStr)
	{
		ReplaceAll<std::string>(text, std::string(oldStr), std::string(newStr));
	}

	template <typename StringType>
	StringType ExtractDirectory(StringType& filePath)
	{
		size_t nPos = filePath.find_last_of(static_cast<typename StringType::value_type>("\\/"), nPos);
		if (nPos != std::wstring::npos)
		{
			return filePath.substr(0, nPos);
		}
		return filePath;
	}
}

#endif // !TEXT_UTILITY_H_
