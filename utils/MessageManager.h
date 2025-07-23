#pragma once

#include <unordered_map>
#include <iostream>

class MessageManager {
public:
    enum class Language {
        RUSSIAN,
        ENGLISH
    };

    enum class MessageKey {
        APP_STARTED,
        ASK_INPUT_FILE,
        ASK_OUTPUT_FILE,
        FILE_OPENING,
        FILE_OPENED,
        FILE_NOT_FOUND,
        FILE_CLOSED,
        FILE_SAVED,
        INTERNAL_UNEXPECTED        
    };

    static MessageManager& getInstance();
    void setLanguage(Language);
    void greeting();
    void inputFile();   
    void chooseFileToSave();     
    void print(std::ostream&, MessageKey, const std::string& = "", bool =true);


private:
    Language currentLanguage;
    MessageManager(Language = Language::RUSSIAN);

    // Сообщения на разных языках (в перспективе можно расширить и аппелировать сообщениями из файла)
    std::unordered_map<MessageKey, std::unordered_map<Language, std::string>> messages = {
        {MessageKey::APP_STARTED, {
            {Language::RUSSIAN, "Консольное приложение, для работы с BMP файлами. "},
            {Language::ENGLISH, "The console applicatin to work with BMP files."}
        }},
        {MessageKey::FILE_OPENING, {
            {Language::RUSSIAN, "Файл открывается... "},
            {Language::ENGLISH, "File is opening... "}
        }},   
        {MessageKey::FILE_OPENED, {
            {Language::RUSSIAN, "Файл считан успешно. "},
            {Language::ENGLISH, "File opened currectly. "}
        }},         
        {MessageKey::FILE_NOT_FOUND, {
            {Language::RUSSIAN, "Ошибка чтения файла. "},
            {Language::ENGLISH, "I/O file error. "}
        }},
        {MessageKey::FILE_CLOSED, {
            {Language::RUSSIAN, "Файл закрыт. "},
            {Language::ENGLISH, "File closed. "}
        }},     
        {MessageKey::INTERNAL_UNEXPECTED, {
            {Language::RUSSIAN, "Непредвиденная ошибка. "},
            {Language::ENGLISH, "Unexpected error. "}
        }},   
        {MessageKey::ASK_INPUT_FILE, {
            {Language::RUSSIAN, "Введите имя BMP файла для чтения -> "},
            {Language::ENGLISH, "Input BMP filename to read -> "}
        }},
        {MessageKey::ASK_OUTPUT_FILE, {
            {Language::RUSSIAN, "Введите имя BMP файла для сохранения -> "},
            {Language::ENGLISH, "Input BMP filename to save -> "}
        }},
        {MessageKey::FILE_SAVED, {
            {Language::RUSSIAN, "Файл успешно сохранен. "},
            {Language::ENGLISH, "File saved. "}
        }},                                           
    };
};