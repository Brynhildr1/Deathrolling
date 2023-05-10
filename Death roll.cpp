#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

// Function to check if an account exists
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

// Function to create a new account
void createAccount(char accountLetter)
{
    ofstream file("Accounts.txt", ios_base::app);
    file << accountLetter << " 1234 1000\n"; //  pin of 1234 and default starting balance of $1000
    file.close();
}

// Function to validate a 4-digit pin
bool validatePin(string pin)
{
    if (pin.length() != 4)
    {
        return false;
    }
    for (char c : pin)
    {
        if (!isdigit(c))
        {
            return false;
        }
    }
    return true;
}

// Function to prompt user for account creation and validate input
char promptForAccountCreation()
{
    char createAccount;
    do
    {
        cout << "Account does not exist. Create an account? (Y/N): ";
        cin >> createAccount;
        if (createAccount == 'Y' || createAccount == 'y' || createAccount == 'N' || createAccount == 'n')
        {
            break;
        }
        else
        {
            cout << "Invalid input. Please enter Y or N: ";
            cin.clear();                                         // clear the input stream
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore any remaining characters in the input buffer
        }
    } while (true);
    return createAccount;
}

// Main function
int main()
{
    char account;
    string pin;
    int balance = 1000;

    while (true)
    {
        cout << "Enter your account (A/B/C/D): ";
        cin >> account;

        ifstream accountsFile("Accounts.txt");
        bool accountExists = false;

        if (accountsFile.is_open())
        {
            string line;
            while (getline(accountsFile, line))
            {
                if (line[0] == account)
                {
                    accountExists = true;
                    int tries = 0;
                    while (tries < 3)
                    {
                        cout << "Enter your pin: ";
                        cin >> pin;
                        if (line.substr(1, 4) == pin) // compare only the first 4 characters
                        {
                            cout << "Login successful" << endl;
                            balance = stoi(line.substr(7, 4)); // extract 4 characters starting from the 8th character
                            break;
                        }
                        else
                        {
                            cout << "Pin is incorrect. Please re-enter your pin." << endl;
                            tries++;
                        }
                    }
                    if (tries == 3)
                    {
                        cout << "Maximum number of attempts reached. Please try again." << endl;
                        break;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            accountsFile.close();
        }

        if (!accountExists)
        {
            char createAccount = promptForAccountCreation();
            if (createAccount == 'Y' || createAccount == 'y')
            {
                cout << "Enter a 4-digit pin: ";
                cin >> pin;
                while (!validatePin(pin) || pin.length() != 4)
                {
                    cout << "Invalid pin. Enter a 4-digit pin: ";
                    cin >> pin;
                }
                ofstream accountsFile("Accounts.txt", ios::app);
                accountsFile << account << pin << " 1000 " << endl;
                accountsFile.close();
                cout << "Account created successfully" << endl;
            }
            else if (createAccount == 'N' || createAccount == 'n')
            {
                cout << "Returning" << endl;
                continue;
            }
            else
            {
                cout << "Invalid input. Please try again." << endl;
            }
        }
    }

    return 0;
}