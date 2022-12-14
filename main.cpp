#include <iostream>
#include <fstream>
#include "queue.h"
#include "linearlist.h"

using namespace std;

const int numWords = 5757;

struct Word {
    string
        value;
    int
        ptr{};
    LinearList<int>
        list;
};

Queue<int>
        wordQueue;

Word wordList[numWords];

// Read in all the word from the .txt file and stores them into an array of type Word
// Sets default values for each field of the Word struct
void createWordList() {
    string
        fileName = "sgb-words.txt";
    ifstream
        wordsFile;

    wordsFile.open(fileName);
    if (!wordsFile) {
        throw runtime_error("Error: " + fileName + " could not be opened");
    }

    for (auto & word : wordList) {
        wordsFile >> word.value;
        word.ptr = -1;
        word.list.clear();
    }
    wordsFile.close();
}

// Checks wether the two words have a difference of one letter/character
// If there is only a one letter difference, return true
bool adjacent(const string& _word, const string& _adjacentWord) {
    int
        i = 0, hammingDistance = 0;

    while(_word[i] != '\0') {
        if (_word[i] == _adjacentWord[i])
            hammingDistance++;
        i++;
    }
    return hammingDistance == 4;
}

// Loops through all the words in the word list
// Compares the word to all other words
// If the two words are adjacent, then add adjecent word to the word's list
void buildAdjacentList() {
    for (int i = 0; i < numWords; i++) {
        for (int j = 0; j < i; j++) {
            if (adjacent(wordList[i].value, wordList[j].value)) {
                wordList[j].list.insert(wordList[j].list.size(), i);
                wordList[i].list.insert(wordList[i].list.size(), j);
            }
        }
    }
}

// Searches for word inside wordlist
// If word is found, return true and update Word variable
// Else return false
bool search(const string& _string, int &_word) {
    for (int i = 0; i < numWords; i++) {
        if (wordList[i].value == _string) {
            _word = i;
            return true;
        }
    }
    return false;
}

// Generates word ladder given two words that are in the wordlist
// If there is a ladder, the ladder will be displayed
// Else, outputs not ladder
void genLadder(int w1, int w2) {
    int
        w,v;

    wordQueue.enqueue(w2);

    while (!wordQueue.isEmpty()) {
        w = wordQueue.dequeue();

        for (int i = 0; i < wordList[w].list.size(); i++) {
            v = wordList[w].list[i];
            if (wordList[v].ptr == -1 && wordList[v].value != wordList[w2].value) {
                wordList[v].ptr = w;
                wordQueue.enqueue(v);
            }
        }
    }

    if (wordList[w1].ptr != -1) {
        cout << wordList[w1].value << endl;
        w = wordList[w1].ptr;

        while (w != -1) {
            cout << wordList[w].value << endl;
            w = wordList[w].ptr;
        }
    } else {
        cout << "No Ladder Exist!" << endl;
    }
}

int main() {
    string
        stringOne, stringTwo;
    int
        wordOne, wordTwo;

    // Creates wordlist from .txt file
    // Build adjacent list for each word
    createWordList();
    buildAdjacentList();

    // Get input from user
    cout << "Provide two 5-letter words: ";
    cin >> stringOne >> stringTwo;

    // check that both words are in word list
    if (!search(stringOne, wordOne)) {
        cout << "Error: first word not in list" << endl;
        return 1;
    } else if (!search(stringTwo, wordTwo)) {
        cout << "Error: second word not in list" << endl;
        return 1;
    }

    // if so, send words into generate word ladder
    genLadder(wordOne, wordTwo);

    return 0;

}