#pragma once
#include "Exp.h"

class Exp_contr : public Exp {
public:
    Exp_contr(int errorCode, const std::string& errorMessage) : Exp(errorCode, errorMessage) {}
    ~Exp_contr() = default;
};