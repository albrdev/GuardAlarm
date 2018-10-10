#include "credentials.h"

// Oddly, we'll have to specify the content of static arrays in the source file
const char* Credentials::USERNAME_VALIDCHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-";
const char* Credentials::PIN_VALIDCHARS = "0123456789";

/*
    ValidateUsername: Check if string compiles with basic username rules
*/
bool Credentials::ValidateUsername(const std::string& value)
{
    return (value.length() >= USERNAME_MIN && value.length() <= USERNAME_MAX) && value.find_first_not_of(USERNAME_VALIDCHARS) == std::string::npos;
}

/*
    ValidatePIN: Check if string compiles with PIN rules
*/
bool Credentials::ValidatePIN(const std::string& value)
{
    return (value.length() >= PIN_MIN && value.length() <= PIN_MAX) && value.find_first_not_of(PIN_VALIDCHARS) == std::string::npos;
}

// Get methods (put these on one line to save some space for readability)
std::string Credentials::GetUsername(void) const { return m_Username; }
std::string Credentials::GetPIN(void) const { return m_PIN; }
UserStatus Credentials::GetStatus(void) const { return m_Status; }

// Set methods
void Credentials::SetUsername(const std::string& value) { m_Username = value; }
void Credentials::SetPIN(const std::string& value) { m_PIN = value; }
void Credentials::SetStatus(const UserStatus value) { m_Status = value; }

// Constructors
Credentials::Credentials(const std::string& username, const std::string& pin, UserStatus status)
{
    m_Username = username;
    m_PIN = pin;
    m_Status = status;
}

Credentials::Credentials(void) { }
