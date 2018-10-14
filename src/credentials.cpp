#include "credentials.h"

const RegexAssembly Credentials::c_PasswordRegex("[0-9]{4,6}");
const RegexAssembly Credentials::c_UsernameRegex("[a-zA-Z]+");
const RegexAssembly Credentials::c_TagIDRegex("[0-9]*");
const RegexAssembly Credentials::c_StatusRegex("[1-3]");
const RegexAssembly Credentials::c_ReservedRegex("\\S*");

/*
    ValidatePassword: Check if string compiles with password rules
*/
bool Credentials::ValidatePassword(const std::string& value)
{
    return std::regex_match(value, c_PasswordRegex.GetRegex());
}

/*
    ValidateUsername: Check if string compiles with basic username rules
*/
bool Credentials::ValidateUsername(const std::string& value)
{
    return std::regex_match(value, c_UsernameRegex.GetRegex());
}

bool Credentials::ValidateTagID(const std::string& value)
{
    return std::regex_match(value, c_TagIDRegex.GetRegex());
}

bool Credentials::ValidateStatus(const std::string& value)
{
    return std::regex_match(value, c_StatusRegex.GetRegex());
}

// Get methods (put these on one line to save some space for readability)
std::string Credentials::GetPassword(void) const { return m_Password; }
std::string Credentials::GetUsername(void) const { return m_Username; }
int Credentials::GetTagID(void) const { return m_TagID; }
UserStatus Credentials::GetStatus(void) const { return m_Status; }

// Set methods
void Credentials::SetPassword(const std::string& value) { m_Password = value; }
void Credentials::SetUsername(const std::string& value) { m_Username = value; }
void Credentials::SetTagID(const int value) { m_TagID = value; }
void Credentials::SetStatus(const UserStatus value) { m_Status = value; }

std::string Credentials::ToString(void) const
{
    std::ostringstream stream;
    stream << "Credentials(";
    stream << m_ID << ", ";
    stream << m_Password << ", ";
    stream << m_Username << ", ";
    stream << m_TagID << ", ";
    stream << m_Status;
    stream << ")";

    return stream.str();
}

std::ostream& operator <<(std::ostream& stream, const Credentials& object)
{
    return stream << object.ToString();
}

// Constructors
Credentials::Credentials(const int id, const std::string& password, const std::string& username, const int tagID, UserStatus status) : Entry(id)
{
    m_Password = password;
    m_Username = username;
    m_TagID = tagID;
    m_Status = status;
}

Credentials::Credentials(const int id, const std::string& password, const std::string& username, UserStatus status) : Entry(id)
{
    m_Password = password;
    m_Username = username;
    m_Status = status;
}

Credentials::Credentials(void) { }
