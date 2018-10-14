#include "entry.h"

const RegexAssembly Entry::c_IDRegex("[0-9]+");

bool Entry::ValidateID(const std::string& value)
{
    return std::regex_match(value, c_IDRegex.GetRegex());
}

// Get methods
int Entry::GetID(void) const { return m_ID; }

// Set methods
void Entry::SetID(const int value) { m_ID = value; }

std::string Entry::ToString(void) const
{
    std::ostringstream stream;
    stream << "Entry(";
    stream << m_ID;
    stream << ")";

    return stream.str();
}

std::ostream& operator <<(std::ostream& stream, const Entry& object)
{
    return stream << object.ToString();
}

// Constructors
Entry::Entry(const int id)
{
    m_ID = id;
}

Entry::Entry(void) { }
