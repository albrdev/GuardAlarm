#include "Generic.h"

/*
    readString: Reads a string from standard input stream
*/
bool readString(std::string& result)
{
    std::getline(std::cin, result);
    if(!std::cin.good()) // Check if any of fail/bad/eof bits are set. std::getline could set them somehow according to documentation (maybe that's not the case with just std::cin)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>().max());
        return false;
    }

    return true;
}

/*
    readChar: Reads a single character from the standard input stream
*/
bool readChar(char& result)
{
    std::cin.get(result);
    if(!std::cin.good())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max());
        return false;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Flush the rest of thecharacters in the buffer so it won't interfere with further input
    return true;
}

/*
    strcmpic: Compare two strings regardless of letter casing (Function named after 'strcmp')
              Returns 0 if both strings are equal
*/
int strcmpic(const std::string& a, const std::string& b)
{
    if(a.length() != b.length()) // Different lengths cannot be the same
    {
        return (int)(a.length() - b.length());
    }

    for(size_t i = 0; i < a.length(); i++)
    {
        if(tolower(a[i]) != tolower(b[i])) // Get the lower case characters in both strings at the same index and compare
        {
            return a[i] - b[i];
        }
    }

    return 0;
}

/*
    timeString: Generates a human readable string representing the time provided by the parameter
*/
std::string timetostr(const std::string& format, const time_t value)
{
    char timeStringBuffer[256 + 1];
    struct tm* timeInfo = localtime(&value);
    strftime(timeStringBuffer, sizeof(timeStringBuffer), format.c_str(), timeInfo);
    return timeStringBuffer;
}

std::string timetostr(const std::string& format)
{
    char timeStringBuffer[256 + 1];
    time_t value = time(NULL);
    struct tm* timeInfo = localtime(&value);
    strftime(timeStringBuffer, sizeof(timeStringBuffer), format.c_str(), timeInfo);
    return timeStringBuffer;
}

std::time_t strtotime(const char *const str)
{
    struct tm ts;
    memset(&ts, 0, sizeof(ts));
    ts.tm_isdst = -1;

    if(std::sscanf(str, "%4d.%2d.%2d %2d:%2d:%2d", &ts.tm_year, &ts.tm_mon, &ts.tm_mday, &ts.tm_hour, &ts.tm_min, &ts.tm_sec) != 6)
    {
        return (std::time_t) - 1;
    }

    ts.tm_year -= 1900;
    ts.tm_mon -= 1;
    return std::mktime(&ts);
}

template<typename T> std::string ToString(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

template<typename T> T FromString(const std::string& str)
{
    std::istringstream iss(str);
    T ret;
    iss >> ret;
    return ret;
}

template<typename T> bool FromString(const std::string& str, T& result)
{
    std::istringstream iss(str);
    iss >> result;
    return !iss.fail();
}
