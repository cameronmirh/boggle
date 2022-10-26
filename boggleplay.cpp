

#include "lexicon.h"
#include "Boggle.h"
#include "simpio.h"
#include "console.h"
#include "bogglegui.h"

bool checkInput(Lexicon&, string);
int humanTurn(Boggle&);
void resetConsole(Boggle&);
int computerTurn(Boggle&);
void printResponse(int, string);





void playOneGame(Lexicon& dictionary) {
    string boardText = "";

    if(!getYesOrNo("Do you want to generate a random board? ")) {
        boardText = getLine("Type the 16 letters to appear on the board: ");
        while(!checkInput(dictionary, boardText)) {
            boardText = getLine("Type the 16 letters to appear on the board: ");
        }
    }
    Boggle boggleGame = Boggle(dictionary, boardText);
    clearConsole();
    printResponse(1, "");
    resetConsole(boggleGame);

    int humanScore = humanTurn(boggleGame);
    int computerScore = computerTurn(boggleGame);

    if(humanScore > computerScore) {
        printResponse(5, "");
    } else {
        printResponse(6, "");
    }

}

//prints out computer score and words it finds
int computerTurn(Boggle& boggleGame) {
    Set<string> computerWordSet = boggleGame.computerWordSearch();
    cout << "" << endl;
    printResponse(1, "");
    cout << "My words (" << computerWordSet.size() << "): " << computerWordSet.toString() << endl;
    cout << "My Score: " << boggleGame.getScoreComputer() << endl;
    return boggleGame.getScoreComputer();

}


//while the human still wants to play, will get word from grid and print appropriate responses
int humanTurn(Boggle& boggleGame) {
    while(true) {
        string word = toUpperCase(getLine("Type a word (or Enter to stop): "));
        //to quit
        if(word == "") break;
        //to check word
        while(!boggleGame.checkWord(word)) {
            clearConsole();
            printResponse(2, "");
            resetConsole(boggleGame);
            word = toUpperCase(getLine("Type a word (or Enter to stop): "));
            //must check word again
            if(word == "") break;
        }
        if(word == "") break;

        clearConsole();
        if(boggleGame.humanWordSearch(word)) {
            printResponse(3, word);
            resetConsole(boggleGame);
        } else {
            printResponse(4, "");
            resetConsole(boggleGame);
        }
    }
    return boggleGame.getScoreHuman();
}



//uses switch statement to print appropriate statements in both console and GUI
void printResponse(int n, string word) {
    string message;
    switch (n) {
    case 1: message = "It's your turn!";
        break;
    case 2: message = "You must enter an unfound 4+ letter word from the dictionary.";
        break;
    case 3: message = "You found a new word! \"" + word + "\"";
        break;
    case 4: message = "That word can't be formed on this board.";
        break;
    case 5: message = "WOW, you defeated me! Congratulations!";
        break;
    case 6: message = "Ha ha ha, I destroyed you. Better luck next time, puny human!";
        break;
    default:
        message = "";
        break;
    }
    BoggleGUI::setStatusMessage(message);
    cout << message << endl;
}



//prints out boggle grid
//prints out score
//print out set of words
void resetConsole(Boggle& boggleGame) {
    //possible switch case that takes in number and correlates with word
    cout << boggleGame << endl;
    cout << "Your words" << " (" << boggleGame.getNumHumanWords() << "): " << boggleGame.getHumanWords() << endl;
    cout << "Your score: " << boggleGame.getScoreHuman() << endl;
}





//checks if the input is valid by:
//  checking length
//  checking each index to see if it is a character or not
bool checkInput(Lexicon& dictionary, string boardText) {

    //check for invalid input
    int counter = 0;
    for(int i = 0; i < boardText.length(); i++) {
        if(!isalpha(boardText[i])) {
            cout << "That is not a valid 16-letter board string. Try again." << endl;
            cout << "" << endl;
            return false;
        }
        counter++;

    }
    if(counter != 16) {
        cout << "That is not a valid 16-letter board string. Try again." << endl;
        return false;
    }

    return true;


}
