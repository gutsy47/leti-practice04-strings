#include <iostream>
#include <iomanip>
#include <fstream>


short int overwriteStringFromFile(const char * filePath, std::string &str) {
    std::ifstream file(filePath, std::ios::in);
    if (!file.is_open()) return 1;
    while (!file.eof()) std::getline(file, str);
    file.close();
    return 0;
}

int findCharEntrance(char chr, const char * str, unsigned strLen) {
    for (int i = 0; i < strLen; ++i)
        if (str[i] == chr) return i;
    return -1;
}

void removeExtraSpaces(std::string &str) {
    unsigned long length = str.length();

    // Left trim
    for (int i = 0; i < length; ++i) {
        if (str[i] != ' ') break;
        str = str.substr(i+1, --length);
    }

    // Trim spaces between words
    for (int i = 0; i < length; ++i) {
        if (str[i] == ' ' && str[i+1] == ' ') {
            str = str.substr(0, i) + str.substr(i+1, --length);
            --i;
        }
    }

    // Right trim
    if (str[length-1] == ' ') str = str.substr(0, length-2);
}

void correctPunctuation(std::string &str) {
    unsigned long length = str.length();

    for (int i = 0; i < length; ++i) {
        // \W[. ! ? !? ... !!! ???]\s[A-Z0-9]
        if (findCharEntrance(str[i], ".!?", 3) != -1) {
            // Wrong before the symbol
            if (findCharEntrance(str[i-1], " .?!,([{+-*", 11) != -1) {
                str = str.substr(0, i-1) + str.substr(i, length--);
                i--;
            }
            if (str[i + 1] == ' ') continue;  // Space after symbol - all is correct
            if ((str[i] == '!' && str[i+1] == '?') || (str[i] == '?' && str[i+1] == '!')) i++;  // ?! or !?
            if (str[i] == str[i+1] && str[i+1] == str[i+2]) i += 2; // Triple
            for (int j = i+1; findCharEntrance(str[j], ".?!,()[]{}+-*", 13) != -1; ++j) // Another symbols
                str = str.substr(0, j) + str.substr(j + 1, length--);
        }

        // \W[,)]}]\s\W || \W[([{+-*]\W
        if (findCharEntrance(str[i], ",+-*()[]{}", 10) != -1) {
            // Space before
            if (str[i-1] == ' ') {
                str = str.substr(0, i-1) + str.substr(i, length--);
                i--;
            }
            // Another symbol before the current
            if (findCharEntrance(str[i-1], " .?!,([{+-*", 11) != -1) {
                str = str.substr(0, i) + str.substr(i+1, length--);
                i--;
            }
            // Space afterwards
            if (str[i+1] == ' ' && findCharEntrance(str[i+1], "([{+-*", 6) != -1)
                str = str.substr(0, i-1) + str.substr(i, length--);
            // Another symbols
            for (int j = i+1; findCharEntrance(str[j], ".?!,()[]{}+-*", 13) != -1; ++j)
                str = str.substr(0, j) + str.substr(j + 1, length--);
        }
    }
}

void correctLetterCase(std::string &str) {
    unsigned long length = str.length();
    const char alphabetLower[] = "abcdefghijklmnopqrstuvwxyz";
    const char alphabetUpper[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned alpLen = sizeof(alphabetLower)/sizeof(alphabetLower[0]) - 1;

    // First letter always capital
    if (findCharEntrance(str[0], alphabetUpper, alpLen) == -1)
        str[0] = alphabetUpper[findCharEntrance(str[0], alphabetLower, alpLen)];

    // All not after .?! is lower
    for (int i = 1; i < length; ++i) {
        // Lower
        if (findCharEntrance(str[i], alphabetUpper, alpLen) != -1)
            str[i] = alphabetLower[findCharEntrance(str[i], alphabetUpper, alpLen)];

        // New sentence
        if (findCharEntrance(str[i-2], ".?!", 3) != -1
            && findCharEntrance(str[i], alphabetLower, alpLen) != -1) {
            str[i] = alphabetUpper[findCharEntrance(str[i], alphabetLower, alpLen)];
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    // Init the string
    std::string input;
    std::cout << "<< Write the sentence (or just press the Enter to apply reading from the input.txt):\n>> ";
    std::getline(std::cin, input);

    // Read from file if input is empty
    if (input.empty()) {
        const char* filePath = "C:/dev/university/cpp/practice_4/input.txt";
        short int response = overwriteStringFromFile(filePath, input);
        if (response != 0) {
            std::cout << "FileNotFoundError: No such file or directory\n";
            return 1;
        }
    }

    // Main loop
    std::cout << "Enter 'h' to get list of commands\n";
    while (true) {

        // Get command from user
        char userAction;
        std::cout << "<< Action:\n>> ";
        std::cin.sync();
        std::cin >> userAction;

        // Exit
        if (userAction == '0') break;

        switch (userAction) {

            // Edit text
            case '1': {
                std::cout << "Before: " << input << std::endl;
                removeExtraSpaces(input);
                correctPunctuation(input);
                correctLetterCase(input);
                std::cout << "After: " << input << std::endl;
                break;
            }

            // Update the user input
            case 'c': {
                std::cout << "<< Write the sentence (or just press the Enter to apply reading from the input.txt):\n>> ";
                std::cin.sync();
                std::getline(std::cin, input);

                // Read from file if input is empty
                if (input.empty()) {
                    const char* filePath = "C:/dev/university/cpp/practice_4/input.txt";
                    short int response = overwriteStringFromFile(filePath, input);
                    if (response != 0) {
                        std::cout << "FileNotFoundError: No such file or directory\n";
                        return 1;
                    }
                }
                break;
            }

            // Help
            case 'h': {
                std::cout << "Available commands:\n";
                std::cout << std::setw(32) << std::setfill('-') << '\n';
                std::cout << "h: Help\n";
                std::cout << std::setw(32) << std::setfill('-') << '\n';
                std::cout << "1: Correct the text\n";
                std::cout << "c: Change the sentence (or reread from the input.txt file)\n";
                std::cout << std::setw(32) << std::setfill('-') << '\n';
                std::cout << "0: Exit\n";
                std::cout << std::setw(32) << std::setfill('-') << '\n';
                std::cout << "NOTE: Script will skip all the useless input (e.g. extra symbols in action input)\n";
                std::cout << std::setw(32) << std::setfill('-') << '\n';
                std::cout << std::setfill(' ');
                break;
            }

            // Unknown command error
            default: std::cout << "RuntimeError: unknown command\n";
        }
    }

    return 0;
}
