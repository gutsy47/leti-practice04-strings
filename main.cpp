#include <iostream>
#include <iomanip>
#include <fstream>

unsigned short getInput(std::string &str) {
    // Default input
    std::cout << "<< Write the sentence (or just press the Enter to apply reading from the input.txt):\n>> ";
    std::cin.sync();
    std::getline(std::cin, str);

    // Read from file if input is empty
    if (str.empty()) {
        const char* filePath = "C:/dev/university/cpp/practice_4/input.txt";
        std::ifstream file(filePath, std::ios::in);
        if (!file.is_open()) {
            std::cout << "FileNotFoundError: No such file or directory\n";
            return 1;
        }
        while (!file.eof()) std::getline(file, str);
        file.close();
    }
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

void showFilteredByStr(std::string &str, const char * alphabet, unsigned alpLen) {
    unsigned long length = str.length();
    const char fullAlphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    unsigned int fullAlpLen = sizeof(fullAlphabet)/sizeof(fullAlphabet[0]);

    // Split to words
    std::string words[100];
    std::string current;
    unsigned short counter = 0;
    for (int i = 0; i < length; ++i) {
        if (findCharEntrance(str[i], fullAlphabet, fullAlpLen) != -1) {
            current += str[i];
        } else if (!current.empty()){
            words[counter++] = current;
            current = "";
        }
    }
    words[counter++] = current;

    // Filter
    for (int i = 0; i < counter; ++i) {
        unsigned int wordLen = words[i].size()/sizeof(char);
        bool isWordCorrect = true;
        for (int j = 0; j < wordLen; ++j) {
            if (findCharEntrance(words[i][j], alphabet, alpLen) == -1) {
                isWordCorrect = false;
                break;
            }
        }
        if (isWordCorrect) std::cout << words[i] << ' ';
    }
    std::cout << '\n';
}

int main() {
    // Init the string
    std::string input;
    unsigned short response = getInput(input);
    if (response == 1) return 1;

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
                std::cout << " After: " << input << std::endl;
                break;
            }

            // Filter words by letters or numbers presence
            case '2': {
                std::cout << "  Your input: " << input << std::endl;

                // Alphabets for filters
                const char lettersOnly[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
                const char numbersOnly[] = "0123456789";
                const char fullAlphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

                // Output
                std::cout << "Letters only: ";
                showFilteredByStr(input, lettersOnly, sizeof(lettersOnly)/sizeof(lettersOnly[0])-1);
                std::cout << "Numbers only: ";
                showFilteredByStr(input, numbersOnly, sizeof(numbersOnly)/sizeof(numbersOnly[0])-1);
                std::cout << "  Everything: ";
                showFilteredByStr(input, fullAlphabet, sizeof(fullAlphabet)/sizeof(fullAlphabet[0])-1);
                break;
            }

            // Update the user input
            case 'c': {
                response = getInput(input);
                if (response == 1) return 1;
                break;
            }

            // Help
            case 'h': {
                std::cout << "Available commands:\n";
                std::cout << std::setw(32) << std::setfill('-') << '\n';
                std::cout << "h: Help\n";
                std::cout << std::setw(32) << std::setfill('-') << '\n';
                std::cout << "1: Correct the text\n";
                std::cout << "2: Filter & display words by letters or numbers presence\n";
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
