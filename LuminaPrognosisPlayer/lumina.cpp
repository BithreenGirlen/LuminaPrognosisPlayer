
#include <unordered_map>

#include "lumina.h"

#include "win_filesystem.h"
#include "win_text.h"
#include "text_utility.h"

#include "deps/nlohmann/json.hpp"

namespace lumina
{
	/*Import形式からデータ抽出*/
	static std::wstring ExtractDataFromCocos2dImportFile(const std::wstring& wstrFilePath)
	{
		std::wstring wstrScenario;
		std::string strFile = win_filesystem::LoadFileAsString(wstrFilePath.c_str());
		if (!strFile.empty())
		{
			std::string strError;

			try
			{
				nlohmann::json nlJson = nlohmann::json::parse(strFile);
				std::string str;

				wstrScenario = win_text::WidenUtf8(nlJson.at(5).at(0).at(2));
			}
			catch (nlohmann::json::exception e)
			{
				strError = e.what();
			}
		}

		return wstrScenario;
	}

	struct SResourcePath
	{
		std::wstring wstrVoiceFolder;
		std::wstring wstrStillFolder;
		std::wstring wstrAnimationFolder;
	};

	static SResourcePath g_resourcePath;

	static bool DeriveResourcePathsFromScenarioFilePath(const std::wstring& wstrScenarioFilePath)
	{
		size_t nPos = wstrScenarioFilePath.rfind(L"Scenario_csv");
		if (nPos == std::wstring::npos)return false;

		std::wstring wstrResourceFolderPath = wstrScenarioFilePath.substr(0, nPos);

		g_resourcePath.wstrVoiceFolder = wstrResourceFolderPath + L"Sound\\Voice\\";
		g_resourcePath.wstrStillFolder = wstrResourceFolderPath + L"Texture\\";
		g_resourcePath.wstrAnimationFolder = wstrResourceFolderPath + L"Texture\\HCG\\Animation\\";

		return true;
	}
}

bool lumina::LoadScenario(const std::wstring& wstrFilePath, std::vector<adv::TextDatum>& textData, std::vector<adv::ImageFileDatum>& imageFileData)
{
	bool bRet = DeriveResourcePathsFromScenarioFilePath(wstrFilePath);
	if (!bRet)return false;

	std::wstring wstrScenario = ExtractDataFromCocos2dImportFile(wstrFilePath);
	if (wstrScenario.empty())return false;

	std::vector<std::wstring> lines;
	text_utility::TextToLines(wstrScenario, lines);

	std::unordered_map<std::wstring, std::wstring> characterNameMap;
	characterNameMap.insert({ L"#主人公", L"主人公" });

	std::wstring wstrTextBuffer;
	std::wstring wstrVoiceBuffer;

	for (const auto& line : lines)
	{
		std::vector<std::wstring> commands;
		text_utility::SplitTextBySeparator(line, L',', commands);

		if (commands.empty())continue;

		const std::wstring &wstrType = commands[0];
		if (wstrType == L"Character")
		{
			/*人物名ラベル*/
			if (commands.size() > 3)
			{
				characterNameMap.insert({ commands[2], commands[3] });
			}
		}
		else if (wstrType == L"Message")
		{
			/*
			* Example:
			* Message,,#主人公,「それじゃ、後はよろしく」,,
			* Message,,,捕まえた泥棒を、ネデア騎士団の/n騎士たちに引き渡した。,,
			*/
			if (commands.size() > 3)
			{
				if (!commands[2].empty())
				{
					const auto& iter = characterNameMap.find(commands[2]);
					if (iter != characterNameMap.cend())
					{
						wstrTextBuffer = iter->second;
						wstrTextBuffer += L":\n";
					}
				}
				text_utility::ReplaceAll(commands[3], L"/n", L"\n");
				wstrTextBuffer += commands[3];
			}
		}
		else if (wstrType == L"Voice")
		{
			/*
			* Example:
			* Voice,,,1103001/1103001_H1_001,,
			*/
			if (commands.size() > 3)
			{
				wstrVoiceBuffer = g_resourcePath.wstrVoiceFolder + commands[3] + L".mp3";
			}
		}
		else if (wstrType == L"Still")
		{
			/*
			* Example:
			* Still,0,,HCG/Still/1103001_1/1103001_2,,
			*/
			if (commands.size() > 3)
			{
				adv::ImageFileDatum s;
				s.bAnimation = false;
				s.wstrFilePath = g_resourcePath.wstrStillFolder + commands[3] + L".jpg";

				/*
				* 未指定・再指定の差分有り。
				* 未指定分は表情のみの差分。再指定は除外。
				*/
				const auto IsContained = [&imageFileData](const std::wstring& wstrFilePath)
					-> bool
					{
						for (const auto& imageFileDatum : imageFileData)
						{
							if (imageFileDatum.wstrFilePath == wstrFilePath)
							{
								return true;
							}
						}
						return false;
					};

				if (!IsContained(s.wstrFilePath))
				{
					imageFileData.push_back(std::move(s));
				}
			}
		}
		else if (wstrType == L"Animation")
		{
			/*ファイル探索は動作規定時に行う*/
			if (commands.size() > 3)
			{
				adv::ImageFileDatum s;
				s.bAnimation = true;
				s.wstrFilePath = g_resourcePath.wstrAnimationFolder + commands[3];
				imageFileData.push_back(std::move(s));
			}
		}

		if (!wstrTextBuffer.empty())
		{
			adv::TextDatum s;
			
			if (!wstrVoiceBuffer.empty())
			{
				s.wstrVoicePath = wstrVoiceBuffer;
			}

			s.wstrText = wstrTextBuffer;

			wstrVoiceBuffer.clear();
			wstrTextBuffer.clear();

			textData.push_back(std::move(s));
		}
	}

	return true;
}
