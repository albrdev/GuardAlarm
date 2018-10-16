#include "database.h"

// Regex explanation:
// ^\s*([0-9]+)\s*;\s*([0-9]{4,6})\s*;\s*([a-zA-Z]+)\s*;\s*([0-9]*)\s*;\s*([1-3])\s*;\s*(\S*)\s*$
//
// ^ = Match the start of the string.
// $ = Match the end of the string.
// \s* = Match any number of whitespace between value fields (allowing for possible (and unnecessary) spaces in between).
// ; = Match the literal comma that separates the fields.

// ([0-9]+) = For the first field (ID); match any base-10 digit, one or more times.
// ([0-9]{4,6}) = For the second field (Password); match any base-10 digit, 4-6 times.
// ([a-zA-Z]+) = For the third field (Username); match any letter from a-z, upper- and lowercase, one or more times.
// ([0-9]*) = For the fourth field (TagID); match any base-10 digit, zero or more times (this field is optional).
// ([1-3]) = For the fifth field (Status); match 1, 2 or 3, one single time.
// ([^\\s;]*) = For the sixth field (Reserved); match any non-whitespace character, zero or more times (this field could later be ignored).
const RegexAssembly Database::c_CSVRegex("^\\s*([0-9]+)\\s*;\\s*([0-9]{4,6})\\s*;\\s*([a-zA-Z]+)\\s*;\\s*([0-9]*)\\s*;\\s*([1-3])\\s*;\\s*([^\\s;]*)\\s*$");

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

Credentials* Database::FindByPassword(const std::string& password)
{
    for(size_t i = 0; i < m_Content.size(); i++)
    {
        if(strcmpic(m_Content[i].GetPassword(), password) == 0)
        {
            return &m_Content[i];
        }
    }

    return NULL;
}

/*
    FindBySecondaryPassword: Finds an entry by comparing against secondary (emergency) password
*/
Credentials* Database::FindBySecondaryPassword(const std::string& password)
{
    for(size_t i = 0; i < m_Content.size(); i++)
    {
        if(strcmpic(m_Content[i].GetSecondaryPassword(), password) == 0)
        {
            return &m_Content[i];
        }
    }

    return NULL;
}

/*
    ParseCSV: Split a string line into fields and convert them into correct type and put them in a Credentials object
*/
bool Database::ParseCSV(const std::string& value, Credentials& result)
{
    std::smatch match;
    if(!c_CSVRegex.Match(value, match)) // If regex fails, there was an error on some value field, every field in the line must succeed.
    {
        return false;
    }

    // Index 0 contains the entire line, so we begin at 1, these are the fields read in order accoring to the regex.
    result = Credentials(std::stoi(match[1]), match[2], match[3], (UserStatus)std::stoi(match[5]));
    if(!((std::string)match[4]).empty())
    {
        result.SetTagID(std::stoi(match[4]));
    }

    return true;
}

/*
    GetNextID:  Attempts to read all the entries in a file, skipping empty lines and handles spaces between value fields.
                If there's a paring error of a field, the line is discarded and function returns 'false'.
*/
bool Database::Load(const std::string& filePath, Database& result)
{
    std::fstream stream(filePath, std::ifstream::in);
    if(!stream.is_open())
    {
        return false;
    }

    int highestID = std::numeric_limits<int>().min(); // Let's find the highest ID while we are iterating through the entries
    std::string line;
    while(std::getline(stream, line))
    {
        if(line.empty()) // We don't want to attempt to read harmless empty lines, continue the to next line
        {
            continue;
        }

        Credentials entry;
        if(!Database::ParseCSV(line, entry)) // Try to parse the line using regex
        {
            stream.close();
            return false;
        }

        result.Add(entry); // Entry read successfully, save it
        if(entry.GetID() > highestID)
        {
            highestID = entry.GetID(); // This will ensure we'll get the highest ID of all entries
        }
    }

    if(stream.bad()) // There was a error in reading the file (maybe bad sector, harddrive was unplugged or whatever)
    {
        stream.close();
        return false;
    }

    stream.close();
    Credentials::s_NextID = highestID + 1; // Set the static integer that keeps track of the next available ID
    return true;
}

// Constructors
Database::Database(void) { }
