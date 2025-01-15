cmake_minimum_required (VERSION 3.8)

set(NLOHMANN_JSON_VERSION "3.11.3")

set(DIR_NLOHMANN_JSON ${CMAKE_CURRENT_LIST_DIR}/nlohmann)

set(DIR_TEMP ${CMAKE_CURRENT_BINARY_DIR}/temp)

function(download_and_extract HOST_URL ARCHIVE_NAME)
	if (NOT EXISTS "${DIR_TEMP}/${ARCHIVE_NAME}.zip")
		file(DOWNLOAD "${HOST_URL}" "${DIR_TEMP}/${ARCHIVE_NAME}.zip" SHOW_PROGRESS)
	endif()

	if (NOT EXISTS "${DIR_TEMP}/${ARCHIVE_NAME}")
		file(ARCHIVE_EXTRACT INPUT "${DIR_TEMP}/${ARCHIVE_NAME}.zip" DESTINATION "${DIR_TEMP}")
	endif()
endfunction()

# Donwload JSON for Modern C++
if (NOT EXISTS "${DIR_NLOHMANN_JSON}")
	set(SINGLE_HEADER_FILENAME "json.hpp")
	set(NLOHMANN_JSON_URL "https://github.com/nlohmann/json/releases/download/v${NLOHMANN_JSON_VERSION}/${SINGLE_HEADER_FILENAME}")
	file(DOWNLOAD "${NLOHMANN_JSON_URL}" "${DIR_NLOHMANN_JSON}/${SINGLE_HEADER_FILENAME}")
endif()

