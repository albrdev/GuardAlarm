#ifndef _MISC_H_
#define _MISC_H_

#include <cstdio>
#include <ctime>
#include <string>
#include "credentials.h"
#include "database.h"

/*
    AuthStatus: An enum for representing login status
*/
enum AuthStatus
{
    AS_FAILURE = 0,     // 00000000
    AS_SUCCESS = 1,     // 00000001
    AS_EMERGENCY = 2    // 00000010
    // Possibly more enum constants here must have the value of double the last one, because they're bitflags
};

/*
    MenuOption: These are the menu options when logged in
                They have the value of the ASCII characters integer value
*/
enum MenuOption
{
    MO_EXIT = 'q',
    MO_LOGOUT = 'x',
    MO_SETALARM = 'a',
    MO_CHANGEPASSWORD = 'c'
};

int checkPassword(const std::string& providedPIN, const std::string& storedPIN);
int userLogin(const Credentials& providedCredentials, Database& database, Credentials*& result);

void emergencyResponse(void);

void inputCredentials(Credentials& credentials);
bool changePassword(std::string& pin);

bool configMenu(Credentials* const credentials, bool& isAlarmed);
void userSession(Credentials* credentials, bool& isAlarmed);

std::string statusString(bool isOn);

#endif // _MISC_H_
