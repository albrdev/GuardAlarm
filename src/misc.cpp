#include "misc.h"

/*
    checkPassword: Checks if the provided password matches the one stored in the database
              Returns status bits of the authentication
                AS_SUCCESS: The passwords matched successfully
                AS_EMERGENCY: The password matched with its emergency version, an attack might be possible
                AS_FAILURE: The passwords did not match

                AuthStatus is a bitflag enum, we can set different bits with OR and have multiple statuses set in one integer
*/
int checkPassword(const std::string& providedPassword, const std::string& storedPassword)
{
    if(providedPassword != storedPassword)
    {
        std::string temp = storedPassword;
        size_t lastIndex = temp.length() - 1;
        temp[lastIndex] = (temp[lastIndex] == '9' ? '0' : (char)(temp[lastIndex] + 1)); // Had to cast to char again

        if(providedPassword == temp)
        {
            return AS_SUCCESS | AS_EMERGENCY; // Return both these bits to indicate that it's not just a successfull login, it's also an emergency
        }

        return AS_FAILURE; // Zero is failure
    }

    return AS_SUCCESS; // Returns success bit only, no emergency
}

/*
    userLogin: Finds user in database and checks if password is correct
               Uses a reference to a pointer because I think return value should be the status of the login attempt
*/
int userLogin(const Credentials& providedCredentials, Database& database, Credentials*& result)
{
    return (result = database.FindByUsername(providedCredentials.GetUsername())) != NULL ? checkPassword(providedCredentials.GetPassword(), result->GetPassword()) : AS_FAILURE;
}

/*
    emergencyResponse: This function determines what to do when the emergency password has been entered (Now it will just display a message + timestamp)
*/
void emergencyResponse(void)
{
    printf("[%s] *** Emergency code detected: Notifying the authorities!\n", timetostr("%H:%M:%S").c_str());
    printf("Loading police, please wait.....\n");
}

/*
    inputCredentials: Login input dialog, asks for username and password and stores them in a Credentials object passed as a referece to this function
*/
void inputCredentials(Credentials& credentials)
{
    bool validationSuccessful = false;
    while(!validationSuccessful)
    {
        // This is a unconventional approach on how to avoid ugly nestled if-statements
        // do { ... } while(false); will loop only once, but we will have the ability to break out of it anytime we want (and try all the input phases again), which will save us from nestling if, else if, else
        do
        {
            std::string username, password;

            printf("Username: ");
            readString(username);
            validationSuccessful = Credentials::ValidateUsername(username);
            if(!validationSuccessful)
            {
                fprintf(stderr, "*** Error: Invalid username input\n");
                break;
            }

            printf("Input password: ");
            readString(password);
            validationSuccessful = Credentials::ValidatePassword(password);
            if(!validationSuccessful)
            {
                fprintf(stderr, "*** Error: Invalid password input\n");
                break;
            }

            credentials = Credentials(-1, username, password);
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
    if(!Credentials::ValidatePassword(temp1))
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
                Returns 'true' if the menu should continue to ask for options and 'false' if the user wants to log out
                Parameter 'credentials' for changing password code (needed a pointer to update the database entry directly)
                Parameter 'isAlarmed' is an out parameter to set the variable in the calling function
*/
bool configMenu(Credentials* const credentials, bool& isAlarmed)
{
    char choice;
    bool validChoice = false;

    while(!validChoice)
    {
        printf("%c) Turn alarm %s\n", MO_SETALARM, statusString(!isAlarmed).c_str());
        printf("%c) Change password\n", MO_CHANGEPASSWORD);
        printf("%c) Log out\n", MO_LOGOUT);
        printf("%c) Exit\n", MO_EXIT);

        do
        {
            printf("> ");
        } while(!readChar(choice));

        switch(choice)
        {
            case MO_SETALARM:
            {
                isAlarmed = !isAlarmed; // Set to its opposite / toggle alarm ON/OFF
                printf("*** Alarm set to %s\n", statusString(isAlarmed).c_str());

                validChoice = true;
                break;
            }

            // Because of a temporary variable inside a case, I had to encapsule the following to avoid compilation error (I might as well wrap all case labels in {} blocks).
            // Variable 'temp' exists otherwise in all the case labels below it.
            case MO_CHANGEPASSWORD:
            {
                std::string temp;
                if(changePassword(temp))
                {
                    // If new password is acceptable, do the change in the database (With a pointer you don't need to find the entry again)
                    credentials->SetPassword(temp);
                    printf("*** Password changed\n");
                }

                validChoice = true;
                break;
            }

            case MO_LOGOUT:
            {
                return false; // Return 'false' to indicate that we don't want to loop the menu (i.e. log out)
            }

            case MO_EXIT:
            {
                exit(EXIT_SUCCESS); // Exits program, returns 0
                break;
            }

            default:
            {
                fprintf(stderr, "*** Invalid input\n");
                break; // Not actually needed in 'default', but to be consistent
            }
        }
    }

    return true;
}

/*
    userSession: Includes 'configMenu' and the login/logout printing, this function is called in multiple places and needed refactoring
*/
void userSession(Credentials* credentials, bool& isAlarmed)
{
    printf("Alarm is %s!\n", statusString(isAlarmed).c_str());
    printf("[%s] User \'%s\' logged in\n", timetostr("%H:%M:%S").c_str(), credentials->GetUsername().c_str());

    do
    {
        printf("\n");
        statusString(isAlarmed);
    } while(configMenu(credentials, isAlarmed));

    printf("[%s] User \'%s\' logged out\n\n", timetostr("%H:%M:%S").c_str(), credentials->GetUsername().c_str());
}

/*
    statusString: Lazy function to return "ON"/"OFF"
*/
std::string statusString(bool isOn)
{
    return isOn ? "ON" : "OFF"; // Using the conditional (ternary) operator looks better than if-statements
}
