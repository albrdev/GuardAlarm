#include "RegexAssembly.hpp"

// Get methods (put these on one line to save some space for readability)
std::string RegexAssembly::GetPattern(void) const { return m_Pattern; }
std::regex RegexAssembly::GetRegex(void) const { return m_Regex; }
std::regex::flag_type RegexAssembly::GetFlags(void) const { return m_Flags; }

// Set methods
void RegexAssembly::SetPattern(const std::string& value, const std::regex::flag_type flags)
{
    m_Pattern = value;
    m_Regex = std::regex(m_Pattern, flags);
    m_Flags = flags;
}

void RegexAssembly::SetPattern(const std::string& value)
{
    m_Pattern = value;
    m_Regex = std::regex(m_Pattern);
    m_Flags = (std::regex::flag_type)0;
}

void RegexAssembly::SetFlags(const std::regex::flag_type flags)
{
    m_Regex = std::regex(m_Pattern, flags);
    m_Flags = flags;
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

std::string RegexAssembly::Replace(const std::string& input, const std::string& format, const std::regex_constants::match_flag_type flags) const
{
    return std::regex_replace(input, m_Regex, format, flags);
}

std::string::const_iterator RegexAssembly::Replace(const std::string::const_iterator& start, const std::string::const_iterator& end, const std::string& format, const std::string::iterator& result, const std::regex_constants::match_flag_type flags) const
{
    return std::regex_replace(result, start, end, m_Regex, format, flags);
}

std::ostream& operator <<(std::ostream& stream, const RegexAssembly& object)
{
    return stream << object.m_Pattern;
}

// Constructors
RegexAssembly::RegexAssembly(const std::string& pattern, const std::regex::flag_type flags)
{
    SetPattern(pattern, flags);
}

RegexAssembly::RegexAssembly(const std::string& pattern)
{
    SetPattern(pattern);
}

RegexAssembly::RegexAssembly(void) { }
