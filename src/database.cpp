#include "database.h"


size_t Database::Count(void) const { return m_Content.size(); }
bool Database::Empty(void) const { return m_Content.empty(); }
void Database::Add(const Credentials& item) { m_Content.push_back(item); }
void Database::Remove(size_t index) { m_Content.erase(m_Content.begin() + index); }
void Database::Clear(void) { m_Content.clear(); }

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

// Constructors
Database::Database(void) { }
