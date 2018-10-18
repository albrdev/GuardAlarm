#ifndef _ENTRY_H_
#define _ENTRY_H_

#include <string>
#include <sstream>
#include "regexassembly.h"

class Entry
{
private:
    friend std::ostream& operator <<(std::ostream& stream, const Entry& object);

protected:
    static const RegexAssembly c_IDRegex;

    int m_ID = -1;

public:
    static bool ValidateID(const std::string& value);

    // Get methods
    int GetID(void) const;

    // Set methods
    void SetID(const int value);

    virtual std::string ToString(void) const;

    // Constructors
    Entry(const int id);
    Entry(void);
};

#endif // _ENTRY_H_
