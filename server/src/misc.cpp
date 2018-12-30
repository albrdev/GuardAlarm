#include "misc.hpp"

/*
    userLogin: Finds user in the table and checks if password is correct
               Uses a reference to a pointer because I think return value should be the status of the login attempt
*/
int userLogin(const std::string& providedPassword, UserTable& usertable, UserEntry*& result)
{
    // Check emergency password first, if by some reason, someone else has someones emergency password as a regular password, we avoid possible security bug by always prioritize emergency password
    if((result = usertable.FindBySecondaryPassword(providedPassword)) != NULL)
    {
        return AuthStatus::Success | AuthStatus::Emergency;
    }
    else if((result = usertable.FindByPassword(providedPassword)) != NULL) // Now we'll check regular password, when we know nobody had this password as an emergency
    {
        return AuthStatus::Success;
    }

    return AuthStatus::Fail;
}

/*
    emergencyResponse: This function determines what to do when the emergency password has been entered (Now it will just display a message + timestamp)
*/
void emergencyResponse(void)
{
    printf("*** Emergency code entered: Notifying the authorities!\n");
}

/*
    inputPassword: Login input dialog, asks for password and stores it in the string referece parameter
*/
void inputPassword(std::string& password)
{
    bool validationSuccessful = false;
    while(!validationSuccessful)
    {
        // This is a unconventional approach on how to avoid ugly nestled if-statements
        // do { ... } while(false); will loop only once, but we will have the ability to break out of it anytime we want (and try all the input phases again), which will save us from nestling if, else if, else
        do
        {
            std::string input;

            printf("Input password: ");
            readString(input);
            validationSuccessful = UserEntry::ValidatePassword(input);
            if(!validationSuccessful)
            {
                fprintf(stderr, "*** Error: Malformatted password input\n");
                break;
            }

            password = input;
        } while(false);
    }
}

/*
    changePassword: Change password with an input and a second confirmation
               Return true if both strings met the password requirements and are equal
               If successfull, sets out parameter 'password' to the typed password
*/
bool changePassword(std::string& password)
{
    std::string temp1, temp2;

    printf("Type new password: ");
    readString(temp1);
    if(!UserEntry::ValidatePassword(temp1))
    {
        fprintf(stderr, "*** Error: Invalid password format\n");
        return false;
    }

    printf("Retype new password: ");
    readString(temp2);
    if(temp2 != temp1)
    {
        fprintf(stderr, "*** Error: password:s mismatch\n");
        return false;
    }

    password = temp1;
    return true;
}

/*
    configMenu: This is the menu available when logged in. I want to alter variables from the calling function (main) inside this and therefore i used references, which here is kind of ugly, out a simple solution
                Returns the option enum chosen
*/
MenuOption configMenu(void)
{
    char choice;

    printf("%c) Toggle alarm\n", SetAlarm);
    printf("%c) Change password\n", ChangePassword);
    printf("%c) Log out\n", Logout);
    printf("%c) Exit\n", Exit);

    do
    {
        printf("> ");
    } while(!readChar(choice));

    return (MenuOption)choice;
}

/*
    userSession: Includes 'configMenu' and the login/logout printing, this function is called in multiple places and needed refactoring
*/
void userSession(UserEntry* userentry, bool& isAlarmed, Logger& logger)
{
    printf("Alarm is %s!\n", statusString(isAlarmed).c_str());
    logger.WriteCSV(LogEntry(time(NULL), userentry->GetID(), "User logged in"));
    printf("User logged in\n\n");

    MenuOption option = MenuOption::None;
    while(option != MenuOption::Logout)
    {
        switch((option = configMenu()))
        {
            case MenuOption::SetAlarm:
            {
                isAlarmed = !isAlarmed; // Set to its opposite / toggle alarm ON/OFF
                logger.WriteCSV(LogEntry(time(NULL), userentry->GetID(), "Alarm set to " + statusString(isAlarmed)));
                printf("*** Alarm set to %s\n", statusString(isAlarmed).c_str());

                break;
            }

            // Because of a temporary variable inside a case, I had to encapsule the following to avoid compilation error (I might as well wrap all case labels in {} blocks).
            // Variable 'temp' exists otherwise in all the case labels below it.
            case MenuOption::ChangePassword:
            {
                std::string temp;
                if(changePassword(temp))
                {
                    // If new password is acceptable, do the change in the table (With a pointer you don't need to find the entry again)
                    userentry->SetPassword(temp);
                    logger.WriteCSV(LogEntry(time(NULL), userentry->GetID(), "Password changed"));
                    printf("*** Password changed\n");
                }

                break;
            }

            case MenuOption::Logout:
            {
                logger.WriteCSV(LogEntry(time(NULL), userentry->GetID(), "User logged out"));
                printf("User logged out\n");
                break;
            }

            case MenuOption::Exit:
            {
                logger.WriteCSV(LogEntry(time(NULL), userentry->GetID(), "Application exit"));
                exit(EXIT_SUCCESS); // Exits program, returns 0
                break;
            }

            default:
            {
                fprintf(stderr, "*** Invalid input\n");
                option = MenuOption::None;
                break; // Not actually needed in 'default', but to be consistent
            }
        }

        printf("\n");
    }
}

/*
    statusString: Lazy function to return "ON"/"OFF"
*/
std::string statusString(bool isOn)
{
    return isOn ? "ON" : "OFF"; // Using the conditional (ternary) operator looks better than if-statements
}
