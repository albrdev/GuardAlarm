#ifndef _MISC_H_
#define _MISC_H_

#include <cstdio>
#include <ctime>
#include <string>
#include "UserEntry.h"
#include "UserTable.h"
#include "Logger.h"

/*
    AuthStatus: An enum for representing login status
*/
enum AuthStatus
{
    Fail = 0,       // 00000000
    Success = 1,    // 00000001
    Emergency = 2   // 00000010
    // Possibly more enum constants here must have the value of double the last one, because they're bitflags
};

/*
    MenuOption: These are the menu options when logged in
                They have the value of the ASCII characters integer value
*/
enum MenuOption
{
    None = 0,
    Exit = 'q',
    Logout = 'x',
    SetAlarm = 'a',
    ChangePassword = 'c'
};

int checkPassword(const std::string& providedPIN, const std::string& storedPIN);
int userLogin(const std::string& providedPassword, UserTable& usertable, UserEntry*& result);

void emergencyResponse(void);

void inputPassword(std::string& password);
bool changePassword(std::string& pin);

MenuOption configMenu(void);
void userSession(UserEntry* userentry, bool& isAlarmed, Logger& logger);

std::string statusString(bool isOn);

#endif // _MISC_H_
