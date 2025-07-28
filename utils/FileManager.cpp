#include "FileManager.h"
#include "MessageManager.h"
#include <string>
#include <fstream>
#include <iostream>

FileManager::FileManager(std::string& fileName) : fileName(fileName) {
    BMPFile.open( fileName, std::ios::binary | std::ios::ate );
    if ( (BMPFile.rdstate() & std::ifstream::failbit) != 0) {
        MessageManager::getInstance().print(std::cerr, MessageManager::MessageKey::FILE_NOT_FOUND, fileName);
        BMPFile.close();
        throw std::runtime_error("FileManager::FileManager(std::string fileName)");
    }
    MessageManager::getInstance().print(std::cout, MessageManager::MessageKey::FILE_OPENED);
    getDataFromFile();
    closeFile();
}

FileManager::~FileManager() {
    closeFile(); //  Лишнее, так как при выходе из области видимости файловый объект закроется сам, но пусть будет =)
}

void FileManager::getDataFromFile() {
    if (!BMPFile.is_open()) {
        MessageManager::getInstance().print( std::cerr, MessageManager::MessageKey::INTERNAL_UNEXPECTED, "getVectorFromFile()" );
        throw std::runtime_error("BMPFile is closed");
    }
    
    std::streamsize size = BMPFile.tellg(); // Файл уже открыт конструктором с кареткой в конце
    fileData.resize(static_cast<size_t>(size)); // Явно приводим на всякий пожарный

    BMPFile.seekg( 0, std::ios::beg ); // Возвращаем каретку в начало
    if (!BMPFile.read(reinterpret_cast<char*>(fileData.data()), size)) { // Читаем из файла в буфер - в вектор, указав начало вектора с явным приведением в char*
        MessageManager::getInstance().print(std::cerr, MessageManager::MessageKey::INTERNAL_UNEXPECTED, "getVectorFromFile()" );
        throw std::runtime_error("Can't get data from " + fileName);
    }
}

const std::vector<uint8_t>& FileManager::getData() const {
    return fileData;
}

std::vector<uint8_t> FileManager::extractData() {
    return std::move(fileData);
}

void FileManager::closeFile() {
    if (BMPFile.is_open()) {
        BMPFile.close();
        MessageManager::getInstance().print(std::cout, MessageManager::MessageKey::FILE_CLOSED);
    }
}

void FileManager::saveBMP(const std::string& fileName, const std::vector<uint8_t>& data) {
    std::ofstream fileBMP(fileName, std::ios::binary);
    if (!fileBMP) {
        throw std::runtime_error("Can't create file: " + fileName);
    }
    fileBMP.write(reinterpret_cast<const char*>(data.data()), data.size());
    fileBMP.close();
    MessageManager::getInstance().print(std::cout, MessageManager::MessageKey::FILE_SAVED);
}
