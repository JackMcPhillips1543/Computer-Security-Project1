#include <iostream>
#include <string>
#include <array>
#include <cctype>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include <functional>
#include <utility>
#include <map>

std::string normalDistribution = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
std::unordered_set<std::string> wordList;
std::map<char, char> key;

// Reads the ciphertext from file
std::string readCiphertext() {
    std::ifstream file("ProjectDoc/ciphertext.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return "";
    }
    return std::string((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
}

// Reads the dictionary from file
std::unordered_set<std::string> createWordList(){
    std::unordered_set<std::string> dictionary;
    std::ifstream file("ProjectDoc/dictionary.txt");
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return dictionary;
    }
    std::string word;
    while (std::getline(file, word)) {
        dictionary.insert(word);
    }
    return dictionary;
}

// Comparator for sorting pairs by the second value (frequency)
bool comparePairs(const std::pair<char, int>& a, const std::pair<char, int>& b) {
    return a.second > b.second;
}

// Counts character frequencies
std::array<std::pair<char, int>, 26> getCharCount(const std::string& input) {
    std::array<std::pair<char, int>, 26> charCounts = {};
    for (int i = 0; i < 26; ++i) {
        charCounts[i] = std::make_pair('A' + i, 0);
    }

    for (char ch : input) {
        if (std::isalpha(ch)) {
            ++charCounts[std::toupper(ch) - 'A'].second;
        }
    }

    std::sort(charCounts.begin(), charCounts.end(), comparePairs);

    // Debug output: Char counts
    for (const auto& pair : charCounts) {
        std::cout << pair.first << ": " << pair.second << ", ";
    }

    return charCounts;
}

// Counts the number of occurrences of a keyword in text
int countKeywordOccurrences(const std::string& text, const std::string& keyword) {
    int count = 0;
    size_t pos = text.find(keyword);
    while (pos != std::string::npos) {
        ++count;
        pos = text.find(keyword, pos + keyword.size());
    }
    return count;
}

// Scores a key based on occurrences in the word list
int getKeyScore(const std::string& input) {
    int keyScore = 0;
    for (const auto& word : wordList) {
        keyScore += word.size() * word.size() * countKeywordOccurrences(input, word);
    }
    return keyScore;
}

// Swaps the mapping of two characters in the key map
void keySwap(char firstCh, char secondCh) {
    auto index1 = key.find(firstCh);
    auto index2 = key.find(secondCh);

    if (index1 != key.end() && index2 != key.end()) {
        std::swap(index1->second, index2->second);
    }
}

// Mutates a string by swapping occurrences of two characters
std::string mutateString(char firstCh, char secondCh, std::string& input) {
    for (char& ch : input) {
        if (ch == std::toupper(firstCh)) ch = secondCh;
        else if (ch == std::toupper(secondCh)) ch = firstCh;
    }
    keySwap(firstCh, secondCh);
    return input;
}

// Matches character frequencies to a normal distribution and updates the key map
void matchNormalDistribution(const std::string& normalDistribution, const std::array<std::pair<char, int>, 26>& charCounts) {
    for (int i = 0; i < normalDistribution.size(); ++i) {
        key[charCounts[i].first] = normalDistribution[i];
    }
}

// Prints the current key mappings
void printKey() {
    std::cout << "Current Key: " << std::endl;
    for (const auto& pair : key) {
        std::cout << pair.second;
    }
}

// Main function
int main() {
    wordList = createWordList();
    std::string input = readCiphertext();
    std::string toSolve = input;
    int maxScore = 0;
    bool solving = true;
    std::pair<char, char> toSwap;

    std::cout << "Input cipher:\n" << input << '\n' << std::endl;
    std::cout << "Original Char Counts\n";
    std::array<std::pair<char, int>, 26> OGcharCounts = getCharCount(input);

    matchNormalDistribution(normalDistribution, OGcharCounts);

    for (char& ch : toSolve) {
        if (std::isalpha(ch)) ch = key[ch];
    }

    std::cout << "\nMatched cipher:\n" << toSolve << std::endl;
    maxScore = getKeyScore(toSolve);
    std::cout << "\nScore: " << maxScore << std::endl;

    printKey();

    while(solving) {
        std::cout << "\nHighest Score: " << maxScore << "\nCurrent Cipher Text:\n" << toSolve << std::endl;
        std::cout << "\nWhat letters would you like to swap (e.g., A B): ";
        std::cin >> toSwap.first >> toSwap.second;
        toSolve = mutateString(toSwap.first, toSwap.second, toSolve);
        int newScore = getKeyScore(toSolve);
        std::cout << "\nNew Score: " << newScore << std::endl;

        if (newScore > maxScore) {
            maxScore = newScore;
            std::cout << "Score increased. This is closer to being deciphered." << std::endl;
        }

        printKey();
        std::cout << "\nNew Cipher Text:\n" << toSolve << '\n' << std::endl;
        std::cout << "Current Char Counts: " << std::endl;
        getCharCount(toSolve);

        char cont;
        std::cout << "\nSwap another letter/continue (y/n): ";
        std::cin >> cont;

        if (cont == 'n') {
            solving = false;
        }
    }

    std::cout << "\nFinal Plaintext: \n" << toSolve << std::endl;
    printKey();

    return 0;
}