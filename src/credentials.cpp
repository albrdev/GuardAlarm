#include "credentials.h"

// We'll have to specify the content of static variables in the source file
const char* Credentials::USERNAME_VALIDCHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-";
const char* Credentials::PASSWORD_VALIDCHARS = "0123456789";
int Credentials::s_LastID = -1;

/*
    ValidateUsername: Check if string compiles with basic username rules
*/
bool Credentials::ValidateUsername(const std::string& value)
{
    return (value.length() >= USERNAME_MIN && value.length() <= USERNAME_MAX) && value.find_first_not_of(USERNAME_VALIDCHARS) == std::string::npos;
}

/*
    ValidatePassword: Check if string compiles with password rules
*/
bool Credentials::ValidatePassword(const std::string& value)
{
    return (value.length() >= PASSWORD_MIN && value.length() <= PASSWORD_MAX) && value.find_first_not_of(PASSWORD_VALIDCHARS) == std::string::npos;
}

int Credentials::GenerateID(void)
{
    return ++s_LastID;
}

// Get methods (put these on one line to save some space for readability)
int Credentials::GetID(void) const { return m_ID; }
int Credentials::GetTagID(void) const { return m_TagID; }
std::string Credentials::GetUsername(void) const { return m_Username; }
std::string Credentials::GetPassword(void) const { return m_Password; }
UserStatus Credentials::GetStatus(void) const { return m_Status; }

// Set methods
void Credentials::SetID(const int value) { m_ID = value; }
void Credentials::SetTagID(const int value) { m_TagID = value; }
void Credentials::SetUsername(const std::string& value) { m_Username = value; }
void Credentials::SetPassword(const std::string& value) { m_Password = value; }
void Credentials::SetStatus(const UserStatus value) { m_Status = value; }

std::string Credentials::ToString(void) const
{
    std::ostringstream stream;
    stream << "Credentials(";
    stream << m_ID << ", ";
    stream << m_TagID << ", ";
    stream << m_Username << ", ";
    stream << m_Password << ", ";
    stream << m_Status;
    stream << ")";

    return stream.str();
}

std::ostream& operator <<(std::ostream &stream, Credentials const &object)
{
    return stream << object.ToString();
}

// Constructors
Credentials::Credentials(const int id, const int tagID, const std::string& username, const std::string& password, UserStatus status)
{
    m_ID = id;
    m_TagID = tagID;
    m_Username = username;
    m_Password = password;
    m_Status = status;
}

Credentials::Credentials(const int id, const std::string& username, const std::string& password, UserStatus status)
{
    m_ID = id;
    m_Username = username;
    m_Password = password;
    m_Status = status;
}

Credentials::Credentials(const std::string& username, const std::string& password, UserStatus status)
{
    m_ID = Credentials::GenerateID();
    m_Username = username;
    m_Password = password;
    m_Status = status;
}

Credentials::Credentials(void) { }
