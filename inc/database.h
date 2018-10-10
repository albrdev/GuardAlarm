#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <cstdlib>
#include <fstream>
#include <vector>
#include "generic.h"
#include "credentials.h"

class Database
{
protected:
    typedef std::vector<Credentials> container_t;
    container_t m_Content; // Underlying conten

public:
    std::size_t Count(void) const;
    bool Empty(void) const;
    void Add(const Credentials& item);
    void Remove(size_t index);
    void Clear(void);

    Credentials operator [](const std::size_t index) const;
    Credentials operator [](const std::string& username) const;

    Credentials* FindByID(const int id);
    Credentials* FindByUsername(const std::string& username);

    static bool Load(const std::string& filePath, Database& result);

    // Constructors
    Database(void);
};

#endif // _DATABASE_H_
