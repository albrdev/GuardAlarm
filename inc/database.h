#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <cstdlib>
#include <fstream>
#include <vector>
#include "generic.h"
#include "table.h"
#include "credentials.h"
#include "regexassembly.h"

class Database : public Table<Credentials>
{
protected:
    static const RegexAssembly c_CSVRegex;

    static bool ParseCSV(const std::string& value, Credentials& result);

public:
    bool FindByID(const int id, Credentials& result);
    Credentials* FindByID(const int id);
    bool FindByUsername(const std::string& username, Credentials& result);
    Credentials* FindByUsername(const std::string& username);
    Credentials* FindByPassword(const std::string& password);
    Credentials* FindBySecondaryPassword(const std::string& password);

    static bool Load(const std::string& filePath, Database& result);

    // Constructors
    Database(void);
};

#endif // _DATABASE_H_
