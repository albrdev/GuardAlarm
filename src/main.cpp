#include <cstdio>
#include "credentials.h"
#include "database.h"
#include "misc.h"

/*
    I'd prefer printf over std::cout mostly because it's much more readable, especially mixed with variables and not just constant strings
    I be consistent I would have changed the input too to some C function, like scanf, but I didn't have time, so I keep std::cin
*/

int main(void)
{
    const int LOGINATTEMPTS_MAX = 3; // Uppercase for constants, right?
    const std::string databaseFilePath = "users.dat";

    Database database;
    bool isAlarmed = true;

    // Remove buffered output for 'stderr'
    if(setvbuf(stderr, NULL, _IONBF, 0) != 0)
    {
        fprintf(stderr, "Could not set \'stderr\' unbuffered\n");
        return 1;
    }

    if(!Database::Load(databaseFilePath, database))
    {
        fprintf(stderr, "*** Error reading database: %s:%llu\n", databaseFilePath.c_str(), database.Count() + 1);
        return 2;
    }

    printf("Database successfully loaded %llu entries\n", database.Count());
    /*for(std::size_t i = 0; i < database.Count(); i++)
    {
        std::cout << "  " << database[i] << std::endl;
    }
    std::cout << std::endl;*/

    int attemptedLogins = 0;
    while(true)
    {
        Credentials providedCredentials;
        Credentials* credentialsEntry = NULL;

        // Print status, input credentials
        printf("Alarm is %s!\n", statusString(isAlarmed).c_str());
        inputCredentials(providedCredentials);

        // Find the entry in the database that matches the username, test the password and return a status bitflags of how well the authentication went
        int statusFlags = userLogin(providedCredentials, database, credentialsEntry);
        if((statusFlags & AS_SUCCESS) != 0) // Check if AS_SUCCESS bit is set with AND
        {
            attemptedLogins = 0; // Successfull login, clear login attempts
            isAlarmed = !isAlarmed; // Toggle alarm

            if((statusFlags & AS_EMERGENCY) != 0) // Check if AS_EMERGENCY bit is also set
            {
                // Call emergency function (this function should not print or alert somehow, just completly quiet, but not in this very test case)
                emergencyResponse();
                // Continue as usual with the menu (to not startle the possible attacker(s))
            }

            userSession(credentialsEntry, isAlarmed);
        }
        else
        {
            // Login failed, count a maximum of LOGINATTEMPTS_MAX times in a row and then block and deactivate user
            fprintf(stderr, "*** Error: Incorrect username or password\n");
            if(++attemptedLogins >= LOGINATTEMPTS_MAX)
            {
                fprintf(stderr, "*** Warning: You have been locked out\n");
                if(credentialsEntry != NULL) // Could be 'NULL' here (If login failed because we didn't find a user with that name)
                {
                    credentialsEntry->SetStatus(UserStatus::Blocked); // Deactivate the account
                }

                return -1; // Program exits and could (in the future) save timestamp somewhere safe, to use and refuse to start again under a certain time (to protect from further password-cracking attempts)
            }
        }
    }

    return 0;
}
