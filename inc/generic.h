#ifndef _GENERIC_H_
#define _GENERIC_H_

#include <ctime>
#include <iostream>
#include <string>
#include <limits>

bool readString(std::string& result);
bool readChar(char& result);

int strcmpic(const std::string& a, const std::string& b);

std::string timeString(time_t t);
std::time_t strtotime(const char *const str);

#endif // _GENERIC_H_
