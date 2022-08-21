#include "dir_tool.h"

PGINF_NAMESPACE_BEGIN()

std::string 
dir_tool::CurrentFolder() {
    std::string current_program{};
    try {
#if defined(_MSC_VER)
        char * current_program_c = nullptr;
        errno_t error_code = _get_pgmptr(&current_program_c);
        if (error_code != 0) {
            char error_str[80];
            strerror_s(error_str, 80, error_code);
            LOGGER("[Error] Can't get the path of current program.\n   Detail: %s ", error_str);
            return std::string();
        }
        current_program = current_program_c;
#else
        current_program = _pgmptr;
#endif
    } catch (std::exception & e) {
        LOGGER("[Error] Can't get the path of current program.\n   Detail: %s ", e.what());
        return std::string();
    }

    current_program = TurnRightSlashes2LeftInPath(current_program);

    size_t last_dash = current_program.find_last_of('/');
    std::string folder = current_program.substr(0, last_dash);
    return folder;
}

void 
dir_tool::ListFiles(std::list<std::string>& list, 
    const std::string& folder_path, const std::string& extension, bool recursive) { 
    // Empty string
    if (folder_path.empty())
        return;

    // Open the file handle
    _finddata_t file_info;
    std::string current_path = folder_path + "/*.*";
    intptr_t    handle       = _findfirst(current_path.c_str(), &file_info);
    if (handle == -1) {
        LOGGER("[Error] Cannot match the path \"%s\".", folder_path.c_str());
		return;
	}

    // Search (recursive or not)
    do
	{
		// dir 
		if (file_info.attrib == _A_SUBDIR) {
			if (strcmp(file_info.name, "..") != 0 && strcmp(file_info.name, ".") != 0 && recursive)   
				ListFiles(list, folder_path + "/" + file_info.name, extension, recursive);
		} else {
            std::string file_name = file_info.name;
            auto last_dot = file_name.find_last_of('.');
#ifdef PGINF_DEBUG
            auto tmp = file_name.substr(last_dot + 1, extension.size());
            LOGGER(" -- [dir_tool::ListFiles] File : %s ; File's Ext: \"%s\".", file_name.c_str(), tmp.c_str());
#endif // PGINF_DEBUG
            if (file_name.substr(last_dot + 1, extension.size()) == extension)
                list.emplace_back(folder_path + "/" + file_info.name);
		}
	} while (!_findnext(handle, &file_info));
   
    // Close the file handle
	_findclose(handle);
}


std::string 
dir_tool::GetFolderPathFromFile(const std::string& file_path, const std::string& extension) {
    size_t last_dot = file_path.find_last_of('.');
    if (last_dot + 1 + extension.size() > file_path.size())
        return std::string();
    std::string real_extension = file_path.substr(last_dot + 1, extension.size());
    if (real_extension != extension)
        return std::string();
    // Get path of folder.
    size_t last_dash = file_path.find_last_of('/');
    std::string folder = file_path.substr(0, last_dash);
    return folder;
}

std::string 
dir_tool::TurnRightSlashes2LeftInPath(const std::string& path) {
    // Get path of folder.
    std::string ans = path;
    for (int i = 0; i < path.size(); i++) {
        if (ans[i] == '\\')
            ans[i] = '/';
    }
    return ans;
}

PGINF_NAMESPACE_END()