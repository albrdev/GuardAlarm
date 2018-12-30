#ifndef _SERIALPORT_HPP_
#define _SERIALPORT_HPP_

#define ARDUINO_WAIT_TIME 1000
#define MAX_DATA_LENGTH 256

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class SerialPort
{
    private:
        HANDLE m_Handle;
        bool m_IsConnected;
        COMSTAT m_ComStat;
        DWORD m_Error;

    public:
        bool IsConnected(void);
        bool Open(const char *const portName);
        void Close(void);

        int Read(char *buffer, unsigned int buf_size);
        bool Write(const char *buffer, unsigned int buf_size);

        SerialPort(const char *const portName);
        ~SerialPort(void);
};

#endif // _SERIALPORT_HPP_
