#ifndef UNCHUNK_CONTENT_HPP
# define UNCHUNK_CONTENT_HPP
# include <iostream>
#include <sstream>

class UnchunkContent {
    public:
        static bool is_valid_hex(std::string  const& x);
        static std::string handle(std::string const& content);
        static std::string loop_over_numbers(std::string const& tmp);
        static std::string get_chunk(int n, std::string iss);
};
#endif