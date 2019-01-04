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

char portID[] = "COM10";
char *portName = portID;
unsigned int POLL = 100;

char data[MAX_DATA_LENGTH] = { 0 };
SerialPort serialPort;

const std::string usersFilePath = "users.dat";
const std::string logFilePath = "system.log";
const std::string sensorsFilePath = "sensors.dat";

UserTable users;
LogTable logs;
SensorTable sensors;
Logger logger;

int main(void)
{
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

    while(true)
    {
        while(!serialPort.Open(portName))
        {
            fprintf(stderr, "*** Error: Connection failed on serial port \'%s\'\n", portName);
            fflush(stderr);
            Sleep(1000);
        }

        fprintf(stderr, "*** Connection successfully established on serial port \'%s\'\n", portName);
        fflush(stderr);
        Sleep(1000);

        while(serialPort.IsConnected())
        {
            int size = serialPort.Read(data, sizeof(data));
            if(size < (int)sizeof(packet_header_t))
            {
                if(size != 0)
                {
                    fprintf(stderr, "Error: Invalid packet size: %d\n", size);
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

            packet_sensorstatus_t *dts;
            packet_pincode_t *dta;
            switch(pkt->type)
            {
                case PT_SENSORSTATUS:
                    dts = (packet_sensorstatus_t *)data;
                    printf(", data(id=%d, status=%hhd)\n", dts->id, (int8_t)dts->status); fflush(stdout);
                    break;
                case PT_PIN:
                    dta = (packet_pincode_t *)data;
                    printf(", data(pin=%s, mode=%c (%hhu))\n", dta->pin, dta->mode, dta->mode);

                    std::string providedPassword((const char *)dta->pin);
                    UserEntry* userEntry = NULL;
                    int statusFlags = userLogin(providedPassword, users, userEntry);
                    if((statusFlags & AuthStatus::Success) != 0) // Check if AS_SUCCESS bit is set with AND
                    {
                        fprintf(stderr, "*** Successfull login: id=%d, name=%s, tag=%d, status=%d\n", userEntry->GetID(), userEntry->GetUsername().c_str(), userEntry->GetTagID(), userEntry->GetStatus());
                        //attemptedLogins = 0; // Successfull login, clear login attempts
                        //isAlarmed = false; // Turn off alarm

                        //attemptedLogins = 0;
                        if((statusFlags & AuthStatus::Emergency) != 0) // Check if AS_EMERGENCY bit is also set
                        {
                            // Call emergency function (this function should not print or alert somehow, just completly quiet, but not in this very test case)
                            logger.WriteCSV(LogEntry(time(NULL), userEntry->GetID(), "Emergency code entered"));
                            //emergencyResponse();
                            // Continue as usual (to not startle the possible attacker(s))
                        }

                        packet_header_t pp;
                        packet_mkheader(&pp, sizeof(pp), PT_SUCCESS);
                        serialPort.Write((const char *)&pp, sizeof(pp));

                        //userSession(userEntry, isAlarmed, logger);
                    }
                    else
                    {
                        packet_header_t pp;
                        packet_mkheader(&pp, sizeof(pp), PT_FAILURE);
                        serialPort.Write((const char *)&pp, sizeof(pp));
                        // Login failed, count a maximum of LOGINATTEMPTS_MAX times in a row and then block and deactivate user
                        logger.WriteCSV(LogEntry(time(NULL), -1, "Failed login attempt"));
                        fprintf(stderr, "*** Error: Failed login attempt\n");
                        /*if(++attemptedLogins >= LOGINATTEMPTS_MAX)
                        {
                            fprintf(stderr, "*** Warning: You have been locked out\n");
                            if(userEntry != NULL) // Could be 'NULL' here (If login failed because we didn't find a user with that name)
                            {
                                userEntry->SetStatus(UserStatus::Blocked); // Deactivate the account
                            }

                            return -1; // Program exits and could (in the future) save timestamp somewhere safe, to use and refuse to start again under a certain time (to protect from further password-cracking attempts)
                        }*/
                    }

                    fflush(stdout);
                    break;
            }

            //Sleep(POLL);
        }

        fprintf(stderr, "*** Error: Lost serial connection on port \'%s\'\n", portName);
    }

    return 0;
}
