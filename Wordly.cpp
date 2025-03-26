#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <thread>
#include <chrono>
#include <algorithm>
#include <vector>
#include <fstream>

using namespace std;

vector<string> loadWordList(const string& filename);
vector<string> wordList = loadWordList("wordlist.txt");

void typeEffect(const string& text, int speed = 50) {
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(speed));
    }
    cout << endl;
    cout.flush();
}

string pickRandomWord() {
    if (wordList.empty()) {
        return "ERROR";  // handle empty list
    }
    
    srand(time(0));
    return wordList[rand() % wordList.size()];
}

void displayFeedback(const string& guess, const string& correctWord) {
    string feedback = "";
    vector<bool> used(correctWord.size(), false);

    for (size_t i = 0; i < guess.size(); i++) {
        if (guess[i] == correctWord[i]) {
            feedback += "[" + string(1, guess[i]) + "]"; // correct
            used[i] = true;
        } else {
            feedback += "_"; 
        }
    }
    
    for (size_t i = 0; i < guess.size(); i++) {
        if (feedback[i] == '_') {
            bool found = false;
            for (size_t j = 0; j < correctWord.size(); j++) {
                if (!used[j] && guess[i] == correctWord[j]) {
                    feedback[i] = '*'; // wrong position
                    used[j] = true;
                    found = true;
                    break;
                }
            }
            if (!found) {
                feedback[i] = 'X'; // incorrect
            }
        }
    }

    typeEffect("Feedback: " + feedback);
}

void wordly() {
    string correctWord = pickRandomWord();
    int attempts = 6;
    typeEffect("[ACCESS DENIED] Enter the correct password");

    while (attempts > 0) {
        cout << "Enter your guess: ";
        string guess;
        cin >> guess;
        transform(guess.begin(), guess.end(), guess.begin(), ::toupper);

        if (find(wordList.begin(), wordList.end(), guess) == wordList.end()) {
            typeEffect("It's not a word.");
            continue;
        }

        if (guess == correctWord) {
            typeEffect("ACCESS GRANTED! System unlocked.");
            return;
        }
        displayFeedback(guess, correctWord);
        typeEffect("Attempts left: " + to_string(--attempts));
    }

    // show the correct word after failure
    typeEffect("[LOCKED OUT] System shutting down...");
    typeEffect("The correct word was: " + correctWord);
}

vector<string> loadWordList(const string& filename) {
    vector<string> words;
    ifstream file(filename);
    string word;

    if (!file) {
        cerr << "Error: Unable to open word list file" << endl;
        return words;  
    }

    while (getline(file, word)) {
        transform(word.begin(), word.end(), word.begin(), ::toupper);  // uppercase
        words.push_back(word);
    }

    file.close();
    return words;
}

int main() {
    srand(time(0));
    cout.sync_with_stdio(false);
    system("clear"); // if you're on Windows use "cls"
    typeEffect("*** WELCOME TO WORDLY ***");
    typeEffect("Guess the 5-letter word in 6 tries! [L] = correct, * = wrong spot, X = not in word.");
    this_thread::sleep_for(chrono::seconds(2));
    
    wordly();
    
    return 0;
}
