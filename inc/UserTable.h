#ifndef _USERTABLE_H_
#define _USERTABLE_H_

#include <cstdlib>
#include <fstream>
#include <vector>
#include "generic.h"
#include "Table.h"
#include "UserEntry.h"
#include "RegexAssembly.h"

class UserTable : public Table<UserEntry>
{
protected:
    static const RegexAssembly c_CSVRegex;

    static bool ParseCSV(const std::string& value, UserEntry& result);

public:
    bool FindByID(const int id, UserEntry& result);
    UserEntry* FindByID(const int id);
    bool FindByUsername(const std::string& username, UserEntry& result);
    UserEntry* FindByUsername(const std::string& username);
    UserEntry* FindByPassword(const std::string& password);
    UserEntry* FindBySecondaryPassword(const std::string& password);

    static bool Load(const std::string& filePath, UserTable& result);

    // Constructors
    UserTable(void);
};

#endif // _USERTABLE_H_
