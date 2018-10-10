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

    bool Match(const std::string& input, const std::regex_constants::match_flag_type flags = std::regex_constants::match_default) const;
    bool Match(const std::string& input, std::smatch& result, const std::regex_constants::match_flag_type flags = std::regex_constants::match_default) const;
    bool Match(const std::string::const_iterator& start, const std::string::const_iterator& end, const std::regex_constants::match_flag_type flags = std::regex_constants::match_default) const;
    bool Match(const std::string::const_iterator& start, const std::string::const_iterator& end, std::smatch& result, const std::regex_constants::match_flag_type flags = std::regex_constants::match_default) const;

    // Constructors
    RegexAssembly(const std::string& pattern);
    RegexAssembly(void);
};

#endif // _REGEXASSEMBLY_H_
