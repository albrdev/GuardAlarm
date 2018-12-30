#include <cstdio>
#include <cstdlib>
#include <string>
#include <ctime>
#include "SerialPort.hpp"
#include "UserEntry.hpp"
#include "UserTable.hpp"
#include "LogEntry.hpp"
#include "LogTable.hpp"
#include "SensorEntry.hpp"
#include "SensorTable.hpp"
#include "Logger.hpp"
#include "crc.h"
#include "packet.h"
#include "misc.hpp"

char portID[] = "\\\\.\\COM10";
char *portName = portID;
unsigned int POLL = 100;

char data[MAX_DATA_LENGTH] = { 0 };
SerialPort serialPort(portName);

int main(void)
{
    printf("Connection %s\n", serialPort.IsConnected() ? "successfull" : "failed");
    fflush(stdout);

    //while(!serialPort.isConnected());

    //int size = serialPort.readSerialPort(data, MAX_DATA_LENGTH);
    while(serialPort.IsConnected())
    {
        int size = serialPort.Read(data, sizeof(data));
        if(size < (int)sizeof(packet_header_t))
        {
            if(size != 0)
            {
                fprintf(stderr, "Error: Invalid packet size\n");
                fflush(stderr);
            }

            Sleep(POLL);
            continue;
        }

        packet_header_t *pkt = (packet_header_t *)data;
        uint16_t crc = mkcrc16((uint8_t *)pkt + sizeof(pkt->checksum), size - sizeof(pkt->checksum));

        printf("recv(size=%d, crc=%04hX)", size, crc);
        printf(", header(chk=%04hX, type=%d)", pkt->checksum, pkt->type);
        if(!packet_verify(pkt, size, pkt->checksum))
        {
            Sleep(POLL);
            continue;
        }

        packet_pincode_t *dta;
        switch(pkt->type)
        {
            case PT_PIN:
                dta = (packet_pincode_t *)data;
                printf(", data(pin=%s, mode=%c (%hhu))\n", dta->pin, dta->mode, dta->mode);
                break;
        }

        putchar('\n');
        fflush(stdout);

        Sleep(POLL);
    }

    return 0;
}
