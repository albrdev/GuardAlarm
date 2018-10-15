#include "database.h"

// Regex explanation:
// ^\s*([0-9]+)\s*;\s*([0-9]{4,6})\s*;\s*([a-zA-Z]+)\s*;\s*([0-9]*)\s*;\s*([1-3])\s*;\s*(\S*)\s*$
//
// ^ = Match the start of the string.
// $ = Match the end of the string
// \s* = Match any number of whitespace between value fields (allowing for possible (and unnecessary) spaces in between).
// ; = Match the literal comma that separates the fields.

// ([0-9]+) = For the first field (ID); match any base-10 digit, one or more times.
// ([0-9]{4,6}) = For the second field (Password); match any base-10 digit, 4-6 times.
// ([a-zA-Z]+) = For the third field (Username); match any letter from a-z, upper- and lowercase, one or more times.
// ([0-9]*) = For the fourth field (TagID); match any base-10 digit, zero or more times (this field is optional).
// ([1-3]) = For the fifth field (Status); match any number from 1-3, one single time
// (\S*) = For the sixth field (Reserved); match any non-whitespace character, zero or more times (this field could later be ignored).
const RegexAssembly Database::c_CSVRegex("^\\s*([0-9]+)\\s*;\\s*([0-9]{4,6})\\s*;\\s*([a-zA-Z]+)\\s*;\\s*([0-9]*)\\s*;\\s*([1-3])\\s*;\\s*(\\S*)\\s*$");

bool Database::FindByID(const int id, Credentials& result)
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

Credentials* Database::FindByID(const int id)
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

/*
    Database::FindByUsername: Find entry that matches 'username'
                              Return pointer to the matched entry in the database, or 'NULL' if no entry could be found
*/
bool Database::FindByUsername(const std::string& username, Credentials& result)
{
    for(size_t i = 0; i < m_Content.size(); i++)
    {
        if(strcmpic(m_Content[i].GetUsername(), username) == 0)
        {
            result = m_Content[i];
            return true;
        }
    }

    return false;
}

Credentials* Database::FindByUsername(const std::string& username)
{
    for(size_t i = 0; i < m_Content.size(); i++)
    {
        if(strcmpic(m_Content[i].GetUsername(), username) == 0)
        {
            return &m_Content[i];
        }
    }

    return NULL;
}

bool Database::ParseCSV(const std::string& value, Credentials& result)
{
    std::smatch match;
    if(!c_CSVRegex.Match(value, match))
    {
        return false;
    }

    result = Credentials(std::stoi(match[1]), match[2], match[3], (UserStatus)std::stoi(match[5]));
    if(!((std::string)match[4]).empty())
    {
        result.SetTagID(std::stoi(match[4]));
    }

    return true;
}

bool Database::Load(const std::string& filePath, Database& result)
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

        Credentials entry;
        if(!Database::ParseCSV(line, entry))
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
Database::Database(void) { }
