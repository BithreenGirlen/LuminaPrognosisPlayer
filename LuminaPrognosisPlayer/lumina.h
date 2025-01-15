#ifndef LUMINA_H_
#define LUMINA_H_

#include <string>
#include <vector>

#include "adv.h"

namespace lumina
{
	bool LoadScenario(const std::wstring& wstrFilePath, std::vector<adv::TextDatum>& textData, std::vector<adv::ImageFileDatum>& imageFileData);
}
#endif // !LUMINA_H_
