#include "userentry.h"

int UserEntry::s_NextID = 0;

const RegexAssembly UserEntry::c_PasswordRegex("[0-9]{4,6}");
const RegexAssembly UserEntry::c_UsernameRegex("[a-zA-Z]+");
const RegexAssembly UserEntry::c_TagIDRegex("[0-9]*");
const RegexAssembly UserEntry::c_StatusRegex("[1-3]");
const RegexAssembly UserEntry::c_ReservedRegex("[^\\s;]*");

/*
    GetNextID: Gets next available ID using an underlying static integer that keeps track of the highest ID + 1
*/
int UserEntry::GetNextID(void)
{
    return s_NextID++;
}

/*
    ValidatePassword: Check if string compiles with password rules
*/
bool UserEntry::ValidatePassword(const std::string& value)
{
    return std::regex_match(value, c_PasswordRegex.GetRegex());
}

/*
    ValidateUsername: Check if string compiles with basic username rules
*/
bool UserEntry::ValidateUsername(const std::string& value)
{
    return std::regex_match(value, c_UsernameRegex.GetRegex());
}

bool UserEntry::ValidateTagID(const std::string& value)
{
    return std::regex_match(value, c_TagIDRegex.GetRegex());
}

bool UserEntry::ValidateStatus(const std::string& value)
{
    return std::regex_match(value, c_StatusRegex.GetRegex());
}

// Get methods (put these on one line to save some space for readability)
std::string UserEntry::GetPassword(void) const { return m_Password; }
std::string UserEntry::GetUsername(void) const { return m_Username; }
int UserEntry::GetTagID(void) const { return m_TagID; }
UserStatus UserEntry::GetStatus(void) const { return m_Status; }

std::string UserEntry::GetSecondaryPassword() const
{
    std::string temp = m_Password;
    size_t lastIndex = temp.length() - 1;
    temp[lastIndex] = (temp[lastIndex] == '9' ? '0' : (char)(temp[lastIndex] + 1));
    return temp;
}

// Set methods
void UserEntry::SetPassword(const std::string& value) { m_Password = value; }
void UserEntry::SetUsername(const std::string& value) { m_Username = value; }
void UserEntry::SetTagID(const int value) { m_TagID = value; }
void UserEntry::SetStatus(const UserStatus value) { m_Status = value; }

std::string UserEntry::ToCSVString(void) const
{
    std::ostringstream stream;
    stream << m_ID << ';';
    stream << m_Password << ';';
    stream << m_Username << ';';
    stream << m_TagID << ';';
    stream << m_Status << ';';
    stream << ""; // Reserved

    return stream.str();
}

std::string UserEntry::ToString(void) const
{
    std::ostringstream stream;
    stream << "UserEntry(";
    stream << m_ID << ", ";
    stream << m_Password << ", ";
    stream << m_Username << ", ";
    stream << m_TagID << ", ";
    stream << m_Status;
    stream << ")";

    return stream.str();
}

std::ostream& operator <<(std::ostream& stream, const UserEntry& object)
{
    return stream << object.ToString();
}

// Constructors
UserEntry::UserEntry(const int id, const std::string& password, const std::string& username, const int tagID, UserStatus status) : UserEntry(id, password, username, status)
{
    m_TagID = tagID;
}

UserEntry::UserEntry(const int id, const std::string& password, const std::string& username, UserStatus status) : Entry(id)
{
    m_Password = password;
    m_Username = username;
    m_Status = status;

    if(id > s_NextID)
    {
        s_NextID = id + 1;
    }
}

UserEntry::UserEntry(const std::string& password, const std::string& username, const int tagID, UserStatus status) : UserEntry(password, username, status)
{
    m_TagID = tagID;
}

UserEntry::UserEntry(const std::string& password, const std::string& username, UserStatus status) : Entry(GetNextID())
{
    m_Password = password;
    m_Username = username;
    m_Status = status;
}

UserEntry::UserEntry(void) { }
