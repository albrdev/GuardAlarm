#ifndef _GENERIC_HPP_
#define _GENERIC_HPP_

#include <cstring>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <limits>

bool readString(std::string& result);
bool readChar(char& result);

int strcmpic(const std::string& a, const std::string& b);

std::string timetostr(const std::string& format, const time_t time);
std::string timetostr(const std::string& format);
std::time_t strtotime(const char *const str);

template<typename T> std::string ToString(const T& value);
template<typename T> T FromString(const std::string& str);
template<typename T> bool FromString(const std::string& str, T& result);

#endif // _GENERIC_HPP_
