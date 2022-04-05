#include "RequestValidator.hpp"

#include <iostream>
#include <assert.h>
#include <sstream>

bool RequestValidator::validHeader(std::string& line) {
        size_t pos = line.find(":");
        if (pos == std::string::npos || pos == 0)
                return false;
        if (line.substr(line.size() - 2) != "\r\n")
                return false;
        return true;
}
bool RequestValidator::validHeaders(std::string& headers) {
        std::istringstream iss(headers);
        std::string _line;
        bool first = true;
        bool host_exists = false;
        while (std::getline(iss, _line))
        {
                _line.append("\n");
                if (first)
                {
                        if (!RequestValidator::validRequest(_line))
                                return false;
                        first = false;
                }
                else
                {
                        if (!RequestValidator::validHeader(_line))
                                return false;
                        size_t pos = _line.find(":");
                        if (_line.substr(0, pos) == "Host")
                                host_exists = true;
                }
        }
        if (host_exists)
                return true;
        return false;
}

bool RequestValidator::validRequest(std::string& line) {
    size_t pos = line.find(" ");
    if (pos == std::string::npos || pos == 0)
            return false;
    std::string tmp = line.substr(0, pos);
    if (tmp != "GET" && tmp != "POST" && tmp != "DELETE")
            return false;
    tmp = line.substr(pos + 1);
    if (tmp[0] != '/')
            return false;
    pos = tmp.find(" ");
    if (pos == std::string::npos)
            return false;
    tmp = tmp.substr(pos + 1);
    if (tmp != "HTTP/1.1\r\n")
            throw std::runtime_error("proto invalid");
    return true;
}