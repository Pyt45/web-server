#include "Regex.hpp"

Regex::Regex(std::string r) {
	_re = r;
}
void	Regex::regex_search(const std::string& data, Match& match, const Regex& re) {
	std::string _suffix = "";
	std::string _prefix = "";
	std::string _self = "";
	if (data.find(re._re) != std::string::npos)
	{
		_self = data.substr(0, re._re.length());
		_prefix = data.substr(0, data.find(re._re));
		_suffix = data.substr(data.find(re._re) + re._re.length(), data.length());
	}
	match._setSuffix(_suffix);
	match._setPrefix(_prefix);
	match._setSelf(_self);
}