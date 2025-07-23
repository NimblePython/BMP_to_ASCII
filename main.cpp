#include "utils/FileManager.h"
#include "utils/ImageManager.h"
#include "utils/MessageManager.h"

#ifdef _WIN32
    // Windows
    #include <windows.h>
#else
    // macOS и прочие unix like системы
    #include <unistd.h>
#endif

int main() {

#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif    

    try {
        MessageManager::getInstance().greeting();
        MessageManager::getInstance().inputFile();
        std::string fileName;
        std::cin >> fileName;
        FileManager file(fileName);
        BMPImageManager image(file.extractData());
        // std::cout << image.getWidth() << std::endl;
        // std::cout << image.getHeight() << std::endl;        
        // std::cout << image.getCoordPosition() << std::endl;
        // std::cout << image.getBitsCount() << std::endl; 
        // std::cout << image.calculateRowPadding() << std::endl;         
        image.drawToConsole();    
        MessageManager::getInstance().chooseFileToSave();
        std::cin >> fileName; 
        file.saveBMP(fileName, image.getPixelsVector());  
    } catch(const std::runtime_error& e) {
        std::cout << "Oooops... Runtime error. Program will terminate. Sorry..." << std::endl;
        return 1;
    }



    return 0;
}

/***** Для компиляции и запуска *****

g++ -o prog main.cpp utils/FileManager.cpp utils/ImageManager.cpp utils/MessageManager.cpp

************************************/
