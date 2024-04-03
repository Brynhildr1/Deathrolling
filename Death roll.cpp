#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <iomanip>
#include <cctype>
#include <windows.h>
#include <cstdlib>
#include <ctime>

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

struct AccountInfo
{
    char accountLetter;
    double balance;
};

AccountInfo getAccountLetter()
{
    AccountInfo accountInfo;

    string accountInput;
    char accountLetter;
    int pin;
    double balance = 0.0;

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

        // Get / Validate Pin
        int numTries = 0;

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

                    // Find the position of the space after the balance value
                    size_t spacePos = line.find(' ', 6);

                    // Extract the balance substring from the line
                    string balanceSubstring = line.substr(6, spacePos - 6);

                    // Convert the balance substring to an integer
                    balance = stoi(balanceSubstring);

                    continue;
                }
                else
                {
                    cout << "\nPin is incorrect\n";
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

    // return the account letter and its balance to the main function using a struct
    accountInfo.accountLetter = accountLetter;
    accountInfo.balance = balance;

    return accountInfo;
}

// Function to update the account balance in Accounts.txt
void updateAccountBalance(char accountLetter, double newBalance)
{
    ifstream inFile("Accounts.txt");
    ofstream outFile("temp.txt");

    string line;
    while (getline(inFile, line))
    {
        if (line[0] == accountLetter)
        {
            // Find the position of the space after the account letter
            size_t spacePos = line.find(' ');

            // Construct the updated line with the new balance
            string updatedLine = line.substr(0, spacePos + 1) + to_string(static_cast<int>(newBalance));

            // Write the updated line to the temporary file
            outFile << updatedLine << endl;
        }
        else
        {
            // Write the unchanged line to the temporary file
            outFile << line << endl;
        }
    }

    inFile.close();
    outFile.close();

    // Replace the original file with the updated one
    remove("Accounts.txt");
    rename("temp.txt", "Accounts.txt");
}

