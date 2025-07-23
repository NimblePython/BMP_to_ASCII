#pragma once

#include <string>
#include <fstream>

class FileManager {
public:
    FileManager(std::string&);
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;
    ~FileManager();
    const std::vector<uint8_t>& getData() const;
    std::vector<uint8_t> extractData();

    // static потому что перемещаю вектор в ImageManager (может и не надо, но кажется, что логично и оптимально)
    static void saveBMP(const std::string&, const std::vector<uint8_t>&);

private:
    std::ifstream BMPFile;
    std::string fileName;
    std::vector<uint8_t> fileData;

private:
    void closeFile();
    void getDataFromFile();
};
