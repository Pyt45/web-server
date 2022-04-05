#ifndef REGEX_HPP
# define REGEX_HPP

# include "Match.hpp"

class Regex
{
	public:
		Regex(std::string r);
		void regex_search(const std::string& data, Match& match, const Regex& re);
	private:
		std::string _re;
};

#endif