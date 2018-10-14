#include "generic.h"

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
                Time format will be in HH:MM:SS
*/
std::string timeString(time_t t)
{
    char timeStringBuffer[9]; // A maximum of 8 + 1 ('\0') characters will be written
    struct tm* timeInfo = localtime(&t);
    strftime(timeStringBuffer, sizeof(timeStringBuffer), "%H:%M:%S", timeInfo);
    return timeStringBuffer;
}

std::time_t strtotime(const char *const str)
{
    struct tm ts = { };
    if(std::sscanf(str, "%4d.%2d.%2d %2d:%2d:%2d", &ts.tm_year, &ts.tm_mon, &ts.tm_mday, &ts.tm_hour, &ts.tm_min, &ts.tm_sec) != 6)
    {
        return (std::time_t) - 1;
    }

    ts.tm_year -= 1900;
    ts.tm_mon -= 1;
    return std::mktime(&ts);
}
