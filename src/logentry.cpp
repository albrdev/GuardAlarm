#include "logentry.h"

int LogEntry::s_NextID = 0;

const RegexAssembly LogEntry::c_TimestampRegex("[0-9]{4}\\.[0-9]{2}\\.[0-9]{2}\\s+[0-9]{2}:[0-9]{2}:[0-9]{2}");
const RegexAssembly LogEntry::c_UserIDRegex("[0-9]+");
const RegexAssembly LogEntry::c_ReservedRegex("[^\\s;]*");
const RegexAssembly LogEntry::c_MessageRegex("\\S.+?");

int LogEntry::GetNextID(void)
{
    return s_NextID++;
}

bool LogEntry::ValidateTimestamp(const std::string& value)
{
    return std::regex_match(value, c_TimestampRegex.GetRegex());
}

/*
    ValidateUserID: Check if string compiles with user ID rules
*/
bool LogEntry::ValidateUserID(const std::string& value)
{
    return std::regex_match(value, c_UserIDRegex.GetRegex());
}

/*
    ValidateMessage: Check if string compiles with message rules
*/
bool LogEntry::ValidateMessage(const std::string& value)
{
    return value.length() <= 100 && std::regex_match(value, c_MessageRegex.GetRegex());
}

// Get methods
std::time_t LogEntry::GetTimestamp(void) const { return m_Timestamp; }
int LogEntry::GetUserID(void) const { return m_UserID; }
std::string LogEntry::GetMessage(void) const { return m_Message; }

// Set methods
void LogEntry::SetTimestamp(const std::time_t value) { m_Timestamp = value; }
void LogEntry::SetUserID(const int value) { m_UserID = value; }
void LogEntry::SetMessage(const std::string& value) { m_Message = value; }

std::string LogEntry::ToCSVString(void) const
{
    std::ostringstream stream;
    stream << m_ID << ';';
    stream << timetostr("%Y.%m.%d %H:%M:%S", m_Timestamp) << ';';
    stream << m_UserID << ';';
    stream << "" << ';'; // Reserved
    stream << m_Message;

    return stream.str();
}

std::string LogEntry::ToString(void) const
{
    std::ostringstream stream;
    stream << "LogEntry(";
    stream << m_ID << ", ";
    stream << timetostr("%Y.%m.%d %H:%M:%S", m_Timestamp) << ", ";
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
LogEntry::LogEntry(const int id, const std::time_t timestamp, const int userID, const std::string& message) : Entry(id)
{
    m_Timestamp = timestamp;
    m_UserID = userID;
    m_Message = message;

    if(id > s_NextID)
    {
        s_NextID = id + 1;
    }
}

LogEntry::LogEntry(const std::time_t timestamp, const int userID, const std::string& message) : Entry(GetNextID())
{
    m_Timestamp = timestamp;
    m_UserID = userID;
    m_Message = message;
}

LogEntry::LogEntry(void) { }
