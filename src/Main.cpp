#include <cstdio>
#include "UserEntry.h"
#include "UserTable.h"
#include "LogEntry.h"
#include "LogTable.h"
#include "SensorEntry.h"
#include "SensorTable.h"
#include "Logger.h"
#include "misc.h"

/*
    I'd prefer printf over std::cout mostly because it's much more readable, especially mixed with variables and not just constant strings
    I be consistent I would have changed the input too to some C function, like scanf, but I didn't have time, so I keep std::cin
*/

template<typename T> void printTable(const Table<T>& table)
{
    for(std::size_t i = 0; i < table.Count(); i++)
    {
        std::cout << "  " << table[i].ToString() << std::endl;
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    const int LOGINATTEMPTS_MAX = 3;
    const std::string usersFilePath = "users.dat";
    const std::string logFilePath = "system.log";
    const std::string sensorsFilePath = "sensors.dat";

    UserTable users;
    LogTable logs;
    SensorTable sensors;
    Logger logger;
    bool isAlarmed = true;

    // Remove buffered output for 'stderr'
    if(setvbuf(stderr, NULL, _IONBF, 0) != 0)
    {
        fprintf(stderr, "Could not set \'stderr\' unbuffered\n");
        return 1;
    }

    if(!logger.Open(logFilePath))
    {
        fprintf(stderr, "*** Error opening logfile: %s\n", logger.GetFilePath().c_str());
        return 2;
    }

    if(!SensorTable::Load(sensorsFilePath, sensors))
    {
        fprintf(stderr, "*** Error reading sensors: %s:%llu\n", sensorsFilePath.c_str(), sensors.Count() + 1);
        return 5;
    }

    printf("Sensors successfully loaded %llu entries\n", sensors.Count());
    printTable(sensors);

    if(!UserTable::Load(usersFilePath, users))
    {
        fprintf(stderr, "*** Error reading users: %s:%llu\n", usersFilePath.c_str(), users.Count() + 1);
        return 3;
    }

    printf("Users successfully loaded %llu entries\n", users.Count());
    //printTable(users);

    if(!LogTable::Load(logFilePath, logs))
    {
        fprintf(stderr, "*** Error reading logs: %s:%llu\n", logFilePath.c_str(), logs.Count() + 1);
        return 4;
    }

    printf("Logs successfully loaded %llu entries\n", logs.Count());
    //printTable(logs);

    int attemptedLogins = 0;
    while(true)
    {
        std::string providedPassword;
        UserEntry* userEntry = NULL;

        // Print status
        printf("Alarm is %s!\n", statusString(isAlarmed).c_str());
        // Input password
        inputPassword(providedPassword);

        // Find the entry in the table that matches the username, test the password and return a status bitflags of how well the authentication went
        int statusFlags = userLogin(providedPassword, users, userEntry);
        if((statusFlags & AuthStatus::Success) != 0) // Check if AS_SUCCESS bit is set with AND
        {
            attemptedLogins = 0; // Successfull login, clear login attempts
            isAlarmed = false; // Turn off alarm

            if((statusFlags & AuthStatus::Emergency) != 0) // Check if AS_EMERGENCY bit is also set
            {
                // Call emergency function (this function should not print or alert somehow, just completly quiet, but not in this very test case)
                logger.WriteCSV(LogEntry(time(NULL), userEntry->GetID(), "Emergency code entered: Notifying the authorities!"));
                emergencyResponse();
                // Continue as usual with the menu (to not startle the possible attacker(s))
            }

            userSession(userEntry, isAlarmed, logger);
        }
        else
        {
            // Login failed, count a maximum of LOGINATTEMPTS_MAX times in a row and then block and deactivate user
            logger.WriteCSV(LogEntry(time(NULL), -1, "Failed login attempt"));
            fprintf(stderr, "*** Error: Failed login attempt\n");
            if(++attemptedLogins >= LOGINATTEMPTS_MAX)
            {
                fprintf(stderr, "*** Warning: You have been locked out\n");
                if(userEntry != NULL) // Could be 'NULL' here (If login failed because we didn't find a user with that name)
                {
                    userEntry->SetStatus(UserStatus::Blocked); // Deactivate the account
                }

                return -1; // Program exits and could (in the future) save timestamp somewhere safe, to use and refuse to start again under a certain time (to protect from further password-cracking attempts)
            }
        }
    }

    return 0;
}
