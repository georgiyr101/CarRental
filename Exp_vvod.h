#pragma once
#include "Exp.h"

class Exp_vvod : public Exp
{
public:
	Exp_vvod(int errorCode, const std::string& errorMessage) : Exp(errorCode, errorMessage) {}
	~Exp_vvod() = default;
};
