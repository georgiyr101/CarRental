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
                throw Exp_vvod(1, "Empty string. Please enter a string.");
            }

            for (size_t i = 0; i < line.length(); ++i) {
                unsigned char c = static_cast<unsigned char>(line[i]);
                bool isLatin = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
                bool isDigit = (c >= '0' && c <= '9');
                bool isAllowed = (c == '-' || c == ' ' || c == '@' || c == '.');
                bool isCyrillicStart = (c == 0xD0 || c == 0xD1);
                
                if (!isLatin && !isDigit && !isAllowed && !isCyrillicStart) {
                    if (i > 0 && (static_cast<unsigned char>(line[i-1]) == 0xD0 || 
                        static_cast<unsigned char>(line[i-1]) == 0xD1)) {
                        continue;
                    }
                    throw Exp_vvod(2, "Invalid characters in string. Only letters (Latin/Cyrillic), digits, hyphen and space are allowed.");
                }
            }
            return line;
        }
        catch (const Exp_vvod& e) {
            std::cout << e.what() << "\nTry again: ";
        }
    }
}