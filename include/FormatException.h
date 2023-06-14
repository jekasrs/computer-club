#ifndef MY_SYSTEM_FORMATEXCEPTION_H
#define MY_SYSTEM_FORMATEXCEPTION_H

#include <exception>
#include <iostream>
#include <string>

#include "FormatExceptionDefine.h"

class FormatException: public std::exception {

public:
    explicit FormatException(int lineNumber, const std::string &line, const std::string &info) noexcept;
    explicit FormatException(const std::string &info) noexcept;

    const char *what() const noexcept override;

    int getLineNumber() const noexcept;
    const std::string& getLine() const noexcept;
    const std::string& getInfo() const noexcept;

private:
    int lineNumber;
    std::string line;
    std::string info;
    std::string msg;
};

#endif
