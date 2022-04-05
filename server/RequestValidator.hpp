#include <iostream>
#include <sstream>

class RequestValidator {
        public:
                static bool validHeader(std::string& _line);
                static bool validHeaders(std::string& headers);
                static bool validRequest(std::string& _line);
        private:
};