#include "Exp_vvod.h"
#include "Input.h"
#include <iostream>
#include <limits>
using namespace std;

std::string inputString()
{
    while (true)
    {
        try {
            std::string line;
            std::getline(std::cin, line);

            if (line.empty()) {
                throw Exp_vvod(1, "Пустая строка. Пожалуйста, введите строку.");
            }

            // Проверка на допустимые символы: латиница, кириллица (UTF-8), цифры, дефис, пробел
            // Для UTF-8 кириллица занимает 2 байта, поэтому проверяем более гибко
            for (size_t i = 0; i < line.length(); ++i) {
                unsigned char c = static_cast<unsigned char>(line[i]);
                bool isLatin = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
                bool isDigit = (c >= '0' && c <= '9');
                bool isAllowed = (c == '-' || c == ' ');
                // Проверка на начало кириллического символа в UTF-8 (0xD0 или 0xD1)
                bool isCyrillicStart = (c == 0xD0 || c == 0xD1);
                
                if (!isLatin && !isDigit && !isAllowed && !isCyrillicStart) {
                    // Если это не начало кириллицы, проверяем, не является ли это продолжением UTF-8 символа
                    if (i > 0 && (static_cast<unsigned char>(line[i-1]) == 0xD0 || static_cast<unsigned char>(line[i-1]) == 0xD1)) {
                        // Это продолжение кириллического символа, пропускаем
                        continue;
                    }
                    throw Exp_vvod(2, "Некорректные символы в строке. Разрешены только буквы (латиница/кириллица), цифры, дефис и пробел.");
                }
            }

            return line;

        }
        catch (const Exp_vvod& e) {
            std::cout << e.what() << "\nПовторите ввод: ";
        }
    }
}