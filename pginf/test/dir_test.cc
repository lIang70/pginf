#include <gtest/gtest.h>

#include <list>
#include <string>
#include <list>

#define LOGGER(format, ...) \
    do { \
        fprintf(stdout, format##"\n", __VA_ARGS__); \
        fflush(stdout); \
    } while (0)

void ListFiles(std::list<std::string>& list, 
    const std::string& folder_path, const std::string& extension, bool recursive) { 
    // Empty string
    if (folder_path.empty())
        return;

    // Open the file handle
    _finddata_t file_info;
    std::string current_path = folder_path + "/*.*";
    intptr_t    handle       = _findfirst(current_path.c_str(), &file_info);
    if (handle == -1) {
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

TEST(DirToolTest, ListFilesTest) {
    std::list<std::string> rest;

    std::string folder = "C:\\Windows\\System32";

    ListFiles(rest, folder, "dll", true);

    for (auto & _res : rest)
    {
        size_t len = _res.size();
        size_t pod = _res.find_last_of('.') + 1;
        EXPECT_NE(pod, len);
        std::string ext;
        for (size_t i = pod; i < len; i++) 
            ext.push_back(_res[i]);
        EXPECT_EQ(ext, "dll");
    }
}

int main(int argc, char **argv) {

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}