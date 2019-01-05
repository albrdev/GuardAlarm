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
#include "shared.h"

char portName[] = "COM10";
unsigned int POLL = 100;

char data[DATA_MAXSIZE] = { 0 };
SerialPort serialPort;

const std::string sensorsFilePath = "sensors.dat";
const std::string usersFilePath = "users.dat";
const std::string logFilePath = "system.log";

UserTable users;
LogTable logs;
SensorTable sensors;
Logger logger;

unsigned int loginAttempts = 0;

bool handleData(const struct _packet_header *const pkt, size_t size)
{
    uint16_t checksum = mkcrc16((uint8_t *)pkt + sizeof(pkt->checksum), size - sizeof(pkt->checksum));

    fprintf(stderr, "recv(size=%zu, crc=%04hX)", size, checksum);
    fprintf(stderr, ", header(chk=%04hX, type=%d)\n", pkt->checksum, pkt->type);
    if(!packet_verify(pkt, size, pkt->checksum))
    {
        fprintf(stderr, "*** Error: Checksum mismatch: %04hX, %04hX\n", pkt->checksum, checksum);
        return false;
    }

    packet_sensorstatus_t *pss;
    packet_pincode_t *ppc;
    SensorEntry *sensorEntry;
    switch(pkt->type)
    {
        case PT_SENSORSTATUS:
            pss = (packet_sensorstatus_t *)data;
            fprintf(stderr, "data(id=%d, status=%hhd)\n", pss->id, pss->status);

            sensorEntry = sensors.FindByID(pss->id);
            if(sensorEntry != nullptr)
                sensorEntry->SetStatus((SensorStatus)pss->status);
            else
                sensors.Add(SensorEntry(pss->id, (SensorStatus)pss->status));

            SensorTable::Save(sensorsFilePath, sensors);
            break;

        case PT_PINCODE:
            ppc = (packet_pincode_t *)data;
            fprintf(stderr, "data(pin=%s, mode=%c (%hhu))\n", ppc->pin, ppc->mode, ppc->mode);

            std::string providedPassword((const char *)ppc->pin);
            UserEntry* userEntry = NULL;
            int statusFlags = userLogin(providedPassword, users, userEntry);
            if((statusFlags & AuthStatus::AS_SUCCESS) != 0)
            {
                loginAttempts = 0; // Successfull login, clear login attempts
                //isAlarmed = false; // Turn off alarm

                if((statusFlags & AuthStatus::AS_EMERGENCY) != 0)
                {
                    fprintf(stderr, "*** Emergency code entered: id=%d, name=%s, tag=%d, status=%d\n", userEntry->GetID(), userEntry->GetUsername().c_str(), userEntry->GetTagID(), userEntry->GetStatus());
                    logger.WriteCSV(LogEntry(time(NULL), userEntry->GetID(), "Emergency code entered"));
                    //emergencyResponse();
                }
                else
                {
                    fprintf(stderr, "*** Successfull login: id=%d, name=%s, tag=%d, status=%d\n", userEntry->GetID(), userEntry->GetUsername().c_str(), userEntry->GetTagID(), userEntry->GetStatus());
                }

                packet_header_t pp;
                packet_mkheader(&pp, sizeof(pp), PT_SUCCESS);
                serialPort.Write((const char *)&pp, sizeof(pp));

                //userSession(userEntry, isAlarmed, logger);
            }
            else
            {
                fprintf(stderr, "*** Failed login attempt\n");
                logger.WriteCSV(LogEntry(time(NULL), -1, "Failed login attempt"));

                packet_header_t pp;
                packet_mkheader(&pp, sizeof(pp), PT_FAILURE);
                serialPort.Write((const char *)&pp, sizeof(pp));

                if(++loginAttempts >= LOGINATTEMPTS_MAX)
                {
                    fprintf(stderr, "*** Alarm triggered due to number of login attempts exceeded\n");
                    if(userEntry != NULL) // Could be 'NULL' here (If login failed because we didn't find a user with that name)
                    {
                        userEntry->SetStatus(UserStatus::US_BLOCKED); // Deactivate the account
                    }

                    //return -1; // Program exits and could (in the future) save timestamp somewhere safe, to use and refuse to start again under a certain time (to protect from further password-cracking attempts)
                }
            }
            break;
    }

    fputc('\n', stderr);
    return true;
}

int main(void)
{
    // Remove buffered output for 'stderr'
    if(setvbuf(stderr, NULL, _IONBF, 0) != 0)
    {
        fprintf(stderr, "Could not set \'stderr\' unbuffered\n");
        fflush(stderr);
        return 1;
    }

    if(!SensorTable::Load(sensorsFilePath, sensors))
    {
        fprintf(stderr, "*** Error reading sensors: %s:%zu\n", sensorsFilePath.c_str(), sensors.Count() + 1);
        return 5;
    }

    printf("Sensors successfully loaded %zu entries\n", sensors.Count());
    //printTable(sensors);

    if(!UserTable::Load(usersFilePath, users))
    {
        fprintf(stderr, "*** Error reading users: %s:%zu\n", usersFilePath.c_str(), users.Count() + 1);
        return 3;
    }

    printf("Users successfully loaded %zu entries\n", users.Count());
    //printTable(users);

    fflush(stdout);

    while(true)
    {
        while(!serialPort.Open(portName))
        {
            fprintf(stderr, "*** Error: Connection failed on serial port \'%s\'\n", portName);
            Sleep(1000);
        }

        fprintf(stderr, "*** Connection successfully established on serial port \'%s\'\n\n", portName);
        Sleep(1000);

        while(serialPort.IsConnected())
        {
            int size = serialPort.Read(data, sizeof(data));
            if(size < 0)
            {
                fprintf(stderr, "Error: Failed to read serial data\n");
            }
            else if((size_t)size >= sizeof(packet_header_t))
            {
                handleData((packet_header_t *)data, (size_t)size);
            }
            else if(size != 0)
            {
                fprintf(stderr, "Error: Invalid packet size: %zu\n", (size_t)size);
            }

            Sleep(POLL);
        }

        fprintf(stderr, "*** Error: Lost serial connection on port \'%s\'\n", portName);
    }

    return 0;
}
