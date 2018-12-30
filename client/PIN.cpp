#include "PIN.hpp"

const char *const PIN::GetContent(void) const
{
    return m_Content;
}

void PIN::SetContent(const char *const value)
{
    strncpy(m_Content, value, PIN::MAXLENGTH);
}

size_t PIN::GetLength(void) const
{
    return m_Length;
}

char PIN::GetOption(void) const
{
    return m_Option;
}

void PIN::SetOption(const char value)
{
    m_Option = value;
}

bool PIN::IsValid(void) const
{
    return m_Length >= PIN::MINLENGTH && m_Length <= PIN::MAXLENGTH;
}

void PIN::Append(const char chr)
{
    if(m_Length < PIN::MAXLENGTH)
    {
        m_Content[m_Length] = chr;
        m_Length++;
    }
    else
    {
        for(size_t i = 1U; i < m_Length; i++)
        {
            m_Content[i - 1U] = m_Content[i];
        }

        m_Content[m_Length - 1U] = chr;
    }
}

void PIN::Clear(void)
{
    memset(m_Content, '\0', sizeof(m_Content));
    m_Length = 0U;
    m_Option = '\0';
}

PIN::PIN(const char *const pin, const char option)
{
    SetContent(pin);
    SetOption(option);
}

PIN::PIN(const char *const pin)
{
    SetContent(pin);
}

PIN::PIN(void) { }
