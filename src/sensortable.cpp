#include "SensorTable.h"

// Regex explanation:
// ^\s*([0-9]+)\s*;\s*(-1|[0-1])\s*$
//
// ^ = Match the start of the string.
// $ = Match the end of the string
// \s* = Match any number of whitespace between value fields (allowing for possible (and unnecessary) spaces in between).
// ; = Match the literal comma that separates the fields.

// ([0-9]+) = For the first field (ID); match any base-10 digit, one or more times.
// (-1|[0-1]) = For the second field (Status); match -1, 0 or 1, one single time.
const RegexAssembly SensorTable::c_CSVRegex("^\\s*([0-9]+)\\s*;\\s*(-1|[0-1])\\s*$");

bool SensorTable::FindByID(const int id, SensorEntry& result)
{
    for(size_t i = 0; i < m_Content.size(); i++)
    {
        if(m_Content[i].GetID() == id)
        {
            result = m_Content[i];
            return true;
        }
    }

    return false;
}

SensorEntry* SensorTable::FindByID(const int id)
{
    for(size_t i = 0; i < m_Content.size(); i++)
    {
        if(m_Content[i].GetID() == id)
        {
            return &m_Content[i];
        }
    }

    return NULL;
}

bool SensorTable::ParseCSV(const std::string& value, SensorEntry& result)
{
    std::smatch match;
    if(!c_CSVRegex.Match(value, match))
    {
        return false;
    }

    result = SensorEntry(std::stoi(match[1]), (SensorStatus)std::stoi(match[2]));

    return true;
}

bool SensorTable::Load(const std::string& filePath, SensorTable& result)
{
    std::fstream stream(filePath, std::ifstream::in);
    if(!stream.is_open())
    {
        return false;
    }

    int highestID = std::numeric_limits<int>().min();
    std::string line;
    while(std::getline(stream, line))
    {
        if(line.empty())
        {
            continue;
        }

        SensorEntry entry;
        if(!SensorTable::ParseCSV(line, entry))
        {
            stream.close();
            return false;
        }

        result.Add(entry);
        if(entry.GetID() > highestID)
        {
            highestID = entry.GetID();
        }
    }

    if(stream.bad())
    {
        stream.close();
        return false;
    }

    stream.close();
    SensorEntry::s_NextID = highestID + 1;
    return true;
}

// Constructors
SensorTable::SensorTable(void) { }
