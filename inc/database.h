#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <cstdlib>
#include <fstream>
#include <vector>
#include "generic.h"
#include "regexassembly.h"
#include "credentials.h"

class Database
{
protected:
    static const RegexAssembly c_CSVRegex;

    typedef std::vector<Credentials> container_t;
    container_t m_Content; // Underlying content

    static bool ParseCSV(const std::string& value, Credentials& result);

public:
    std::size_t Count(void) const;
    bool Empty(void) const;
    void Add(const Credentials& item);
    void Remove(std::size_t index);
    void Clear(void);

    Credentials operator [](const int id) const;

    Credentials* FindByID(const int id);
    Credentials* FindByUsername(const std::string& username);

    static bool Load(const std::string& filePath, Database& result);

    // Constructors
    Database(void);
};

#endif // _DATABASE_H_
