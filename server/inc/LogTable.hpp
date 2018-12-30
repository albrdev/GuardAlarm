#ifndef _LOGTABLE_HPP_
#define _LOGTABLE_HPP_

#include <cstdlib>
#include <fstream>
#include <vector>
#include "Generic.hpp"
#include "Table.hpp"
#include "LogEntry.hpp"
#include "RegexAssembly.hpp"

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

#endif // _LOGTABLE_HPP_
