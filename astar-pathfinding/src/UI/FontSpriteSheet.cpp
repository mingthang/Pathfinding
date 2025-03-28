#include "FontSpriteSheet.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <filesystem>

namespace FontSpriteSheetPacker {

    struct ImageData {
        int m_width;
        int m_height;
        int m_channelCount;
        void* m_data;
    };

    //std::string EscapeString(const std::string& str);
    std::string UnescapeString(const std::string& str);
    int FindInt(const std::string& json, const std::string& key);
    std::string FindString(const std::string& json, const std::string& key);
    std::vector<std::string> FindArray(const std::string& json, const std::string& key);

    FontSpriteSheet Import(const std::string& filePath) {
        FontSpriteSheet fontSpriteSheet;

        // Read the JSON file
        std::ifstream file(filePath);
        if (!file.is_open()) throw std::runtime_error("Failed to open file: " + filePath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string json = buffer.str();

        // Parse the JSON
        fontSpriteSheet.m_name = FindString(json, "name");
        fontSpriteSheet.m_characters = FindString(json, "characters");
        fontSpriteSheet.m_textureWidth = FindInt(json, "textureWidth");
        fontSpriteSheet.m_textureHeight = FindInt(json, "textureHeight");
        fontSpriteSheet.m_lineHeight = FindInt(json, "lineHeight");
        std::vector<std::string> charDataList = FindArray(json, "charDataList");
        for (const auto& charData : charDataList) {
            FontSpriteSheet::CharData data;
            data.width = FindInt(charData, "width");
            data.height = FindInt(charData, "height");
            data.offsetX = FindInt(charData, "offsetX");
            data.offsetY = FindInt(charData, "offsetY");
            fontSpriteSheet.m_charDataList.push_back(data);
        }

        return fontSpriteSheet;
    }

    std::string UnescapeString(const std::string& str) {
        std::string unescaped;
        size_t i = 0;

        while (i < str.size()) {
            // If see '\' symbol and at least 1 char following, then check the next char
            if (str[i] == '\\' && i + 1 < str.size()) {
                // If the next char is one of these cases
                switch (str[i + 1]) {
					case '\"': unescaped += '\"'; break; // Unescape double quote
					case '\\': unescaped += '\\'; break; // Unescape backslash
					case 'b': unescaped += '\b'; break; // Unescape backspace
					case 'f': unescaped += '\f'; break; // Unescape form feed
					case 'n': unescaped += '\n'; break; // Unescape newline
					case 'r': unescaped += '\r'; break; // Unescape carriage return
					case 't': unescaped += '\t'; break; // Unescape tab
					default: unescaped += str[i + 1]; break; // Handle unknown sequences as-is
                }
                i += 2; // Skip the backslash and the escaped character
            }
            else {
                unescaped += str[i];
                i++;
            }
        }
        return unescaped;
    }

    int FindInt(const std::string& json, const std::string& key) {
        size_t start = json.find("\"" + key + "\":") + key.length() + 3; // skip ": and space
        size_t end = json.find_first_of(",", start);
        if(end == std::string::npos)
            end = json.find_first_of("}", start);
        return std::stoi(json.substr(start, end - start));
    }

    std::string FindString(const std::string& json, const std::string& key) {
        size_t keyPos = json.find("\"" + key + "\":");
        if (keyPos == std::string::npos)
            throw std::runtime_error("FontSpriteSheet::FindString() - Key not found: " + key);

        // key.length() + 2 skip ":" and "space"
        size_t start = json.find_first_of("\"", keyPos + key.length() + 2) + 1;
        if (start == std::string::npos)
            throw std::runtime_error("FontSpriteSheet::FindString() - Invalid JSON format: missing value for key: " + key);

        size_t end = start;
        while (end < json.size()) {
            end = json.find("\"", end);
            if(end == std::string::npos)
				throw std::runtime_error("FontSpriteSheet::FindString() - Invalid JSON format: Unterminated value for key: " + key);
            // if found \" then skip, find another "
            if (end > 0 && json[end - 1] == '\\') {
                size_t backslashCount = 0;
                for (size_t i = end - 1; i >= start && json[i] == '\\'; --i)
                    ++backslashCount;
                if (backslashCount % 2 == 1) {
                    ++end;
                    continue;
                }
            }
            break;
        }

        return UnescapeString(json.substr(start, end - start));
    }

    std::vector<std::string> FindArray(const std::string& json, const std::string& key) {
        size_t start = json.find("\"" + key + "\":") + key.length() + 3; // skip ": and space
        size_t end = json.find("]", start);
        std::string arrayContent = json.substr(start, end - start);
        std::vector<std::string> items;
        size_t pos = 0;
        while ((pos = arrayContent.find("{", pos)) != std::string::npos) {
            size_t close = arrayContent.find("}", pos);
            items.push_back(arrayContent.substr(pos, close - pos - 1));
            pos = close + 1;
        }
        
        return items;
    }

}