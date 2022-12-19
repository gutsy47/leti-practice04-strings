#include <iostream>
#include <iomanip>
#include <fstream>


short int overwriteStringFromFile(const char* filePath, std::string *ptrString) {
    std::ifstream file(filePath, std::ios::in);
    if (!file.is_open()) return 1;
    while (!file.eof()) std::getline(file, *ptrString);
    file.close();
    return 0;
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
        short int response = overwriteStringFromFile(filePath, &input);
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

            // Update the user input
            case 'c': {
                std::cout << "<< Write the sentence (or just press the Enter to apply reading from the input.txt):\n>> ";
                std::cin.sync();
                std::getline(std::cin, input);

                // Read from file if input is empty
                if (input.empty()) {
                    const char* filePath = "C:/dev/university/cpp/practice_4/input.txt";
                    short int response = overwriteStringFromFile(filePath, &input);
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
