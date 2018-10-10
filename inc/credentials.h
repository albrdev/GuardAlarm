#ifndef _CREDENTIALS_H_
#define _CREDENTIALS_H_

#include <string>

enum UserStatus
{
    Active = 1,
    Inactive = 2,
    Blocked = 3
};

class Credentials
{
protected:
    // Member variables, protected from outside interventions
    static const int USERNAME_MIN = 3;
    static const int USERNAME_MAX = 16;
    static const char* USERNAME_VALIDCHARS;

    static const int PIN_MIN = 4;
    static const int PIN_MAX = 6;
    static const char* PIN_VALIDCHARS;

    int m_ID = -1;
    std::string m_Username = "";
    std::string m_PIN = "";
    int m_TagID = -1;
    UserStatus m_Status = UserStatus::Inactive;

public:
    // Static methods to check if a string complies with the rules for Username/PIN
    static bool ValidateUsername(const std::string& value);
    static bool ValidatePIN(const std::string& value);

    // Get methods
    std::string GetUsername(void) const; // Methods with 'const' at the end indicates that it doesn't affect the object (member variables) in any way
    std::string GetPIN(void) const;
    UserStatus GetStatus(void) const;

    // Set methods
    void SetUsername(const std::string& value);
    void SetPIN(const std::string& value);
    void SetStatus(const UserStatus value);

    // Constructors
    Credentials(const std::string& username, const std::string& pin, UserStatus status = UserStatus::Active); // Variable 'active' har a defualt value of 'true', if not specified explicitly
    Credentials(void);
};

#endif // _CREDENTIALS_H_
