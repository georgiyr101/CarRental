#include "Exp.h"

int Exp::getCode() const noexcept {
    return code;
}

const char* Exp::what() const noexcept {
    return message.c_str();
}