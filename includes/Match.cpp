
#include "Match.hpp"

bool	Match::empty() {
	return _prefix.length() == 0 && _suffix.length() == 0;
}
std::string		Match::suffix() {
	return _suffix;
}
std::string		Match::prefix() {
	return _prefix;
}
std::string		Match::self() {
	return _self;
}
void	Match::_setSuffix(std::string s) {
	_suffix = s;
}
void	Match::_setPrefix(std::string s) {
	_prefix = s;
}
void	Match::_setSelf(std::string s) {
	_self = s;
}