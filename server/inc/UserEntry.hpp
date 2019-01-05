#ifndef _USERENTRY_HPP_
#define _USERENTRY_HPP_

#include <string>
#include <sstream>
#include "Entry.hpp"
#include "RegexAssembly.hpp"

class UserTable;

enum UserStatus
{
    US_ACTIVE = 1,
    US_INACTIVE = 2,
    US_BLOCKED = 3
};

class UserEntry : public Entry
{
    friend class UserTable;

private:
    friend std::ostream& operator <<(std::ostream& stream, const UserEntry& object);

    static int s_NextID;

protected:
    // Member variables, protected from outside interventions
    static const RegexAssembly c_PasswordRegex;
    static const RegexAssembly c_UsernameRegex;
    static const RegexAssembly c_TagIDRegex;
    static const RegexAssembly c_StatusRegex;
    static const RegexAssembly c_ReservedRegex;

    std::string m_Password = "";
    std::string m_Username = "";
    int m_TagID = -1;
    UserStatus m_Status = UserStatus::US_INACTIVE;

    static int GetNextID(void);

public:
    // Static methods to check if a string complies with the rules for Username/Password etc.
    static bool ValidatePassword(const std::string& value);
    static bool ValidateUsername(const std::string& value);
    static bool ValidateTagID(const std::string& value);
    static bool ValidateStatus(const std::string& value);

    // Get methods
    std::string GetPassword(void) const;
    std::string GetUsername(void) const; // Methods with 'const' at the end indicates that it doesn't affect the object (member variables) in any way
    int GetTagID(void) const;
    UserStatus GetStatus(void) const;
    std::string GetSecondaryPassword() const;

    // Set methods
    void SetPassword(const std::string& value);
    void SetUsername(const std::string& value);
    void SetTagID(const int value);
    void SetStatus(const UserStatus value);

    virtual std::string ToCSVString(void) const;
    virtual std::string ToString(void) const override;

    // Constructors
    UserEntry(const int id, const std::string& password, const std::string& username, const int tagID, UserStatus status = UserStatus::US_ACTIVE); // Variable 'active' har a defualt value of 'true', if not specified explicitly
    UserEntry(const int id, const std::string& password, const std::string& username, UserStatus status = UserStatus::US_ACTIVE);
    UserEntry(const std::string& password, const std::string& username, const int tagID, UserStatus status);
    UserEntry(const std::string& password, const std::string& username, UserStatus status);
    UserEntry(void);
};

#endif // _USERENTRY_HPP_
