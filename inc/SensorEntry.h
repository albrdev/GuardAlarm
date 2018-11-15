#ifndef _SENSORENTRY_H_
#define _SENSORENTRY_H_

#include <ctime>
#include <string>
#include <sstream>
#include "Generic.h"
#include "Entry.h"
#include "RegexAssembly.h"

enum SensorStatus
{
    Open = 1,
    Closed = 0,
    Failure = -1
};

class SensorEntry : public Entry
{
    friend class SensorTable;

private:
    friend std::ostream& operator <<(std::ostream& stream, const SensorEntry& object);

    static int s_NextID;

protected:
    static const RegexAssembly c_StatusRegex;

    SensorStatus m_Status = SensorStatus::Closed;

    static int GetNextID(void);

public:
    // Static methods to check if a string complies with the rules for Username/PIN
    static bool ValidateStatus(const std::string& value);

    // Get methods
    SensorStatus GetStatus(void) const;

    // Set methods
    void SetStatus(const SensorStatus value);

    virtual std::string ToCSVString(void) const;
    virtual std::string ToString(void) const override;

    // Constructors
    SensorEntry(const int id, SensorStatus status);
    SensorEntry(const SensorStatus status);
    SensorEntry(void);
};

#endif // _SENSORENTRY_H_
