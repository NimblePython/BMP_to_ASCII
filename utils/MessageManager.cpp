#include "MessageManager.h"
#include <iostream>

MessageManager::MessageManager(Language lang) :  currentLanguage(lang) {};

void MessageManager::print(std::ostream& out, MessageKey key, const std::string& addInfo, bool end) {
    std::string message = messages[key][currentLanguage] + (addInfo != "" ? ": " + addInfo : "");
    std::cout << message << (end ? '\n' : ' ');
}

void MessageManager::greeting() {
    print(std::cout, MessageKey::APP_STARTED);
}

void MessageManager::inputFile() {
    print(std::cout, MessageKey::ASK_INPUT_FILE, "", false);
}

void MessageManager::chooseFileToSave() {
    print(std::cout, MessageKey::ASK_OUTPUT_FILE, "", false);
}

MessageManager& MessageManager::getInstance() {
    static MessageManager instance;
    return instance;
}

void MessageManager::setLanguage(Language lang) {
    currentLanguage = lang;
}