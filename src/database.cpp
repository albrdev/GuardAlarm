#include "database.h"


std::size_t Database::Count(void) const { return m_Content.size(); }
bool Database::Empty(void) const { return m_Content.empty(); }
void Database::Add(const Credentials& item) { m_Content.push_back(item); }
void Database::Remove(size_t index) { m_Content.erase(m_Content.begin() + index); }
void Database::Clear(void) { m_Content.clear(); }

Credentials Database::operator [](const std::size_t index) const
{
    return m_Content[index];
}

Credentials Database::operator [](const std::string& username) const
{
    for(std::size_t i = 0; i < m_Content.size(); i++)
    {
        if(strcmpic(m_Content[i].GetUsername(), username) == 0)
        {
            return m_Content[i];
        }
    }

    throw;
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

bool Database::Load(const std::string& filePath, Database& result)
{
    std::fstream stream(filePath, std::ifstream::in);
    if(!stream.is_open())
    {
        return false;
    }

    std::string line;
    std::regex regex("([0-9]+);([0-9]{4,6});([a-zA-Z]+);([0-9]*);([1-3]);(.*)");
    while(std::getline(stream, line))
    {
        if(line.empty())
        {
            continue;
        }

        std::smatch match;
        if(!std::regex_match(line, match, regex))
        {
            stream.close();
            return false;
        }

        Credentials credentials;
        credentials.SetID(std::stoi(match[1]));

        if(!((std::string)match[4]).empty())
            credentials.SetTagID(std::stoi(match[4]));

        credentials.SetUsername(match[3]);
        credentials.SetPassword(match[2]);
        credentials.SetStatus((UserStatus)std::stoi(match[5]));
        result.Add(credentials);
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