int main()
{
    // Initialize the random number generator
    srand(static_cast<unsigned int>(time(nullptr)));

    // Get user 1 account Letter
    cout << "Player 1:\n";

    AccountInfo account1Info = getAccountLetter();
    char account1 = account1Info.accountLetter;
    double balance1 = account1Info.balance;
    cout << "\nPlayer 1 Account: " << account1 << ", Login successful\n";
    cout << "Current account " << account1 << " balance: " << balance1 << "\n\n";

    // Get user 2 / Check if account is already logged in
    char account2;
    double balance2;

    bool uniqueValidation = false;
    do
    {
        cout << "Player 2:\n";
        AccountInfo account2Info = getAccountLetter();
        account2 = account2Info.accountLetter;
        balance2 = account2Info.balance;
        if (account2 != account1)
        {
            uniqueValidation = true;
        }
        else
        {
            cout << "Account: " << account2 << " is already logged in.\n";
            cout << "Reissuing task.\n\n";
        }
    } while (!uniqueValidation);

    cout << "\nPlayer 2 Account: " << account2 << ", Login successful\n";
    cout << "Current balance: " << balance2 << "";

    double bet;
    bool replay = true;
    // bet agreement between players and roll
    while (replay == true)
    {
        /* ask player 1 bet
        check if bet is valid (number/within balance)
        confirm bet with player 2
        if yes, roll

        if no, ask player 2 bet
        check if bet is valid (number/within balance)
        confirm bet with player 1
        if yes, roll

        if no, loop */

        // ask player 1 bet
        cout << "\n\nPlayer " << account1 << ": Enter Bet ($): "; // ask and input p1 bet
        cin >> bet;

        // validate that p1 bet is a number and within the balance of both players, if not error and loop.
        while (cin.fail() || bet > balance1 || bet > balance2)
        {
            cout << "\nInvalid bet. Please enter a valid numeric value within player balances.\nPlayer " << account1 << ": " << balance1 << ", Player " << account2 << ": " << balance2 << "\n\nPlayer " << account1 << " Enter Bet ($): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin >> bet;
        }

        // confirm bet with player2
        char betConfirmation;
        cout << "\nPlayer " << account2 << ", Do you want to match the bet ($): " << bet << "? (Y/N): ";
        while (true) // ensure user input is either Yy, Nn.
        {
            cin >> betConfirmation;
            if (betConfirmation == 'Y' || betConfirmation == 'y' || betConfirmation == 'N' || betConfirmation == 'n')
            {
                break;
            }
            cout << "Invalid input. Please enter 'Y' or 'N': ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (betConfirmation == 'Y' || betConfirmation == 'y')
        {
            // *Roll*
            int roll = bet * 10;
            cout << "Begin Deathrolling from: " << roll << "\n\n";

            while (true)
            {
                // Player 1's turn
                cout << "Player " << account1 << " rolls from: " << roll;
                roll = rand() % roll + 1;
                cout << ", rolling: " << roll << endl;

                if (roll == 1)
                {
                    cout << "Player " << account2 << " wins ($): " << bet << "\n\n";
                    // balance updates
                    balance1 -= bet;
                    balance2 += bet;
                    cout << "New account balances are:\n"
                         << "Player " << account1 << ": " << balance1 << "\nPlayer " << account2 << ": " << balance2;
                    break;
                }

                sleep(1); // introduce a 1-second delay

                // Player 2's turn
                cout << "Player " << account2 << " rolls from: " << roll;
                roll = rand() % roll + 1;
                cout << ", rolling: " << roll << endl;

                if (roll == 1)
                {
                    cout << "Player " << account1 << " wins ($): " << bet << "\n\n";
                    // balance updates
                    balance1 += bet;
                    balance2 -= bet;
                    cout << "New account balances are:\n"
                         << "Player " << account1 << ": " << balance1 << "\nPlayer " << account2 << ": " << balance2;
                    break;
                }

                sleep(1); // introduce a 1-second delay
            }
            updateAccountBalance(account1, balance1);
            updateAccountBalance(account2, balance2);
        }
        else
        {
            // ask for player 2 bet
            cout << "Player " << account2 << ": Enter Bet ($): "; // ask and input p1 bet
            cin >> bet;

            // validate that p1 bet is a number and within the balance of both players, if not error and loop.
            while (cin.fail() || bet > balance1 || bet > balance2)
            {
                cout << "\nInvalid bet. Please enter a valid numeric value within player balances.\nPlayer " << account1 << ": " << balance1 << ", Player " << account2 << ": " << balance2 << "\n\nPlayer " << account2 << " Enter Bet ($): ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin >> bet;
            }

            // confirm bet with player2
            char betConfirmation;
            cout << "\nPlayer " << account1 << ", Do you want to match the bet ($): " << bet << "? (Y/N): ";
            while (true) // ensure user input is either Yy, Nn.
            {
                cin >> betConfirmation;
                if (betConfirmation == 'Y' || betConfirmation == 'y' || betConfirmation == 'N' || betConfirmation == 'n')
                {
                    break;
                }
                cout << "Invalid input. Please enter 'Y' or 'N': ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            if (betConfirmation == 'Y' || betConfirmation == 'y')
            {
                // *Roll*
                int roll = bet * 10;
                cout << "Begin Deathrolling from: " << roll << "\n\n";

                while (true)
                {
                    // Player 2's turn
                    cout << "Player " << account2 << " rolls from: " << roll;
                    roll = rand() % roll + 1;
                    cout << ", rolling: " << roll << endl;

                    if (roll == 1)
                    {
                        cout << "Player " << account1 << " wins ($): " << bet << "\n\n";
                        // balance updates
                        balance1 += bet;
                        balance2 -= bet;
                        cout << "New account balances are:\n"
                             << "Player " << account1 << ": " << balance1 << "\nPlayer " << account2 << ": " << balance2;
                        break;
                    }

                    sleep(1); // introduce a 1-second delay

                    // Player 1's turn
                    cout << "Player " << account1 << " rolls from: " << roll;
                    roll = rand() % roll + 1;
                    cout << ", rolling: " << roll << endl;

                    if (roll == 1)
                    {
                        cout << "Player " << account2 << " wins ($): " << bet << "\n\n";
                        // balance updates
                        balance1 -= bet;
                        balance2 += bet;
                        cout << "New account balances are:\n"
                             << "Player " << account1 << ": " << balance1 << "\nPlayer " << account2 << ": " << balance2;
                        break;
                    }

                    sleep(1); // introduce a 1-second delay
                }
                updateAccountBalance(account1, balance1);
                updateAccountBalance(account2, balance2);
            }
            else
            {
                // Loop
                continue;
            }
        }

        // request replay,
        char replayKey;
        cout << "\n\nDo you want to play again (Y/N): ";

        while (true) // ensure user input is either Yy, Nn.
        {
            cin >> replayKey;
            if (replayKey == 'Y' || replayKey == 'y' || replayKey == 'N' || replayKey == 'n')
            {
                break;
            }
            cout << "\nInvalid input. Please enter 'Y' or 'N': ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (replayKey == 'N' || replayKey == 'n')
        {
            break;
        }
        else
        {
            continue;
        }
    }

    return 0;
}
