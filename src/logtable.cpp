#include "logtable.h"

// Regex explanation:
// ^\s*([0-9]+)\s*;\s*([0-9]{4}\.[0-9]{2}\.[0-9]{2}\s+[0-9]{2}:[0-9]{2}:[0-9]{2})\s*;\s*([0-9]+)\s*;\s*([^\s;]*)\s*;\s*(\S+)\s*$
//
// ^ = Match the start of a string.
// $ = Match the end of a string
// \s* = Match any number of whitespace between value fields (allowing for possible (and unnecessary) spaces in between).
// ; = Match the literal comma that separates the fields.

// ([0-9]+) = For the first field (ID); match any base-10 digit, one or more times.
// ([0-9]{4}\.[0-9]{2}\.[0-9]{2}\s+[0-9]{2}:[0-9]{2}:[0-9]{2}) = For the second field (Timestamp); match any base-10 digit, 4 times, 2 times and 2 times, separated by dots (this is the date). match any base-10 digit, 2 times, 2 times and 2 times, separated by semicolons (this is the time).
// ([0-9]+) = For the third field (UserID); match any base-10 digit, one or more times.
// ([^\s;]*) = For the fourth field (Reserved); match any character that is not whitespace nor a semicolon, zero or more times (this field could later be ignored).
// (\S*) = For the fifth field (Message); match any non-whitespace character, one or more times (max length is checked later).
const RegexAssembly LogTable::c_CSVRegex("^\\s*([0-9]+)\\s*;\\s*([0-9]{4}\\.[0-9]{2}\\.[0-9]{2}\\s+[0-9]{2}:[0-9]{2}:[0-9]{2})\\s*;\\s*([0-9]+)\\s*;\\s*([^\\s;]*)\\s*;\\s*(\\S+)\\s*$");

std::size_t LogTable::Count(void) const { return m_Content.size(); }
bool LogTable::Empty(void) const { return m_Content.empty(); }
void LogTable::Add(const LogEntry& item) { m_Content.push_back(item); }
void LogTable::Remove(std::size_t index) { m_Content.erase(m_Content.begin() + index); }
void LogTable::Clear(void) { m_Content.clear(); }

LogEntry LogTable::operator [](const int id) const
{
    for(std::size_t i = 0; i < m_Content.size(); i++)
    {
        if(m_Content[i].GetID() == id)
        {
            return m_Content[i];
        }
    }

    throw;
}

LogEntry* LogTable::FindByID(const int id)
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

bool LogTable::ParseCSV(const std::string& value, LogEntry& result)
{
    std::smatch match;
    if(!c_CSVRegex.Match(value, match))
    {
        return false;
    }

    std::time_t time = strtotime(((std::string)match[2]).c_str());
    if(time == (std::time_t) - 1)
    {
        return false;
    }

    result = LogEntry(std::stoi(match[1]), time, std::stoi(match[3]), match[5]);

    return true;
}

bool LogTable::Load(const std::string& filePath, LogTable& result)
{
    std::fstream stream(filePath, std::ifstream::in);
    if(!stream.is_open())
    {
        return false;
    }

    std::string line;
    while(std::getline(stream, line))
    {
        if(line.empty())
        {
            continue;
        }

        LogEntry entry;
        if(!LogTable::ParseCSV(line, entry))
        {
            stream.close();
            return false;
        }

        result.Add(entry);
    }

    if(stream.bad())
    {
        stream.close();
        return false;
    }

    stream.close();
    return true;
}

// Constructors
LogTable::LogTable(void) { }
