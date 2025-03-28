#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include <Common/EngineTypes.h>
#include <Common/EngineEnums.h>

namespace Util {
    // FILE
    std::string ReadTextFromFile(std::string path);
    std::string GetFullPath(const std::filesystem::directory_entry& entry);
    std::string GetFileName(const std::filesystem::directory_entry& entry);
    std::string GetFileNameWithoutExtension(const std::filesystem::directory_entry& entry);
    std::string GetFileExtension(const std::filesystem::directory_entry& entry);
    std::vector<FileInfo> IterateDirectory(const std::string& directory, std::vector<std::string> extensions = {});

    // TEXT
    bool StrCmp(const char* queryA, const char* queryB);
}
