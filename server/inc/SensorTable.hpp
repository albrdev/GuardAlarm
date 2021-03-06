#ifndef _SENSORTABLE_HPP_
#define _SENSORTABLE_HPP_

#include <cstdlib>
#include <fstream>
#include <vector>
#include "Generic.hpp"
#include "Table.hpp"
#include "SensorEntry.hpp"
#include "RegexAssembly.hpp"
#include "shared.h"

class SensorTable : public Table<SensorEntry>
{
protected:
    static const RegexAssembly c_CSVRegex;

    static bool ParseCSV(const std::string& value, SensorEntry& result);

public:
    bool FindByID(const int id, SensorEntry& result);
    SensorEntry* FindByID(const int id);

    static bool Save(const std::string& filePath, const SensorTable& content);
    static bool Load(const std::string& filePath, SensorTable& result);

    // Constructors
    SensorTable(void);
};

#endif // _SENSORTABLE_HPP_
