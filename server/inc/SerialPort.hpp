#ifndef _SERIALPORT_HPP_
#define _SERIALPORT_HPP_

#define MAX_DATA_LENGTH 256

#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <windows.h>

class SerialPort
{
    private:
        static constexpr char PORTNAME_PREFIX[4 + 1] = "\\\\.\\";

        bool m_IsConnected;
        HANDLE m_Handle;
        COMSTAT m_ComStat;
        DWORD m_Error;

    public:
        bool IsConnected(void);
        bool Open(const char *const portName);
        void Close(void);

        bool Available(void);
        long unsigned int AvailableBytes(void);

        int Read(char *const buffer, const long unsigned int buf_size);
        bool Write(const char *const buffer, const long unsigned int buf_size);

        SerialPort(const char *const portName);
        SerialPort(void);
        ~SerialPort(void);
};

#endif // _SERIALPORT_HPP_
