#include "regexassembly.h"

// Get methods (put these on one line to save some space for readability)
std::string RegexAssembly::GetPattern(void) const { return m_Pattern; }
std::regex RegexAssembly::GetRegex(void) const { return m_Regex; }

// Set methods
void RegexAssembly::SetPattern(const std::string& value)
{
    m_Pattern = value;
    m_Regex = std::regex(m_Pattern);
}

bool RegexAssembly::Match(const std::string& input, const std::regex_constants::match_flag_type flags) const
{
    return std::regex_match(input, m_Regex, flags);
}

bool RegexAssembly::Match(const std::string& input, std::smatch& result, const std::regex_constants::match_flag_type flags) const
{
    return std::regex_match(input, result, m_Regex, flags);
}

bool RegexAssembly::Match(const std::string::const_iterator& start, const std::string::const_iterator& end, const std::regex_constants::match_flag_type flags) const
{
    return std::regex_match(start, end, m_Regex, flags);
}

bool RegexAssembly::Match(const std::string::const_iterator& start, const std::string::const_iterator& end, std::smatch& result, const std::regex_constants::match_flag_type flags) const
{
    return std::regex_match(start, end, result, m_Regex, flags);
}

std::ostream& operator <<(std::ostream& stream, const RegexAssembly& object)
{
    return stream << object.m_Pattern;
}

// Constructors
RegexAssembly::RegexAssembly(const std::string& pattern)
{
    SetPattern(pattern);
}

RegexAssembly::RegexAssembly(void) { }
