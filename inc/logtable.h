#ifndef _LOGTABLE_H_
#define _LOGTABLE_H_

#include <cstdlib>
#include <fstream>
#include <vector>
#include "generic.h"
#include "regexassembly.h"
#include "logentry.h"

class LogTable
{
protected:
    static const RegexAssembly c_CSVRegex;

    std::vector<LogEntry> m_Content; // Underlying content

    static bool ParseCSV(const std::string& value, LogEntry& result);

public:
    std::size_t Count(void) const;
    bool Empty(void) const;
    void Add(const LogEntry& item);
    void Remove(std::size_t index);
    void Clear(void);

    LogEntry operator [](const std::size_t index) const;
    LogEntry operator [](const int id) const;

    LogEntry* FindByID(const int id);

    static bool Load(const std::string& filePath, LogTable& result);

    // Constructors
    LogTable(void);
};

#endif // _LOGTABLE_H_
