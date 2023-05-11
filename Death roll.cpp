#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <iomanip>
#include <cctype>

using namespace std;

bool accountExists(char accountLetter)
{
    ifstream file("Accounts.txt");
    string line;
    while (getline(file, line))
    {
        if (line[0] == accountLetter)
        {
            return true;
        }
    }
    return false;
}

bool validatePin(const string &pin)
{
    // checking if input is exactly 4 characters long
    if (pin.size() != 4)
    {
        return false;
    }

    // check if each character is a digit
    for (char c : pin)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }

    // if all checks pass, the input is valid
    return true;
}

char getAccountLetter()
{
    string accountInput;
    char accountLetter;
    int pin;

    while (true) // Loop until a valid account letter is entered or the user chooses to quit
    {
        cout << "Enter Account Letter (A - Z): ";
        cin >> accountInput;

        // check length of input
        if (accountInput.length() != 1)
        {
            cout << "Invalid input. Please enter a single uppercase letter." << endl;
            continue; // recursively call the function to get a valid input
        }

        // check that first character is a uppercase letter
        accountLetter = accountInput[0];
        if (accountLetter < 'A' || accountLetter > 'Z')
        {
            cout << "Invalid input. Please enter a single uppercase letter." << endl;
            continue; // recursively call the function to get a valid input
        }

        // check is account exists
        bool accountExistsFlag = accountExists(accountLetter);

        if (!accountExistsFlag) // if the account doesnt exist then it will prompt the user to create one
        {
            bool invalidInput = false;
            char createAccountFlag;
            cout << "Player account does not exist. Create an account? (Y/N): ";
            while (true)
            {
                cin >> createAccountFlag;
                if (createAccountFlag == 'Y' || createAccountFlag == 'y' || createAccountFlag == 'N' || createAccountFlag == 'n')
                {
                    break;
                }
                cout << "Invalid input. Please enter 'Y' or 'N': ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (createAccountFlag == 'Y' || createAccountFlag == 'y')
            {
                cout << "Enter a 4-digit pin: ";
                cin >> pin;
                while (!validatePin(to_string(pin))) // validate the entered pin, as 4 digits long and all numbers
                {
                    cout << "Invalid pin. Enter a 4-digit pin: ";
                    cin >> pin;
                }
                // write the newly created account to Accounts.txt
                ofstream accountsFile("Accounts.txt", ios::app);
                accountsFile << accountLetter << pin << " 1000 " << endl;
                accountsFile.close();
                cout << "Account created successfully" << endl;
                break;
            }
            else
            {
                cout << "restarting\n\n";
                continue; // start another iteration of the loop to ask for a new account letter.
            }
        }

        // Get/Validate Pin
        int numTries = 0;
        double balance;

        ifstream file;
        file.open("Accounts.txt");
        string line;
        bool authenticated = false;
        while (getline(file, line) && numTries < 3)
        {
            if (line[0] == accountLetter)
            {
                cout << "Enter pin: ";
                cin >> pin;
                if (line.substr(1, 4) == to_string(pin))
                {
                    authenticated = true;
                    balance = stoi(line.substr(7));
                    continue;
                }
                else
                {
                    cout << "Pin is incorrect\n";
                    numTries++;
                    file.clear();
                    file.seekg(0, ios::beg);
                }
            }
        }
        if (!authenticated)
        {
            cout << "Maximum number of attempts exceeded, Returning to Account select.\n\n";
            continue;
        }

        // Break out of the loop and return the account letter if it exists
        break;
    }

    return accountLetter;
}

int main()
{
    // Get user 1 account Letter
    cout << "Player 1:\n";
    char account1 = getAccountLetter();
    cout << "Player 1 Account: " << account1 << ", Login successful\n\n";

    // Get user 2 / Check if account is already logged in
    char account2;

    bool uniqueValidation = false;
    while (!uniqueValidation)
    {
        cout << "player 2:\n";
        account2 = getAccountLetter();
        if (account2 != account1)
        {
            uniqueValidation = true; // if the same account isnt used then the validation will be true and continue
            continue;
        }
        else
        {
            cout << "Account: " << account2 << " is already logged in.\n"; // if the same account is used
            cout << "Reissuing task.\n\n";
        }
    }

    cout << "Player 2 Account: " << account2 << ", Login successful\n\n";

    int x;
    cin >> x;

    return 0;
}