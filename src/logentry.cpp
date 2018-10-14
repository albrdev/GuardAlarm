#include "logentry.h"

const RegexAssembly LogEntry::c_IDRegex("[0-9]+");
const RegexAssembly LogEntry::c_TimestampRegex("[0-9]{4}[0-9]{2}[0-9]{2} [0-9]{2}:[0-9]{2}:[0-9]{2}");
const RegexAssembly LogEntry::c_UserIDRegex("[0-9]+");
const RegexAssembly LogEntry::c_ReservedRegex("[^\\s;]*");
const RegexAssembly LogEntry::c_MessageRegex("\\S*");

bool LogEntry::ValidateID(const std::string& value)
{
    return std::regex_match(value, c_IDRegex.GetRegex());
}

bool LogEntry::ValidateTimestamp(const std::string& value)
{
    return std::regex_match(value, c_TimestampRegex.GetRegex());
}

/*
    ValidateUsername: Check if string compiles with basic username rules
*/
bool LogEntry::ValidateUserID(const std::string& value)
{
    return std::regex_match(value, c_UserIDRegex.GetRegex());
}

/*
    ValidatePassword: Check if string compiles with password rules
*/
bool LogEntry::ValidateMessage(const std::string& value)
{
    return std::regex_match(value, c_MessageRegex.GetRegex());
}

// Get methods (put these on one line to save some space for readability)
int LogEntry::GetID(void) const { return m_ID; }
std::time_t LogEntry::GetTimestamp(void) const { return m_Timestamp; }
int LogEntry::GetUserID(void) const { return m_UserID; }
std::string LogEntry::GetMessage(void) const { return m_Message; }

// Set methods
void LogEntry::SetID(const int value) { m_ID = value; }
void LogEntry::SetTimestamp(const std::time_t value) { m_Timestamp = value; }
void LogEntry::SetUserID(const int value) { m_UserID = value; }
void LogEntry::SetMessage(const std::string& value) { m_Message = value; }

std::string LogEntry::ToString(void) const
{
    std::ostringstream stream;
    stream << "Credentials(";
    stream << m_ID << ", ";
    stream << m_Timestamp << ", ";
    stream << m_UserID << ", ";
    stream << m_Message;
    stream << ")";

    return stream.str();
}

std::ostream& operator <<(std::ostream& stream, const LogEntry& object)
{
    return stream << object.ToString();
}

// Constructors
LogEntry::LogEntry(const int id, const std::time_t timestamp, const int userID, const std::string& message)
{
    m_ID = id;
    m_Timestamp = timestamp;
    m_UserID = userID;
    m_Message = message;
}

LogEntry::LogEntry(void) { }
