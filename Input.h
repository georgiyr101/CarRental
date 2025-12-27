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
        if (stream.fail() || stream.bad()) {stream.clear();}
        if (!std::getline(stream, line)) {
            if (stream.eof()) {
                stream.clear();
                std::cout << "Input error. Please try again.\n";
                continue;
            }
            stream.clear();
            std::cout << "Input error. Please try again.\n";
            continue;
        }

        try {
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
                std::cout << "Empty string. Enter a number: ";
                continue; // Continue loop for retry
            }

            std::istringstream converter(line);
            T tempNumber;

            if (!(converter >> tempNumber)) {
                throw Exp_vvod(2, "Invalid input. Expected a number, got: " + line);
            }

            char extraChar;
            if (converter >> extraChar) {
                throw Exp_vvod(3, "Number entered with extra characters: " + line);
            }

            if (tempNumber < lowerBound || tempNumber > upperBound) {
                std::stringstream msg;
                msg << "Number out of range. Expected from "
                    << lowerBound << " to " << upperBound << ", got: " << tempNumber;
                throw Exp_vvod(4, msg.str());
            }

            value = tempNumber;
            isValid = true;

        }
        catch (const Exp_vvod& e) {
            std::cout << e.what() << "\nTry again: ";
            // Clear stream buffer before next attempt
            if (stream.fail()) {
                stream.clear();
            }
        }

    } while (!isValid);

    return value;
}

std::string inputString();


