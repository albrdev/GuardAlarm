#ifndef _PIN_HPP_
#define _PIN_HPP_

#include <string.h>

class PIN
{
public:
    static const size_t MINLENGTH = 4U;
    static const size_t MAXLENGTH = 6U;

private:
    char m_Content[PIN::MAXLENGTH + 1U] = { '\0' };
    size_t m_Length = 0U;
    char m_Mode = '\0';

public:
    const char *const GetContent(void) const;
    void SetContent(const char *const value);

    size_t GetLength(void) const;

    char GetMode(void) const;
    void SetMode(const char value);

    bool IsValid(void) const;

    void Append(const char chr);
    void Clear(void);

    PIN(const char *const pin, const char option);
    PIN(const char *const pin);
    PIN(void);
};

#endif // _PIN_HPP_
