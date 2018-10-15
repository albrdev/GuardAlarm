#ifndef _LOGTABLE_H_
#define _LOGTABLE_H_

#include <cstdlib>
#include <fstream>
#include <vector>
#include "generic.h"
#include "table.h"
#include "logentry.h"
#include "regexassembly.h"

class LogTable : public Table<LogEntry>
{
protected:
    static const RegexAssembly c_CSVRegex;

    static bool ParseCSV(const std::string& value, LogEntry& result);

public:
    bool FindByID(const int id, LogEntry& result);
    LogEntry* FindByID(const int id);

    static bool Load(const std::string& filePath, LogTable& result);

    // Constructors
    LogTable(void);
};

#endif // _LOGTABLE_H_
