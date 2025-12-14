#pragma once
#include "Exp.h"

class Exp_file : public Exp {
public:
    Exp_file(int errorCode, const std::string& errorMessage) : Exp(errorCode, errorMessage) {}
    ~Exp_file() = default;
};
