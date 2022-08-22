#pragma once
#ifndef _PGINF_DIR_H_
#define _PGINF_DIR_H_

#include "internal_global.h"

PGINF_NAMESPACE_BEGIN()
NAMESPACE_BEGIN(dir_tool)

/****************************************
 * @brief Gets the folder where the current program resides.
 * 
 * @return The path of folder.
 ****************************************/
extern std::string CurrentFolder();

/****************************************
 * @brief Get the list of files found in the folder that contain the extension suffix.
 * 
 * @param list The list of files found in the folder that contain the extension suffix.
 * @param folder_path The path of folder
 * @param extension The suffix of file.
 * @param recursive Whether to search folders recursively.
 ****************************************/
extern void ListFiles(std::list<std::string>& list, const std::string& folder_path, const std::string& extension, bool recursive = true);

/****************************************
 * @brief Get the path of folder that contains file.
 * 
 * @param file_path The path of file.
 * @param extension The suffix of file.
 * @return The path of the folder that contains file.
 ****************************************/
extern std::string GetFolderPathFromFile(const std::string& file_path, const std::string& extension);

/****************************************
 * @brief Change all right slashes to left slashes.
 * 
 * @param path The file path.
 * @return The transformed path.
 ****************************************/
extern std::string TurnRightSlashes2LeftInPath(const std::string& path);

NAMESPACE_END()
PGINF_NAMESPACE_END()

#endif // !_PGINF_DIR_H_