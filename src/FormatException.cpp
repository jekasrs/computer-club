#include "../include/FormatException.h"

const char *FormatException::what() const noexcept {
    return msg.c_str();
}

const std::string& FormatException::getInfo() const noexcept {
    return info;
}

int FormatException::getLineNumber() const noexcept {
    return lineNumber;
}

const std::string& FormatException::getLine() const noexcept {
    return line;
}

FormatException::FormatException(const std::string &info) noexcept : info(info)  {}

FormatException::FormatException(int lineNumber, const std::string &line, const std::string &info) noexcept : lineNumber(
        lineNumber), line(line), info(info) {
    msg = "[Format string exception] line ";
    msg += std::to_string(lineNumber) + ": ";
    msg += "[ " + line + " ]" + "\n";
    msg += "Message: " + info + "" + "\n";
}