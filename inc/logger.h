#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <fstream>
#include "logentry.h"

class Logger
{
protected:
    std::ofstream m_FileStream;
    std::string m_FilePath;
    std::ios_base::openmode m_Mode;

public:
    std::string GetFilePath(void) const;
    std::ios_base::openmode GetMode(void) const;

    bool IsOpen(void) const;
    bool Open(const std::string& filePath, std::ios_base::openmode mode = std::ios_base::app);
    void Close(void);

    bool WriteCSV(const LogEntry &value);

    // Constructors
    Logger(const std::string& filePath, std::ios_base::openmode mode = std::ios_base::app);
    Logger(void);
    ~Logger(void);
};

#endif // _LOGGER_H_
