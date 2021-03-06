#ifndef _LOGENTRY_HPP_
#define _LOGENTRY_HPP_

#include <ctime>
#include <string>
#include <sstream>
#include "Generic.hpp"
#include "Entry.hpp"
#include "RegexAssembly.hpp"

class LogEntry : public Entry
{
    friend class LogTable;

private:
    friend std::ostream& operator <<(std::ostream& stream, const LogEntry& object);

    static int s_NextID;

protected:
    // Member variables, protected from outside interventions
    static const RegexAssembly c_TimestampRegex;
    static const RegexAssembly c_UserIDRegex;
    static const RegexAssembly c_ReservedRegex;
    static const RegexAssembly c_MessageRegex;

    std::time_t m_Timestamp = 0;
    int m_UserID = -1;
    std::string m_Message = "";

    static int GetNextID(void);

public:
    // Static methods to check if a string complies with the rules for Username/PIN
    static bool ValidateTimestamp(const std::string& value);
    static bool ValidateUserID(const std::string& value);
    static bool ValidateMessage(const std::string& value);

    // Get methods
    std::time_t GetTimestamp(void) const;
    int GetUserID(void) const; // Methods with 'const' at the end indicates that it doesn't affect the object (member variables) in any way
    std::string GetMessage(void) const;

    // Set methods
    void SetTimestamp(const std::time_t value);
    void SetUserID(const int value);
    void SetMessage(const std::string& value);

    virtual std::string ToCSVString(void) const;
    virtual std::string ToString(void) const override;

    // Constructors
    LogEntry(const int id, const std::time_t timestamp, const int userID, const std::string& message);
    LogEntry(const std::time_t timestamp, const int userID, const std::string& message);
    LogEntry(void);
};

#endif // _LOGENTRY_HPP_
