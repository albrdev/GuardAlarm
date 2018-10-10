#include "credentials.h"

const RegexAssembly Credentials::c_IDRegex("[0-9]+");
const RegexAssembly Credentials::c_TagIDRegex("[0-9]*");
const RegexAssembly Credentials::c_UsernameRegex("[a-zA-Z]+");
const RegexAssembly Credentials::c_PasswordRegex("[0-9]{4,6}");
const RegexAssembly Credentials::c_StatusRegex("[1-3]");
const RegexAssembly Credentials::c_ReservedRegex(".*");
const RegexAssembly Credentials::c_CSVRegex("(" + Credentials::c_IDRegex.GetPattern() + ");" + "(" + Credentials::c_PasswordRegex.GetPattern() + ");" + "(" + Credentials::c_UsernameRegex.GetPattern() + ");" + "(" + Credentials::c_TagIDRegex.GetPattern() + ");" + "(" + Credentials::c_StatusRegex.GetPattern() + ");" + "(" + Credentials::c_ReservedRegex.GetPattern() + ")");

int Credentials::s_LastID = -1;

int Credentials::GenerateID(void)
{
    return ++s_LastID;
}

bool Credentials::ValidateID(const std::string& value)
{
    return std::regex_match(value, c_IDRegex.GetRegex());
}

bool Credentials::ValidateTagID(const std::string& value)
{
    return std::regex_match(value, c_TagIDRegex.GetRegex());
}

/*
    ValidateUsername: Check if string compiles with basic username rules
*/
bool Credentials::ValidateUsername(const std::string& value)
{
    return std::regex_match(value, c_UsernameRegex.GetRegex());
}

/*
    ValidatePassword: Check if string compiles with password rules
*/
bool Credentials::ValidatePassword(const std::string& value)
{
    return std::regex_match(value, c_PasswordRegex.GetRegex());
}

bool Credentials::ValidateStatus(const std::string& value)
{
    return std::regex_match(value, c_StatusRegex.GetRegex());
}

bool Credentials::ParseCSV(const std::string& value, Credentials& result)
{
    std::smatch match;
    if(!std::regex_match(value, match, c_CSVRegex.GetRegex()))
    {
        return false;
    }

    result = Credentials(std::stoi(match[1]), match[3], match[2], (UserStatus)std::stoi(match[5]));
    if(!((std::string)match[4]).empty())
    {
        result.SetTagID(std::stoi(match[4]));
    }

    return true;
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

std::ostream& operator <<(std::ostream &stream, const Credentials& object)
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
