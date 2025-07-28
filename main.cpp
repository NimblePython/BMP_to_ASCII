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
        MessageManager::getInstance().greeting(); // Представляемся
        MessageManager::getInstance().inputFile(); // Просим указать файл для отображения
        std::string fileName;
        std::cin >> fileName; 
        FileManager file(fileName); // Загружаем и получаем данные из файла, закрываем файл
        BMPImageManager image(file.extractData()); //  Перемещаем данные в сущность "менеджер для работы с картинкой"
        
        image.drawToConsole(); // Рисуем картинку исходную
        
        image.drawLine(0,0, image.getWidth(), image.getHeight()); // Добавим линию 1
        image.drawLine(image.getWidth(), 0, 0, image.getHeight()); // Добавим линию 2
        
        image.drawToConsole(); // Рисуем картинку измененную
        
        MessageManager::getInstance().chooseFileToSave(); // Предлагаем ввести имя файла для сохранения
        std::cin >> fileName; 
        file.saveBMP(fileName, image.getPixelsVector());  // Сохраняем
    } catch(const std::runtime_error& e) {
        std::cout << "Oooops... Runtime error. Program will terminate. Sorry..." << std::endl;
        return 1;
    }

    return 0;
}

/***** Для компиляции и запуска *****

g++ -o prog main.cpp utils/FileManager.cpp utils/ImageManager.cpp utils/MessageManager.cpp

************************************/
