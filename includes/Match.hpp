#ifndef MATCH_HPP
# define MATCH_HPP

# include <iostream>
# include <string>

class Match {
	public:
		bool empty();
		std::string suffix();
		std::string prefix();
		std::string self();
		void _setSuffix(std::string s);
		void _setPrefix(std::string s);
		void _setSelf(std::string s);
	private:
		std::string _prefix;
		std::string _suffix;
		std::string _self;
};

#endif