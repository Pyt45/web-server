#include "UnchunkContent.hpp"
#include "HexConvertor.hpp"
#include <sstream>
#include <string>
#include <exception>

bool UnchunkContent::is_valid_hex(std::string  const& x) {
    for (size_t i = 0; i < x.size(); i++)
    {
        if (!((x[i] >= '0' && x[i] <= '9') || (x[i] >= 'A' && x[i] <= 'F')))
            return false;
    }
    return true;
}

std::string UnchunkContent::loop_over_numbers(std::string const& tmp)
{
    std::string _line;
    HexConvertor c;
    std::string content = "";

    size_t i = 0;
    bool is_number = true;
    int nbr;

    while (i < tmp.size())
    {
        if (is_number)
        {
        
            size_t k = tmp.substr(i, tmp.size()).find("\r\n");
            if (UnchunkContent::is_valid_hex(tmp.substr(i, k - 1)) == false)
                throw std::runtime_error("Not a good number");
            nbr = c.to_decimal(tmp.substr(i, k));
            if (nbr == 0)
                break;
            i += k + 2;
            is_number = false;
        }
        else
        {
            content += get_chunk(nbr, tmp.substr(i, nbr));
            i += nbr + 2;
            is_number = true;
        }        
    }
    return content;
}

std::string UnchunkContent::get_chunk(int n, std::string str)
{
    int i = 0;
    std::string content = "";

    while (i < n)
    {
        content += str[i];
        i++;
    }
    return (content);   
}
