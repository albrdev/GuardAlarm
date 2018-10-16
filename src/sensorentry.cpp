#include "sensorentry.h"

int SensorEntry::s_NextID = 0;

const RegexAssembly SensorEntry::c_StatusRegex("-1|[0-1]");

int SensorEntry::GetNextID(void)
{
    return s_NextID++;
}

/*
    ValidateStatus: Check if string compiles with status rules
*/
bool SensorEntry::ValidateStatus(const std::string& value)
{
    return std::regex_match(value, c_StatusRegex.GetRegex());
}

// Get methods
SensorStatus SensorEntry::GetStatus(void) const { return m_Status; }

// Set methods
void SensorEntry::SetStatus(const SensorStatus value) { m_Status = value; }

std::string SensorEntry::ToCSVString(void) const
{
    std::ostringstream stream;
    stream << m_ID << ';';
    stream << m_Status;

    return stream.str();
}

std::string SensorEntry::ToString(void) const
{
    std::ostringstream stream;
    stream << "SensorEntry(";
    stream << m_ID << ", ";
    stream << m_Status;
    stream << ")";

    return stream.str();
}

std::ostream& operator <<(std::ostream& stream, const SensorEntry& object)
{
    return stream << object.ToString();
}

// Constructors
SensorEntry::SensorEntry(const int id, const SensorStatus status) : Entry(id)
{
    m_Status = status;
}

SensorEntry::SensorEntry(const SensorStatus status) : Entry(0)
{
    m_Status = status;
}

SensorEntry::SensorEntry(void) { }
