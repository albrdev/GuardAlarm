#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <cstdlib>
#include <vector>
#include "generic.h"
#include "credentials.h"

class Database
{
protected:
    std::vector<Credentials> m_Content; // Underlying content

public:
    size_t Count(void) const;
    bool Empty(void) const;
    void Add(const Credentials& item);
    void Remove(size_t index);
    void Clear(void);

    Credentials* FindByUsername(const std::string& username);

    // Constructors
    Database(void);
};

#endif // _DATABASE_H_
