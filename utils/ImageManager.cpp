#include "ImageManager.h"
#include "FileManager.h"

#include <iostream>

#ifdef _WIN32
    // Windows
    #include <windows.h>
#else
    // macOS и прочие unix like системы
    #include <unistd.h>
#endif

BMPImageManager::BMPImageManager(std::vector<uint8_t>&& data) : data(std::move(data)) {
    Type formatBMP = validateBMP();

    // Присваиваем свойствам значения исходя из формата файла
    // Немножечко нарушен принцип открытости/закрытости, но в задании не требовалось -)
    if ( formatBMP == Type::_16bit ) {
        width = getWord(static_cast<size_t>(BMP_16::WIDTH_OFFSET));
        height = getWord(static_cast<size_t>(BMP_16::HEIGHT_OFFSET));
        bitsCount = getWord(static_cast<size_t>(BMP_16::BIT_COUNT_OFFSET));
    } else if ( formatBMP == Type::_32bit ) {
        width = getDWord(static_cast<size_t>(BMP_32::WIDTH_OFFSET)); // 4 байта
        height = getDWord(static_cast<size_t>(BMP_32::HEIGHT_OFFSET)); // 4 байта
        bitsCount = getWord(static_cast<size_t>(BMP_32::BIT_COUNT_OFFSET)); // (!) 2 байта
    }
    
    if (bitsCount != 24 && bitsCount != 32) {
        throw std::out_of_range("This program support only 24 or 32 color bit");        
    } 

    coordPosition = extractCoordPosition();
    rowPadding = calculateRowPadding();
}

uint32_t BMPImageManager::getDWord(uint32_t offset) const {
    if (data.size() < offset+4) {
        throw std::out_of_range("Not enough data");
    }
    uint32_t value;
    std::memcpy(&value, &data[offset], sizeof(uint32_t)); //Работает только с little-endian
    return value;    
}

uint16_t BMPImageManager::getWord(uint32_t offset) const {
    if (data.size() < offset+2) {
        throw std::out_of_range("Not enough data");
    }
    uint16_t value;
    std::memcpy(&value, &data[offset], sizeof(uint16_t)); //Работает только с little-endian
    return value;    
}

uint32_t BMPImageManager::extractCoordPosition() const {
    return getDWord(static_cast<size_t>(BMP_COMMON::OFFSET_TO_PIXELS));
}

uint32_t BMPImageManager::getWidth() const {
    return width;
}

uint32_t BMPImageManager::getHeight() const {
    return height;
}

uint32_t BMPImageManager::getCoordPosition() const {
    return coordPosition;
}

uint32_t BMPImageManager::getBitsCount() const {
    return bitsCount;
}

const std::vector<uint8_t>& BMPImageManager::getPixelsVector() const {
    return data;
}

BMPImageManager::Type BMPImageManager::validateBMP() const{
    // Проверим заголовок и размер, которые обязаны, по сути, быть в любом BMP файле
    if (data.size() < 18) {
        throw std::runtime_error("Size of file is too small to be BMP");
    } else if (data[0] != 'B' || data[1] != 'M') {
        throw std::runtime_error("Not BMP file");
    }
    
    size_t headerSize = getDWord(static_cast<size_t>(BMP_COMMON::INFO_HEADER_SIZE));

    if ( headerSize == 12 ) {
        return Type::_16bit;
    } else if ( headerSize == 40 || headerSize == 108 || headerSize == 124 ) {
        return Type::_32bit;
    } else {
        throw std::runtime_error("Only Windows BMP format supported (header size must be 12, 40, 108 or 124 bites (see Wiki)...");
    }
}

void BMPImageManager::drawToConsole() const {
    uint32_t bytesPerPixel = this->bitsCount / 8;
    uint32_t actualBytesPerRow = this->width * bytesPerPixel;
    std::cout << std::endl;
        
    for (int y = this->height-1; y >= 0; --y) {
        int rowStart = this->coordPosition + y * (actualBytesPerRow + rowPadding);
        for (uint32_t x = 0; x < this->width; ++x) {
            uint32_t pos = rowStart + x * bytesPerPixel;            
            uint32_t b = this->data[pos];
            uint32_t g = this->data[pos + 1];
            uint32_t r = this->data[pos + 2];
            if ( b == g && g == r ) {
                if ( r == 255 ) { 
                    std::cout << "#";
                } else if ( r == 0 ) {
                    std::cout << " ";
                } else {
                    std::cout << "-";
                }
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

uint32_t BMPImageManager::calculateRowPadding() const {
    uint32_t bytesPerRow = width * (bitsCount / 8);
    return (4 - (bytesPerRow % 4)) % 4;
}

void BMPImageManager::drawLine(int x1, int y1, int x2, int y2) {
    // Пока не делаю проверок координат - 
    // По хоршему следует сделать контроллер, который будет это проверять, а потом уже вызывать функцию/сервис
    int bytesPerPixel = bitsCount / 8;
    int rowSize = width * bytesPerPixel;
    int actualRowSize = rowSize + rowPadding;

    // Алгоритм Брезенхэма
    int dx = std::abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -std::abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        // BMP хранит строки снизу вверх
        int bmpY = height - 1 - y1;
        int offset = coordPosition + bmpY * actualRowSize + x1 * bytesPerPixel;

        if (offset + 2 < data.size() && x1 >= 0 && x1 < width && y1 >= 0 && y1 < height) {
            // Белый цвет (BGR)
            data[offset + 0] = 255;
            data[offset + 1] = 255;
            data[offset + 2] = 255;
            if (bytesPerPixel == 4) data[offset + 3] = 0; // если есть альфа-канал
        }

        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}