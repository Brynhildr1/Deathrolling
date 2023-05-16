# Deathrolling

The provided code is a C++ program that simulates a game called "Deathrolling" between two players. Here's a breakdown of what the code does:

    The code includes several header files that provide necessary functionality for input/output operations, file handling, string manipulation, and random number generation.

    Two helper functions are defined:
    a. accountExists(char accountLetter): This function checks if an account with the given account letter exists in the "Accounts.txt" file. It reads the file line by line and compares the first character of each line with the provided account letter.
    b. validatePin(const string &pin): This function validates if the provided PIN (as a string) is exactly four characters long and consists only of digits.

    A structure AccountInfo is defined to hold information about an account, including the account letter and balance.

    The getAccountLetter() function is implemented to get an account letter from the user. It prompts the user to enter a single uppercase letter and performs various checks to ensure a valid input. If the account doesn't exist, it offers the option to create a new account with a 4-digit PIN.

    The updateAccountBalance(char accountLetter, double newBalance) function updates the balance of an account in the "Accounts.txt" file. It reads the file, finds the line corresponding to the given account letter, and replaces the balance value with the new balance. The updated information is written to a temporary file, and then the original file is replaced with the temporary file.

    In the main() function, the program starts by initializing the random number generator.

    The program asks for the account letter of the first player using the getAccountLetter() function and retrieves the corresponding balance from the "Accounts.txt" file.

    The program then asks for the account letter of the second player, ensuring that it is different from the first player's account. The balance of the second player is also retrieved.

    The program enters a loop that allows the players to place bets and play the game multiple times.

    Within the loop, the program asks the first player for a bet and validates it. It then asks the second player if they want to match the bet and proceeds accordingly.

    The players take turns rolling the dice. The roll value is initialized based on the bet, and each player takes turns rolling the dice until a player rolls a 1. If a player rolls a 1, the other player wins the bet, and their balances are updated accordingly.

    After each game, the updateAccountBalance() function is called to update the account balances in the "Accounts.txt" file.

    Finally, the program asks the players if they want to play again and repeats the loop accordingly.

Note: This code assumes the existence of an "Accounts.txt" file, where account information is stored. The file format is expected to have each line representing an account and containing the account letter, PIN, and balance separated by spaces.
