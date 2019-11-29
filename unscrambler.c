#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <inttypes.h>

// Converts a string into lowercase letters; (ex. "FOOBAR" => "foobar")
void lowercaseifyString(char* str);
// Compares two words and tests if w1 could be the scrambled spelling of w2
// Ex.  compareWords("FOO", "OOF") => true
//      compareWords("FOO", "OFO") => true
//      compareWords("FOO", "BAR") => false
bool compareWords(char* w1, char* w2);
// Compares the given word with every word given in the dictionary file
void unscrambleWord(char* word);
// Sorts words by most used
// \param result, is an array of integers that determines the order of words, from most popular to least popularL
void sortWordsByPop(char* words[], const int wordCount, int* result);

void sortWordsByPop(char* words[], const int wordCount, int* result) {
    FILE* poplist = fopen("./20k.txt", "r");
    if (!poplist) { perror("fopen"); exit(0); }

    char buffer[128];
    int wordPopularityIndexes[wordCount];
    uint64_t currentWordIndex = 0;
    int j = 0;

    for (int i = 0; i < wordCount; i++) {
        wordPopularityIndexes[i] = 0;
        result[i] = -1;
    }

    while (fgets(buffer, 128, poplist)) {
        for (int x = 0; x < strlen(buffer); x++) if (buffer[x] == '\n') buffer[x] = '\0';
        lowercaseifyString(buffer);
        // Compare current line with every word
        for (int i = 0; i < wordCount; i++) {
            lowercaseifyString(words[i]);
            if (strlen(words[i]) == strlen(buffer) &&
                strcmp(buffer, words[i]) == 0) {
                result[j++] = i;
                continue;
            }
        }
    }

    fclose(poplist);
}

void lowercaseifyString(char* str) {
    for (int i = 0; i < strlen(str); i++) {
        str[i] = tolower(str[i]);
    }
}

bool compareWords(char* w1, char* w2) {
#ifdef _DEBUG
    printf("Comparing %s and %s...\n", w1, w2);
#endif
    lowercaseifyString(w1);
    lowercaseifyString(w2);
    for (int i = 0; i < strlen(w1); i++) {
        int w1count, w2count;
        w1count = w2count = 0;

        for (int j = 0; j < strlen(w1); j++) {
            if (w1[i] == w1[j]) w1count++;
        }

        for (int j = 0; j < strlen(w2); j++) {
            if (w1[i] == w2[j]) w2count++;
        }

        if (w2count != w1count) {
        #ifdef _DEBUG
            printf("%s and %s do NOT match.\n\n", w1, w2);
        #endif
            return false;
        }
    }

#ifdef _DEBUG
    printf("%s and %s match.\n\n", w1, w2);
#endif
    return true;
}

void unscrambleWord(char* word) {
    FILE* dictionary = fopen("./enable1.txt", "r");
    if (!dictionary) { perror("fopen"); exit(1); }

    char buffer[128];
    char* possibleMatches[64] = { NULL };
    int nMatches = 0;

    while (fgets(buffer, 128, dictionary)) {
        for (int i = 0; i < strlen(buffer); i++) if (buffer[i] == '\n') buffer[i] = '\0';
        if (strlen(buffer) != strlen(word)) continue;
        if (compareWords(word, buffer) == true) {
            possibleMatches[nMatches] = (char*)malloc(sizeof(char) * strlen(buffer));
            strcpy(possibleMatches[nMatches], buffer);
            nMatches++;
        }
    }

    int order[nMatches];
    sortWordsByPop(possibleMatches, nMatches, order);
    int x = 0;

    for (int i = 0; i < nMatches; i++) {
        if (order[i] != -1) {
            printf("#%i: Possible match: \"%s\"\n", ++x, possibleMatches[order[i]]);
        }
    }
    
    if (x < nMatches) {
        printf("=== Uncommon Words ===\n");
        for (int i = 0; i < nMatches; i++) {
            bool isUncommon = true;
            for (int j = 0; j < nMatches; j++) {
                if (order[j] == i) {
                    isUncommon = false;
                    break;
                }
            }
            if (isUncommon) {
                printf("#%i: Uncommon possible match: \"%s\"\n", ++x, possibleMatches[i]);
            }
        }
    }

    for (int i = 0; i < nMatches; i++) {
        free(possibleMatches[i]);
    }

    printf("\n");

    fclose(dictionary);
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        char buffer[128];
        printf("Usage: %s [SCRAMBLED WORD]\n", argv[0]);
        puts("Finds words that are possibly the unscrambled version of the input word.");
        printf("> ");
        fflush(stdout);
        while (read(1, buffer, 128)) {
            for (int i = 0; i < strlen(buffer); i++) if (buffer[i] == '\n') buffer[i] = '\0';
            unscrambleWord(buffer);
            printf("> ");
            fflush(stdout);
        }
    } else {
        unscrambleWord(argv[1]);
    }

    return 0;
}

