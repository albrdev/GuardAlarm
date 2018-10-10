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

std::ostream& operator <<(std::ostream &stream, RegexAssembly const &object)
{
    return stream << object.m_Pattern;
}

// Constructors
RegexAssembly::RegexAssembly(const std::string& pattern)
{
    SetPattern(pattern);
}

RegexAssembly::RegexAssembly(void) { }
