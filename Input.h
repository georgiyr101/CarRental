#pragma once
#include <iostream>
#include <sstream>
#include <limits>
#include <string>
#include "Exp_vvod.h"

template <typename T>
T inputNumber(std::istream& stream, T lowerBound, T upperBound)
{
    T value;
    bool isValid = false;

    do {
        std::string line;
        
        // Очищаем любые флаги ошибок перед чтением
        if (stream.fail() || stream.bad()) {
            stream.clear();
        }
        
        // Просто читаем строку - getline будет ждать ввода от пользователя для std::cin
        // Если перед этим был использован оператор >>, символ новой строки будет прочитан как пустая строка
        if (!std::getline(stream, line)) {
            // Если getline вернул false, проверяем причину
            if (stream.eof()) {
                // Для файлов это нормально, но для интерактивного ввода не должно происходить
                stream.clear();
                std::cout << "Ошибка ввода данных. Повторите попытку.\n";
                continue;
            }
            // Другая ошибка - очищаем и продолжаем
            stream.clear();
            std::cout << "Ошибка ввода данных. Повторите попытку.\n";
            continue;
        }

        try {
            // Удаляем пробельные символы в начале и конце строки
            if (!line.empty()) {
                size_t first = line.find_first_not_of(" \t\n\r");
                if (first != std::string::npos) {
                    line.erase(0, first);
                }
                size_t last = line.find_last_not_of(" \t\n\r");
                if (last != std::string::npos) {
                    line.erase(last + 1);
                } else {
                    line.clear();
                }
            }
            
            if (line.empty()) {
                // Для пустой строки просим пользователя ввести значение
                std::cout << "Пустая строка. Введите число: ";
                continue; // Продолжаем цикл для повторного ввода
            }

            std::istringstream converter(line);
            T tempNumber;

            if (!(converter >> tempNumber)) {
                throw Exp_vvod(2, "Некорректный ввод. Ожидается число, получено: " + line);
            }

            char extraChar;
            if (converter >> extraChar) {
                throw Exp_vvod(3, "Введено число с лишними символами: " + line);
            }

            if (tempNumber < lowerBound || tempNumber > upperBound) {
                std::stringstream msg;
                msg << "Число не соответствует диапазону. Ожидается от "
                    << lowerBound << " до " << upperBound << ", получено: " << tempNumber;
                throw Exp_vvod(4, msg.str());
            }

            value = tempNumber;
            isValid = true;

        }
        catch (const Exp_vvod& e) {
            std::cout << e.what() << "\nПовторите ввод: ";
            // Очищаем буфер потока перед следующей попыткой
            if (stream.fail()) {
                stream.clear();
            }
        }

    } while (!isValid);

    return value;
}


std::string inputString();


