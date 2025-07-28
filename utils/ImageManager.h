#pragma once

#include "FileManager.h"
#include <vector>


// Следует поделить класс на более простой, для соблюдения принципа S (OLID), но задачи не было такой =)
// В общем и целом, можно оптимизировать по всем направлениям SOLID

// Общий класс для работы с данными изображения формата BMP: анализ совместимости, и рисование в консоли + геттеры на будущее
class BMPImageManager {
public:
    enum class Type {
        _16bit, // Речь про размер полей заголовков в BMP разных форматах
        _32bit
    };

    enum class BMP_COMMON {
        OFFSET_TO_PIXELS = 10, // 0А -  адрес где хранятся 4 байта - смещение до пикселей
        INFO_HEADER_SIZE = 14, // Сразу после инфо о положении пикселей идет размер структуры, зависящий от версии формата
    };

    enum class BMP_16 {
        WIDTH_OFFSET = 18,
        HEIGHT_OFFSET = 20,
        BIT_COUNT_OFFSET = 24,
    };

    enum class BMP_32 {
        WIDTH_OFFSET = 18,
        HEIGHT_OFFSET = 22,
        BIT_COUNT_OFFSET = 28,
    };

private:
    std::vector<uint8_t> data; // сырой BMP с пикселями и служебными данными

    uint32_t width;    
    uint32_t height;    
    uint32_t bitsCount;
    uint32_t coordPosition;
    uint32_t rowPadding;

public:
    explicit BMPImageManager(std::vector<uint8_t>&&);
    const std::vector<uint8_t>& getPixelsVector() const;    
    void drawToConsole() const;
    void drawLine(int x1, int y1, int x2, int y2);  
    uint32_t getWidth() const;
    uint32_t getHeight() const;    
    uint32_t getCoordPosition() const;
    uint32_t getBitsCount() const;    

private:
    Type validateBMP() const;

    uint32_t getDWord(uint32_t) const;
    uint16_t getWord(uint32_t) const;    uint32_t calculateRowPadding() const;
    
    uint32_t extractCoordPosition() const;

};