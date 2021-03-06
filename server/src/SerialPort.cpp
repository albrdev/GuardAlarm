#include "SerialPort.hpp"
#include <string>

constexpr char SerialPort::PORTNAME_PREFIX[4 + 1];

bool SerialPort::IsConnected()
{
    return m_IsConnected;
}

bool SerialPort::Open(const char *const portName)
{
    if(m_IsConnected)
    {
        Close();
    }

    std::string tmpPortName(SerialPort::PORTNAME_PREFIX);
    tmpPortName.append(portName);
    m_Handle = CreateFileA(static_cast<LPCSTR>(tmpPortName.c_str()), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(m_Handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DCB dcb;
    memset(&dcb, 0, sizeof(dcb));
    if(!GetCommState(m_Handle, &dcb))
    {
        return false;
    }

    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;

    if(!SetCommState(m_Handle, &dcb))
    {
        return false;
    }

    PurgeComm(m_Handle, PURGE_RXCLEAR | PURGE_TXCLEAR);
    m_IsConnected = true;
    return true;
}

void SerialPort::Close(void)
{
    if(m_IsConnected)
    {
        CloseHandle(m_Handle);
        m_IsConnected = false;
    }
}

bool SerialPort::Available(void)
{
    return AvailableBytes() > 0UL;
}

long unsigned int SerialPort::AvailableBytes(void)
{
    ClearCommError(m_Handle, &m_Error, &m_ComStat);
    return m_ComStat.cbInQue;
}

int SerialPort::Read(char *const buffer, const long unsigned int buf_size)
{
    DWORD bytesRead;
    unsigned int toRead;

    SetLastError(ERROR_SUCCESS);
    ClearCommError(m_Handle, &m_Error, &m_ComStat);

    if(m_ComStat.cbInQue <= 0)
        return 0;

    toRead = m_ComStat.cbInQue > buf_size ? buf_size : m_ComStat.cbInQue;
    if(!ReadFile(m_Handle, buffer, toRead, &bytesRead, NULL))
    {
        int error = GetLastError();
        m_IsConnected = error == ERROR_SUCCESS || error == ERROR_IO_PENDING;

        return -1;
    }

    return bytesRead;
}

bool SerialPort::Write(const char *const buffer, const long unsigned int size)
{
    DWORD bytesSent;

    if(!WriteFile(m_Handle, (void*)buffer, size, &bytesSent, 0))
    {
        ClearCommError(m_Handle, &m_Error, &m_ComStat);
        return false;
    }

    return true;
}

SerialPort::SerialPort(const char *const portName)
{
    if(!Open(portName))
    {
        fprintf(stderr, "Error: %ld\n", GetLastError());
        throw;
    }
}

SerialPort::SerialPort(void) { }

SerialPort::~SerialPort(void)
{
    Close();
}
