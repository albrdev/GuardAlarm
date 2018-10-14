#ifndef _CREDENTIALS_H_
#define _CREDENTIALS_H_

#include <string>
#include <sstream>
#include "regexassembly.h"

class Database;

enum UserStatus
{
    Active = 1,
    Inactive = 2,
    Blocked = 3
};

class Credentials
{
    friend class Database;

private:
    friend std::ostream& operator <<(std::ostream& stream, const Credentials& object);

protected:
    // Member variables, protected from outside interventions
    static const RegexAssembly c_IDRegex;
    static const RegexAssembly c_PasswordRegex;
    static const RegexAssembly c_UsernameRegex;
    static const RegexAssembly c_TagIDRegex;
    static const RegexAssembly c_StatusRegex;
    static const RegexAssembly c_ReservedRegex;

    static int s_LastID;

    int m_ID = -1;
    std::string m_Password = "";
    std::string m_Username = "";
    int m_TagID = -1;
    UserStatus m_Status = UserStatus::Inactive;

    static int GenerateID(void);

public:
    // Static methods to check if a string complies with the rules for Username/PIN
    static bool ValidateID(const std::string& value);
    static bool ValidatePassword(const std::string& value);
    static bool ValidateUsername(const std::string& value);
    static bool ValidateTagID(const std::string& value);
    static bool ValidateStatus(const std::string& value);

    // Get methods
    int GetID(void) const;
    std::string GetPassword(void) const;
    std::string GetUsername(void) const; // Methods with 'const' at the end indicates that it doesn't affect the object (member variables) in any way
    int GetTagID(void) const;
    UserStatus GetStatus(void) const;

    // Set methods
    void SetID(const int value);
    void SetPassword(const std::string& value);
    void SetUsername(const std::string& value);
    void SetTagID(const int value);
    void SetStatus(const UserStatus value);

    std::string ToString(void) const;

    // Constructors
    Credentials(const int id, const std::string& password, const std::string& username, const int tagID, UserStatus status = UserStatus::Active);
    Credentials(const int id, const std::string& password, const std::string& username, UserStatus status = UserStatus::Active);
    Credentials(const std::string& password, const std::string& username, UserStatus status = UserStatus::Active); // Variable 'active' har a defualt value of 'true', if not specified explicitly
    Credentials(void);
};

#endif // _CREDENTIALS_H_
