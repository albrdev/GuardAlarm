#ifndef _REGEXASSEMBLY_HPP_
#define _REGEXASSEMBLY_HPP_

#include <string>
#include <regex>

class RegexAssembly
{
private:
    friend std::ostream& operator <<(std::ostream& stream, const RegexAssembly& object);

protected:
    std::string m_Pattern = "";
    std::regex m_Regex;
    std::regex::flag_type m_Flags = (std::regex::flag_type)0;

public:
    // Get methods
    std::string GetPattern(void) const;
    std::regex GetRegex(void) const;
    std::regex::flag_type GetFlags(void) const;

    // Set methods
    void SetPattern(const std::string& value, const std::regex::flag_type flags);
    void SetPattern(const std::string& value);
    void SetFlags(const std::regex::flag_type flags);

    bool Match(const std::string& input, const std::regex_constants::match_flag_type flags = std::regex_constants::match_default) const;
    bool Match(const std::string& input, std::smatch& result, const std::regex_constants::match_flag_type flags = std::regex_constants::match_default) const;
    bool Match(const std::string::const_iterator& start, const std::string::const_iterator& end, const std::regex_constants::match_flag_type flags = std::regex_constants::match_default) const;
    bool Match(const std::string::const_iterator& start, const std::string::const_iterator& end, std::smatch& result, const std::regex_constants::match_flag_type flags = std::regex_constants::match_default) const;

    std::string Replace(const std::string& input, const std::string& format, const std::regex_constants::match_flag_type flags) const;
    std::string::const_iterator Replace(const std::string::const_iterator& start, const std::string::const_iterator& end, const std::string& format, const std::string::iterator& result, const std::regex_constants::match_flag_type flags) const;

    // Constructors
    RegexAssembly(const std::string& pattern, const std::regex::flag_type flags);
    RegexAssembly(const std::string& pattern);
    RegexAssembly(void);
};

#endif // _REGEXASSEMBLY_HPP_
