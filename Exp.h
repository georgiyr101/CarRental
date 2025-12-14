#pragma once

#include <iostream>
#include <exception>
#include <cstring>

class Exp : public std::exception
{
protected:
	int code;
	std::string message;

public:
	Exp(int errorCode, const std::string& errorMessage) : code(errorCode), message(errorMessage) {}
	virtual ~Exp() = default;

	virtual int getCode() const noexcept;
	const char* what() const noexcept override;
};

