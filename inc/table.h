#ifndef _TABLE_H_
#define _TABLE_H_

#include <cstdlib>
#include <fstream>
#include <vector>
#include "generic.h"
#include "regexassembly.h"
#include "entry.h"

template <typename T>
class Table
{
protected:
    std::vector<T> m_Content; // Underlying content

public:
    std::size_t Count(void) const { return m_Content.size(); }
    bool Empty(void) const { return m_Content.empty(); }
    void Add(const T& item) { m_Content.push_back(item); }
    void Clear(void) { m_Content.clear(); }
    void Remove(const std::size_t index) { m_Content.erase(m_Content.begin() + index); }

    bool Remove(const T& item)
    {
        for(size_t i = 0; i < m_Content.size(); i++)
        {
            if(m_Content[i] == item)
            {
                m_Content.erase(m_Content.begin() + i);
                return true;
            }
        }

        return false;
    }

    T operator [](const std::size_t index) const
    {
        return m_Content[index];
    }

    // Constructors
    Table(void) { }
};

#endif // _TABLE_H_
