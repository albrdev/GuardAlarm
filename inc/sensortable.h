#ifndef _SENSORTABLE_H_
#define _SENSORTABLE_H_

#include <cstdlib>
#include <fstream>
#include <vector>
#include "generic.h"
#include "Table.h"
#include "SensorEntry.h"
#include "RegexAssembly.h"

class SensorTable : public Table<SensorEntry>
{
protected:
    static const RegexAssembly c_CSVRegex;

    static bool ParseCSV(const std::string& value, SensorEntry& result);

public:
    bool FindByID(const int id, SensorEntry& result);
    SensorEntry* FindByID(const int id);

    static bool Load(const std::string& filePath, SensorTable& result);

    // Constructors
    SensorTable(void);
};

#endif // _SENSORTABLE_H_
