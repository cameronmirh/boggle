// This is a .h file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman();
    int getScoreComputer();
    Set<string> getHumanWordSet();

    // TODO: add any other member functions/variables necessary
    friend ostream& operator<<(ostream& out, Boggle& boggle);
    bool humanWordSearchWrapper(Grid<bool>&, string, string, int, int);
    void computerWordSearchWrapper(Set<string>&,  Grid<bool>&, string, int, int);
    int getNumHumanWords();
    string getHumanWords();

private:
    Grid<char> letterGrid;
    int humanScore;
    int computerScore;
    Set<string> humanWordSet;

    Lexicon dictionary;
};

#endif // _boggle_h
