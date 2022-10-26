

#include "Boggle.h"
#include "random.h"
#include "grid.h"
#include "shuffle.h"
#include "strlib.h"
#include "shuffle.h"
#include "bogglegui.h"


const int PAUSE = 100;
void initializeGUIBoard(string,Grid<char> &);

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};


//uses parameters to construct grid
Boggle::Boggle(Lexicon& w, string boardText) {
    if(!BoggleGUI::isInitialized()) BoggleGUI::initialize(4,4);
    dictionary = w;
    letterGrid = Grid<char> (4,4);
    humanScore = 0;
    computerScore = 0;
    humanWordSet = {};

    boardText = toUpperCase(boardText);



    int counter = 0;
    for(int r = 0; r < letterGrid.numRows(); r++) {
        for(int c = 0; c < letterGrid.numCols(); c++) {
            if(boardText != "") {
                letterGrid[r][c] = boardText[counter];

                counter++;
            } else {

                string cube = CUBES[counter];
                int index = randomInteger(0,5);
                letterGrid[r][c] = cube[index];
                counter++;
            }
        }
    }
    if(boardText.length() == 0) shuffle(letterGrid);
    initializeGUIBoard(boardText, letterGrid);
}



//labels GUI board
void initializeGUIBoard(string boardText,Grid<char> & letterGrid) {
    if(boardText != "") BoggleGUI::labelAllCubes(boardText);
    else {
        for(int r = 0; r < letterGrid.numRows(); r++) {
            for(int c = 0; c < letterGrid.numCols(); c++) {
                BoggleGUI::labelCube(r,c,letterGrid[r][c], false);
            }
        }
    }

}




//returns particular letter at the grid coordinate
char Boggle::getLetter(int row, int col) {
    return letterGrid[row][col];
}



//checks if word is viable ie if it is at least 4 characters and is valid in the dictionary
bool Boggle::checkWord(string word) {
    if(!dictionary.contains(word) || word.length() < 4 || humanWordSet.contains(word)) {
        string message = "You must enter an unfound 4+ letter word from the dictionary.";
        BoggleGUI::setStatusMessage(message);
        cout << message << endl;
        cout << "" << endl;
        return false;
    }
    return true;
}




//searches through grid recursively
//returns true if input was valid word in grid
bool Boggle::humanWordSearch(string word) {
    BoggleGUI::clearHighlighting();
    string wordSoFar = "";
    Grid<bool> isUsed (4,4);

    //explores each possibility on grid
    for(int r = 0; r < letterGrid.numRows(); r++) {
        for(int c = 0; c < letterGrid.numCols(); c++) {
            BoggleGUI::setHighlighted(r,c, true);
            if(letterGrid[r][c] == word[0]) {
                //choose
                wordSoFar += letterGrid[r][c];
                isUsed[r][c] = true;
                BoggleGUI::setAnimationDelay(PAUSE);
                BoggleGUI::setHighlighted(r,c, true);

                //explores each neighbor
                if(humanWordSearchWrapper(isUsed, word.substr(1), wordSoFar, r, c)) return true;
                //unchoose
                isUsed[r][c] = false;
                wordSoFar.clear();
                BoggleGUI::setAnimationDelay(PAUSE);
                BoggleGUI::setHighlighted(r,c, false);

            }
            BoggleGUI::setHighlighted(r,c, false);
        }

    }
    return false;

}


bool Boggle::humanWordSearchWrapper(Grid<bool> & isUsed, string word, string wordSoFar, int cr, int cc) {
    if(word == "" && !humanWordSet.contains(wordSoFar)){
        //3 is chosen b/c a 4-letter word is worth 1 point, 5-letter is 2 points, etc
        humanScore += wordSoFar.length() - 3;
        humanWordSet.add(wordSoFar);
        //updates GUI
        BoggleGUI::recordWord(wordSoFar, BoggleGUI::HUMAN);
        BoggleGUI::setScore(humanScore, BoggleGUI::HUMAN);
        return true;
    }else {
        if(dictionary.containsPrefix(wordSoFar)) {
            for(int r = -1; r <= 1; r++) {
                for(int c = -1; c <= 1; c++) {
                    int x = cr + r; //row neighbor
                    int y = cc + c; //column neighbor
                    if(letterGrid.inBounds(x,y) && !isUsed[x][y]) {
                        BoggleGUI::setHighlighted(x,y, true);
                        if(letterGrid[x][y] == word[0]) {
                            isUsed[x][y] = true;
                            wordSoFar += letterGrid[x][y];
                            BoggleGUI::setAnimationDelay(PAUSE);
                            BoggleGUI::setHighlighted(x,y, true);

                            //explores each neighbor
                            if(humanWordSearchWrapper(isUsed, word.substr(1), wordSoFar, x, y)) return true;

                            //unchoose
                            isUsed[x][y] = false;
                            wordSoFar = wordSoFar.substr(0, wordSoFar.length()-1);
                            BoggleGUI::setAnimationDelay(PAUSE);
                            BoggleGUI::setHighlighted(x,y, false);
                        }
                        BoggleGUI::setHighlighted(x,y, false);
                    }
                }
            }
        }
    }
    return false;
}









//returns set that contains all of human entries
string Boggle::getHumanWords() {
    return humanWordSet.toString();
}
//returns size of that set
int Boggle::getNumHumanWords() {
    return humanWordSet.size();
}

//returns score of human
int Boggle::getScoreHuman() {
    return humanScore;
}



Set<string> Boggle::computerWordSearch() {
    Set<string> computerWords;
    string word;
    Grid<bool> isUsed (4,4);

    for(int r = 0; r < letterGrid.numRows(); r++) {
        for(int c = 0; c < letterGrid.numCols(); c++) {
            //choose
            word+= letterGrid[r][c];
            isUsed[r][c] = true;
            //explore
            computerWordSearchWrapper(computerWords, isUsed, word, r, c);
            //unchoose
            isUsed[r][c] = false;
            word.clear();
        }
    }
    return computerWords;
}




//recursively backtracks all possible words and adds them to score
void Boggle::computerWordSearchWrapper(Set<string>&computerWords, Grid<bool>& isUsed, string word, int pr, int pc) {
    if(dictionary.contains(word) && word.length() > 3 && !computerWords.contains(word) && !humanWordSet.contains(word)) {
        computerWords.add(word);
        computerScore += word.length() - 3;
        //updates GUI
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
        BoggleGUI::setScore(computerScore, BoggleGUI::COMPUTER);
        return;
    } else {
        if(dictionary.containsPrefix(word)) {
            //choose
            for(int sr = -1; sr <= 1; sr++) {
                for(int sc = -1; sc <= 1; sc++) {
                    int r = pr + sr;
                    int c = pc + sc;
                    //explore
                    if(letterGrid.inBounds(r,c) && !isUsed[r][c]) {
                        word+=letterGrid[r][c];
                        isUsed[r][c] = true;
                        computerWordSearchWrapper(computerWords, isUsed, word, r, c);

                        //unchoose
                        isUsed[r][c] = false;
                        word = word.substr(0, word.length()-1);
                    }

                }
            }
        }

    }
    return;

}



int Boggle::getScoreComputer() {
    return computerScore;
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    for(int r = 0; r < 4; r++) {
        string row;
        for(int c = 0; c < 4; c++) {
            row += boggle.getLetter(r,c);
        }
        cout << row << endl;
    }
        cout << "" << endl;
    return out;
}
