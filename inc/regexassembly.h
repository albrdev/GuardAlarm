#ifndef _REGEXASSEMBLY_H_
#define _REGEXASSEMBLY_H_

#include <string>
#include <regex>

class RegexAssembly
{
private:
    friend std::ostream& operator <<(std::ostream& stream, const RegexAssembly& object);

protected:
    std::string m_Pattern = "";
    std::regex m_Regex;

public:
    // Get methods
    std::string GetPattern(void) const;
    std::regex GetRegex(void) const;

    // Set methods
    void SetPattern(const std::string& value);

    // Constructors
    RegexAssembly(const std::string& pattern);
    RegexAssembly(void);
};

#endif // _REGEXASSEMBLY_H_
