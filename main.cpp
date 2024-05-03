#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <wchar.h>
#include <string.h>
#include <time.h>
#include <io.h>
#include <sys/stat.h>
#include <sys/types.h>


// Structure to represent a dynamic array of wide-character strings
typedef struct {
    wchar_t** data; // Pointer to the array of strings
    size_t size;    // Number of strings in the array
    size_t capacity; // Capacity of the array
} WCharArray;

// Initialize a dynamic array
void initWCharArray(WCharArray* arr) {
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

// Add a string to a dynamic array
void addWChar(WCharArray* arr, const wchar_t* str) {
    if (arr->size >= arr->capacity) {
        size_t newCapacity = (arr->capacity == 0) ? 1 : arr->capacity * 2;
        wchar_t** newData = (wchar_t**)realloc(arr->data, newCapacity * sizeof(wchar_t*));
        if (newData == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        arr->data = newData;
        arr->capacity = newCapacity;
    }
    arr->data[arr->size++] = _wcsdup(str);
    if (arr->data[arr->size - 1] == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
}

// Free the memory allocated by a dynamic array
void freeWCharArray(WCharArray* arr) {
    for (size_t i = 0; i < arr->size; ++i) {
        free(arr->data[i]);
    }
    free(arr->data);
}

// Function to generate a random number between min and max (inclusive)
int getRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Function to generate all 2-digit numbers from 01 to 99
// Function to generate all numbers from 01 to 99, along with special numbers and years from 1980 to 2024
// Function to generate all 2-digit numbers from 01 to 99, along with special numbers and years from 1980 to 2024
void generateAllNumbers(WCharArray* numbers) {
    // Add special numbers
    const wchar_t specialNumbers[][5] = { L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"01", L"02", L"03", L"04", L"05", L"06", L"07", L"08", L"09", L"10", L"11", L"12", L"13", L"14", L"15", L"16", L"17", L"18", L"19", L"20", L"21", L"22", L"23", L"24", L"25", L"26", L"27", L"28", L"29", L"30", L"31", L"32", L"33", L"34", L"35", L"36", L"37", L"38", L"39", L"40", L"41", L"42", L"43", L"44", L"45", L"46", L"47", L"48", L"49", L"50", L"51", L"52", L"53", L"54", L"55", L"56", L"57", L"58", L"59", L"60", L"61", L"62", L"63", L"64", L"65", L"66", L"67", L"68", L"69", L"70", L"71", L"72", L"73", L"74", L"75", L"76", L"77", L"78", L"79", L"80", L"81", L"82", L"83", L"84", L"85", L"86", L"87", L"88", L"89", L"90", L"91", L"92", L"93", L"94", L"95", L"96", L"97", L"98", L"99", L"1337", L"9000", L"007", L"420" };
    const int numSpecialNumbers = sizeof(specialNumbers) / sizeof(specialNumbers[0]);

    for (int i = 0; i < numSpecialNumbers; ++i) {
        addWChar(numbers, specialNumbers[i]);
        printf("Special number: %ls\n", specialNumbers[i]);
    }

    // Add years from 1980 to 2024
    for (int year = 1980; year <= 2024; ++year) {
        wchar_t yearString[5];
        swprintf(yearString, 5, L"%d", year);
        addWChar(numbers, yearString);
        printf("Year: %ls\n", yearString);
    }
}

// Function to generate all possible combinations of special characters
void generateAllSpecialCharCombinations(WCharArray* specialChars) {
    const wchar_t specialCharsList[] = { L'!', L'@', L'#', L'$' };
    const int numChars = sizeof(specialCharsList) / sizeof(specialCharsList[0]);

    // Loop to generate combinations of lengths 1 and 2
    for (int i = 0; i < numChars; ++i) {
        // Add single-character combinations
        wchar_t combination[2] = { specialCharsList[i], L'\0' };
        addWChar(specialChars, combination);
        printf("Special char combination: %ls\n", combination);

        // Add two-character combinations
        for (int j = 0; j < numChars; ++j) {
            wchar_t combination[3] = { specialCharsList[i], specialCharsList[j], L'\0' };
            addWChar(specialChars, combination);
            printf("Special char combination: %ls\n", combination);
        }
    }
}



// Function to read words from text files in a directory
void readWordsFromDirectory(const wchar_t* directoryPath, WCharArray* words) {
    WIN32_FIND_DATAW findFileData;
    HANDLE hFind;
    wchar_t searchPath[MAX_PATH];
    swprintf(searchPath, MAX_PATH, L"%s\\*.txt", directoryPath);
    hFind = FindFirstFileW(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        wprintf(L"No text files found in directory: %s\n", directoryPath);
        return;
    }
    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            wchar_t filePath[MAX_PATH];
            swprintf(filePath, MAX_PATH, L"%s\\%s", directoryPath, findFileData.cFileName);
            FILE* file = _wfopen(filePath, L"r");
            if (file == NULL) {
                wprintf(L"Error opening file: %s\n", filePath);
                continue;
            }
            wchar_t line[256];
            while (fgetws(line, sizeof(line) / sizeof(line[0]), file) != NULL) {
                // Remove newline character and check if the last character is missing
                size_t length = wcslen(line);
                if (length > 0 && line[length - 1] == L'\n') {
                    line[length - 1] = L'\0';
                    length--;
                }
                // Check if the last character is missing and add it back
                if (length > 0 && iswspace(line[length - 1])) {
                    line[length] = L'\0';
                    length++;
                }
                addWChar(words, line);
            }
            fclose(file);
        }
    } while (FindNextFileW(hFind, &findFileData) != 0);
    FindClose(hFind);
}

void createPossiblePasswords(const WCharArray* words, const WCharArray* numbers, const WCharArray* specialChars, WCharArray* passwords) {
    FILE* file = fopen("word_list_base.txt", "w");
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < words->size; ++i) {
        const wchar_t* word = words->data[i];
        const int wordLength = wcslen(word);

        // Write base word to file
        fwprintf(file, L"%s\n", word);

        // Lowercase word
        wchar_t* lowerWord = (wchar_t*)malloc((wordLength + 1) * sizeof(wchar_t)); // +1 for null terminator
        if (lowerWord == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        wcscpy(lowerWord, word);

        // Capitalize the first letter of the word
        wchar_t* capitalWord = (wchar_t*)malloc((wordLength + 1) * sizeof(wchar_t)); // +1 for null terminator
        if (capitalWord == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        wcscpy(capitalWord, word);
        capitalWord[0] = towupper(capitalWord[0]);

        // Reverse the word
        wchar_t* reverseWord = (wchar_t*)malloc((wordLength + 1) * sizeof(wchar_t)); // +1 for null terminator
        if (reverseWord == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < wordLength; ++j) {
            reverseWord[j] = word[wordLength - j - 1];
        }
        reverseWord[wordLength] = L'\0';

        for (size_t j = 0; j < numbers->size; ++j) {
            const wchar_t* number = numbers->data[j];
            const int numberLength = wcslen(number);

            for (size_t k = 0; k < specialChars->size; ++k) {
                const wchar_t* specialChar = specialChars->data[k];
                const int specialCharLength = wcslen(specialChar);

                const int maxPasswordLength = wordLength + numberLength + specialCharLength;
                wchar_t* password = (wchar_t*)malloc((maxPasswordLength + 1) * sizeof(wchar_t)); // +1 for null terminator
                if (password == NULL) {
                    perror("Memory allocation failed");
                    exit(EXIT_FAILURE);
                }

                // Construct password with lowercase word, capitalized word, reversed word, number, and special character
                wcscpy(password, lowerWord);
                wcscat(password, number);
                wcscat(password, specialChar);
                addWChar(passwords, password);

                wcscpy(password, capitalWord);
                wcscat(password, number);
                wcscat(password, specialChar);
                addWChar(passwords, password);

                wcscpy(password, reverseWord);
                wcscat(password, number);
                wcscat(password, specialChar);
                addWChar(passwords, password);
            }
        }

        free(lowerWord);
        free(capitalWord);
        free(reverseWord);
    }

    fclose(file);
}




void readPatternsFromFile(const char* filename, WCharArray* patterns) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    wchar_t pattern[50];
    while (fgetws(pattern, 50, file) != NULL) {
        // Remove newline character if present
        if (pattern[wcslen(pattern) - 1] == L'\n') {
            pattern[wcslen(pattern) - 1] = L'\0';
        }
        addWChar(patterns, pattern);
    }

    fclose(file);
}

void generatePasswordsFromPatterns(const WCharArray* patterns, const WCharArray* numbers, const WCharArray* specialChars, WCharArray* passwords) {
    for (size_t i = 0; i < patterns->size; ++i) {
        const wchar_t* pattern = patterns->data[i];
        const int patternLength = wcslen(pattern);

        // Generate passwords with the original pattern
        for (size_t j = 0; j < numbers->size; ++j) {
            const wchar_t* number = numbers->data[j];
            const int numberLength = wcslen(number);

            // Construct password with the original pattern and number
            wchar_t* password = (wchar_t*)malloc((patternLength + numberLength + 1) * sizeof(wchar_t));
            if (password == NULL) {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            wcscpy(password, pattern);
            wcscat(password, number);
            addWChar(passwords, password);

            // Generate passwords with the original pattern and special character combinations
            for (size_t k = 0; k < specialChars->size; ++k) {
                const wchar_t* specialCharCombination = specialChars->data[k];
                const int specialCharCombinationLength = wcslen(specialCharCombination);

                // Construct password with the original pattern, number, and special character combination
                password = (wchar_t*)malloc((patternLength + numberLength + specialCharCombinationLength + 1) * sizeof(wchar_t));
                if (password == NULL) {
                    perror("Memory allocation failed");
                    exit(EXIT_FAILURE);
                }
                wcscpy(password, pattern);
                wcscat(password, number);
                wcscat(password, specialCharCombination);
                addWChar(passwords, password);
            }
        }

        // Generate passwords with the pattern repeated twice and thrice
        for (int repeat = 2; repeat <= 3; ++repeat) {
            // Construct repeated pattern
            int repeatedPatternLength = patternLength * repeat;
            wchar_t* repeatedPattern = (wchar_t*)malloc((repeatedPatternLength + 1) * sizeof(wchar_t));
            if (repeatedPattern == NULL) {
                perror("Memory allocation failed");
                exit(EXIT_FAILURE);
            }
            repeatedPattern[0] = L'\0';  // Initialize as empty string
            for (int r = 0; r < repeat; ++r) {
                wcscat(repeatedPattern, pattern);
            }

            // Generate passwords with the repeated pattern and number
            for (size_t j = 0; j < numbers->size; ++j) {
                const wchar_t* number = numbers->data[j];
                const int numberLength = wcslen(number);

                // Construct password with the repeated pattern and number
                wchar_t* password = (wchar_t*)malloc((repeatedPatternLength + numberLength + 1) * sizeof(wchar_t));
                if (password == NULL) {
                    perror("Memory allocation failed");
                    exit(EXIT_FAILURE);
                }
                wcscpy(password, repeatedPattern);
                wcscat(password, number);
                addWChar(passwords, password);

                // Generate passwords with the repeated pattern, number, and special character combinations
                for (size_t k = 0; k < specialChars->size; ++k) {
                    const wchar_t* specialCharCombination = specialChars->data[k];
                    const int specialCharCombinationLength = wcslen(specialCharCombination);

                    // Construct password with the repeated pattern, number, and special character combination
                    password = (wchar_t*)malloc((repeatedPatternLength + numberLength + specialCharCombinationLength + 1) * sizeof(wchar_t));
                    if (password == NULL) {
                        perror("Memory allocation failed");
                        exit(EXIT_FAILURE);
                    }
                    wcscpy(password, repeatedPattern);
                    wcscat(password, number);
                    wcscat(password, specialCharCombination);
                    addWChar(passwords, password);
                }
            }

            free(repeatedPattern);
        }
    }
}

void generateLeetPasswords(const WCharArray* passwords, WCharArray* leetPasswords) {
    const wchar_t leetMap[][2] = {
        { L'a', L'4' },
        { L'e', L'3' },
        { L'i', L'1' },
        { L'o', L'0' },
        { L's', L'5' },
        { L't', L'7' },
    };
    const size_t leetMapSize = sizeof(leetMap) / sizeof(leetMap[0]);

    for (size_t i = 0; i < passwords->size; ++i) {
        const wchar_t* password = passwords->data[i];
        const size_t passwordLength = wcslen(password);

        // Allocate memory for Leet password
        wchar_t* leetPassword = (wchar_t*)malloc((passwordLength + 1) * sizeof(wchar_t));
        if (leetPassword == NULL) {
            perror("Memory allocation failed");
            exit(EXIT_FAILURE);
        }

        // Convert characters to Leet
        for (size_t j = 0; j < passwordLength; ++j) {
            wchar_t currentChar = password[j];
            bool isLeet = false;
            for (size_t k = 0; k < leetMapSize; ++k) {
                if (currentChar == leetMap[k][0]) {
                    leetPassword[j] = leetMap[k][1];
                    isLeet = true;
                    break;
                }
            }
            if (!isLeet) {
                leetPassword[j] = currentChar;
            }
        }
        leetPassword[passwordLength] = L'\0'; // Null-terminate the Leet password

        // Add Leet password to the array
        addWChar(leetPasswords, leetPassword);
    }
}



// Function to write passwords to a file, including leet passwords
void writePasswordsToFile(const WCharArray* passwords, const WCharArray* leetPasswords, const wchar_t* folder, const wchar_t* filename) {
    // Create directory if it doesn't exist
    if (_wmkdir(folder) != 0 && errno != EEXIST) {
        perror("Unable to create directory");
        exit(EXIT_FAILURE);
    }

    wchar_t filepath[256];
    swprintf(filepath, 256, L"%ls/%ls", folder, filename);
    FILE* file = _wfopen(filepath, L"w"); // Open file in write mode
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    // Write regular passwords
    for (size_t i = 0; i < passwords->size; ++i) {
        fwprintf(file, L"%s\n", passwords->data[i]); // Write password to file
    }

    // Write leet passwords
    for (size_t i = 0; i < leetPasswords->size; ++i) {
        fwprintf(file, L"%s\n", leetPasswords->data[i]); // Write leet password to file
    }

    fclose(file);
}

// Function to write passwords to a file subset
void writePasswordsToFileSubset(const WCharArray* passwords, const WCharArray* leetPasswords, const wchar_t* folder, const wchar_t* filename, size_t subsetSize) {
    // Create directory if it doesn't exist
    if (_wmkdir(folder) != 0 && errno != EEXIST) {
        perror("Unable to create directory");
        exit(EXIT_FAILURE);
    }

    wchar_t filepath[256];
    swprintf(filepath, 256, L"%ls/%ls", folder, filename);
    FILE* file = _wfopen(filepath, L"w"); // Open file in write mode
    if (file == NULL) {
        perror("Unable to open file");
        exit(EXIT_FAILURE);
    }

    // Write subset of regular passwords
    for (size_t i = 0; i < subsetSize && i < passwords->size; ++i) {
        fwprintf(file, L"%s\n", passwords->data[i]); // Write password to file
    }

    // Write subset of leet passwords
    for (size_t i = 0; i < subsetSize && i < leetPasswords->size; ++i) {
        fwprintf(file, L"%s\n", leetPasswords->data[i]); // Write leet password to file
    }

    fclose(file);
}




int main() {
    srand(time(NULL)); // Initialize the random number generator

    // Initialize dynamic arrays
    WCharArray words;
    initWCharArray(&words);
    WCharArray numbers;
    initWCharArray(&numbers);
    WCharArray specialChars;
    initWCharArray(&specialChars);
    WCharArray passwords;
    initWCharArray(&passwords);
    WCharArray patterns;
    initWCharArray(&patterns);
    WCharArray leetPasswords;
    initWCharArray(&leetPasswords);

    // Read words from text files in the directory
    readWordsFromDirectory(L"categorii", &words);

    // Generate all 2-digit numbers from 01 to 99
    generateAllNumbers(&numbers);

    // Generate all possible combinations of special characters
    generateAllSpecialCharCombinations(&specialChars);

    // Generate possible passwords for each word with every possible number or special character combination
    createPossiblePasswords(&words, &numbers, &specialChars, &passwords);

    // Read patterns from file
    readPatternsFromFile("patterns.txt", &patterns);

    // Generate passwords based on patterns
    generatePasswordsFromPatterns(&patterns, &numbers, &specialChars, &passwords);

    // Generate Leet passwords
    generateLeetPasswords(&passwords, &leetPasswords);

    // Write passwords to files based on the number of passwords
    writePasswordsToFile(&passwords, &leetPasswords, L"word_list", L"word_list_full.txt");

    // Write passwords to files based on the number of passwords
    writePasswordsToFileSubset(&passwords, &leetPasswords, L"word_list", L"word_list_10000.txt", 10000);
    writePasswordsToFileSubset(&passwords, &leetPasswords, L"word_list", L"word_list_100000.txt", 100000);
    writePasswordsToFileSubset(&passwords, &leetPasswords, L"word_list", L"word_list_1000000.txt", 1000000);


    // Free memory
    freeWCharArray(&words);
    freeWCharArray(&numbers);
    freeWCharArray(&specialChars);
    freeWCharArray(&passwords);
    freeWCharArray(&patterns);
    freeWCharArray(&leetPasswords);

    return 0;
}
