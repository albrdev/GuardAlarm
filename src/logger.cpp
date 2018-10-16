#include "logger.h"

std::string Logger::GetFilePath(void) const { return m_FilePath; }
std::ios_base::openmode Logger::GetMode(void) const { return m_Mode; }

bool Logger::IsOpen(void) const { return m_FileStream.is_open(); }

bool Logger::Open(const std::string& filePath, std::ios_base::openmode mode)
{
    Close();
    m_FileStream.open(filePath, std::ios_base::out | mode);
    m_FilePath = filePath;
    m_Mode = mode;

    return m_FileStream.is_open();
}

void Logger::Close(void)
{
    if(m_FileStream.is_open())
    {
        m_FileStream.close();
    }
}

bool Logger::WriteCSV(const LogEntry &value)
{
    m_FileStream << value.ToCSVString() << std::endl;
    return !m_FileStream.fail();
}

// Constructors
Logger::Logger(const std::string& filePath, std::ios_base::openmode mode)
{
    Open(filePath, mode);
}

Logger::Logger(void) { }

Logger::~Logger(void)
{
    Close();
}
