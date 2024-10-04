#include <iostream>
#include <windows.h>
#include <string>
#include <random>
#include <chrono>
#include<ctime>
#include<cstdlib>
#include <stdexcept>

using namespace std;

void setConsoleColor(WORD color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

class Card {
private:
    string suit;
    int rank;
    bool faceUp;

public:
    Card(string s = "", int r = 0, bool f = false) {
        suit = s;
        rank = r;
        faceUp = f;
    }

    void toggleFace() {
        faceUp = !faceUp;
    }

    bool isValidMove(const Card& obj) const {
        bool isThisRed = (this->suit == "Heart" || this->suit == "Diamond");
        bool isObjRed = (obj.suit == "Heart" || obj.suit == "Diamond");
        return (this->rank == obj.rank - 1) && (isThisRed != isObjRed);
    }

    bool isFaceUp() const { return faceUp; }
    int getRank() const { return rank; }
    string getSuit() const { return suit; }

    void printCard() const {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        if (!faceUp) {
            cout << "[*]";
        }
        else {
            string rankStr = to_string(rank);
            if (rank == 1) rankStr = "A";  // Ace
            else if (rank == 11) rankStr = "J"; // Jack
            else if (rank == 12) rankStr = "Q"; // Queen
            else if (rank == 13) rankStr = "K"; // King

            SetConsoleOutputCP(CP_UTF8);

            if (suit == "Heart") {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                cout << "[" << rankStr << " " << "\u2665" << "<< ]";
            }
            else if (suit == "Diamond") {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                cout << "[" << rankStr << " " << "\u2666" << "]";
            }
            else if (suit == "Spade") {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                cout << "[" << rankStr << " " << "\u2660" << "]";
            }
            else if (suit == "Club") {
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                cout << "[" << rankStr << " " << "\u2663" << "]";
            }

            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        }
    }
};

class Node {
public:
    Card data;
    Node* next;
    Node* prev;

    Node(Card c) {
        data = c;
        next = prev = nullptr;
    }
};
template<typename T>
class List {
private:
    Node* head;
    Node* tail;
    int size;

public:
    List() {
        head = tail = nullptr;
        size = 0;
    }

    ~List() {
        // Clear the list
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
        head = tail = nullptr;
        size = 0;
    }
    T& get(int index) {
        Node* current = head;
        int count = 0;
        while (current != nullptr && count < index) {
            current = current->next;
            count++;
        }
        return current->data;
    }
    void push(const Card& value) {
        Node* nod = new Node(value);
        nod->prev = tail;
        nod->next = nullptr;

        if (tail != nullptr) {
            tail->next = nod;
        }
        else {
            head = nod;
        }

        tail = nod;
        size++;
    }

    Card& at(int index) {
        if (index < 0 || index >= size) {
            throw out_of_range("Out of bounds");
        }
        Node* current = head;
        for (int i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    Card pop() {
        if (tail == nullptr) {
            throw out_of_range("List is empty");
        }
        Node* oldTail = tail;
        Card value = tail->data;
        tail = tail->prev;
        if (tail) {
            tail->next = nullptr;
        }
        else {
            head = nullptr;
        }
        delete oldTail;
        --size;
        return value;
    }

    int getSize() const {
        return size;
    }

    bool isEmpty() const {
        return size == 0;
    }

    void printAllCards() const {
        Node* current = head;
        if (isEmpty()) {
            cout << "[empty]";
            cout << endl;
            return;
        }

        while (current != nullptr) {
            current->data.printCard();
            cout << " ";
            current = current->next;
        }

        cout << endl;
    }

    Node* getHead() const { return head; }
};

class Deck {
private:
    List<int> deckList;
    Card deck[52];
    int size;

public:


    Deck() {
        
        initializeDeck();
        shuffle();
        size = 52;
    }

    void initializeDeck() {
        string suits[] = { "Heart", "Diamond", "Club", "Spade" };
        int index = 0;
        for (int i = 0; i < 4; i++) {
            for (int rank = 1; rank <= 13; ++rank) {
                deck[index++] = Card(suits[i], rank, false);
            }
        }
    }
    void shuffle() {
        // Use random_device to seed the mt19937 random number generator
        std::random_device rd;
        std::mt19937 rng(rd());  // Mersenne Twister engine seeded with random_device

        std::shuffle(deck, deck + 52, rng);  // Use std::shuffle to shuffle the deck
    }
    bool empty() const {
        return size == 0;
    }

    Card draw() {
        if (empty()) throw out_of_range("Deck is empty");
        return deck[--size];
    }
};

template<typename T>
class Stack {
private:
    class Node {
    public:
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };
    Node* topNode;
    int size;

public:
    Stack() : topNode(nullptr), size(0) {}

    ~Stack() {
        clear();
    }

    void push(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = topNode;
        topNode = newNode;
        ++size;
    }

    T pop() {
        if (isEmpty()) {
            throw out_of_range("Stack is empty");
        }
        Node* oldTop = topNode;
        T value = topNode->data;
        topNode = topNode->next;
        delete oldTop;
        --size;
        return value;
    }

    T& top() const {
        if (isEmpty()) {
            throw out_of_range("Stack is empty");
        }
        return topNode->data;
    }

    bool isEmpty() const {
        return size == 0;
    }

    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }

    int getSize() const {
        return size;
    }
};

class Tableau {
public:
    List<Card> columns[7];


    void initialize(Deck& deck) {
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j <= i; j++) {
                Card drawn = deck.draw();
                if (i == j) {
                    drawn.toggleFace();
                }
                columns[i].push(drawn);
            }
        }
    }

    void pushCard(int index, const Card& card) {
        if (index < 0 || index >= 7) {
            throw out_of_range("Invalid tableau index");
        }
        columns[index].push(card);
    }

    Card popCard(int index) {
        if (index < 0 || index >= 7 || columns[index].isEmpty()) {
            throw out_of_range("Invalid tableau index");
        }
        return columns[index].pop();
    }

    Card& topCard(int index) {
        if (index < 0 || index >= 7 || columns[index].isEmpty()) {
            throw out_of_range("Invalid tableau index");
        }
        return columns[index].at(columns[index].getSize() - 1);
    }

    int getSize(int index) const {
        if (index < 0 || index >= 7) {
            throw out_of_range("Invalid tableau index");
        }
        return columns[index].getSize();
    }

    bool isEmpty(int index) const {
        if (index < 0 || index >= 7) {
            throw out_of_range("Invalid tableau index");
        }
        return columns[index].isEmpty();
    }

    void printTableau() {
        // Find the maximum height among the columns.
        int maxHeight = 0;
        for (int i = 0; i < 7; i++) {
            if (columns[i].getSize() > maxHeight) {
                maxHeight = columns[i].getSize();
            }
        }

        // Print cards row by row for each column.
        for (int row = 0; row < maxHeight; row++) {
            for (int col = 0; col < 7; col++) {
                // Check if the column has a card at the current row index.
                if (row < columns[col].getSize()) {
                    try {
                        columns[col].at(row).printCard();  // Access card and print it.
                    }
                    catch (const out_of_range& e) {
                        cout << "[Error] " << e.what();  // Handle out-of-bounds access.
                    }
                }
                else {
                    cout << "     ";
                }
                cout << "             ";
            }
            

            cout << endl;  // Newline after each row.
        }
    }
};

class Foundation {
public:
    Stack<Card> stacks[4];

public:
    void pushCard(int index, const Card& card) {
        if (index < 0 || index >= 4) {
            throw out_of_range("Invalid foundation index");
        }
        stacks[index].push(card);
    }

    Card popCard(int index) {
        if (index < 0 || index >= 4 || stacks[index].isEmpty()) {
            throw out_of_range("Invalid foundation index");
        }
        return stacks[index].pop();
    }

    Card& topCard(int index) {
        if (index < 0 || index >= 4 || stacks[index].isEmpty()) {
            throw out_of_range("Invalid foundation index");
        }
        return stacks[index].top();
    }

    int getSize(int index) const {
        if (index < 0 || index >= 4) {
            throw out_of_range("Invalid foundation index");
        }
        return stacks[index].getSize();
    }

    bool isEmpty(int index) const {
        if (index < 0 || index >= 4) {
            throw out_of_range("Invalid foundation index");
        }
        return stacks[index].isEmpty();
    }

    void printFoundation() const {
        for (int i = 0; i < 4; i++) {
            if (!stacks[i].isEmpty()) {
                stacks[i].top().printCard();
            }
            else {
                cout << "[    ]";
            }
            cout << "          ";
        }
        cout << endl;
    }
};

class StockPile {
public:
    Stack<Card> stockPile;
    void initialize(Deck& deck) {
        while (!deck.empty()) {
            stockPile.push(deck.draw());
        }
    }

    void pushCard(const Card& card) {
        stockPile.push(card);
    }

    Card popCard() {
        if (stockPile.isEmpty()) {
            throw out_of_range("Stock pile is empty");
        }
        return stockPile.pop();
    }

    Card& topCard() {
        if (stockPile.isEmpty()) {
            throw out_of_range("Stock pile is empty");
        }
        return stockPile.top();
    }

    bool isEmpty() const {
        return stockPile.isEmpty();
    }

    int getSize() const {
        return stockPile.getSize();
    }

    void resetStock(Stack<Card>& wastePile) {
        while (!wastePile.isEmpty()) {
            Card card = wastePile.pop();
            if (card.isFaceUp()) {
                card.toggleFace();
            }
            stockPile.push(card);
        }
    }

    void printStockPile() const {
        if (!stockPile.isEmpty()) {
            stockPile.top().printCard();
        }
        else {
            cout << "[   ]";
        }
        cout << "       ";
    }
};

class WastePile {
public:
    Stack<Card> wastePile;

public:
    void pushCard(const Card& card) {
        wastePile.push(card);
    }

    Card popCard() {
        if (wastePile.isEmpty()) {
            throw out_of_range("Waste pile is empty");
        }
        return wastePile.pop();
    }

    Card& topCard() {
        if (wastePile.isEmpty()) {
            throw out_of_range("Waste pile is empty");
        }
        return wastePile.top();
    }

    bool isEmpty() const {
        return wastePile.isEmpty();
    }

    int getSize() const {
        return wastePile.getSize();
    }

    void printWastePile() const {
        if (!wastePile.isEmpty()) {
            wastePile.top().printCard();
        }
        else {
            cout << "[   ]";
        }
        cout << "                        ";
    }
};

class Command {
public:
    char cmdType;
    char srcType;
    int srcInd;
    char destType;
    int destInd;
    int numCards;

    Command() {
        cmdType = srcType = destType = 0;
        srcInd = destInd = numCards = 0;
    }
};

class Game {
private:
    StockPile stockPile;
    WastePile wastePile;
    Stack<Command> undoStack;
    Foundation foundation;
    Tableau tableau;
    bool gameOver;
    int totalMoves;

    void initializeGame() {
        Deck deck;
        deck.shuffle();

        tableau.initialize(deck);
        stockPile.initialize(deck);

        if (!stockPile.isEmpty()) {
            Card topCard = stockPile.topCard();
            topCard.toggleFace();
            stockPile.popCard();
            stockPile.pushCard(topCard);
        }
    }

    int getMaxHeight() {
        int maxHeight = 0;
        for (int i = 0; i < 7; i++) {
            if (tableau.getSize(i) > maxHeight) {
                maxHeight = tableau.getSize(i);
            }
        }
        return maxHeight;
    }

    void printGameState() {
        cout << "Stock       Waste" << "                      " << "Foundation1" << "     " << "Foundation2" << "     " << "Foundation3" << "     " << "Foundation4" << endl;

        stockPile.printStockPile();
        wastePile.printWastePile();
        foundation.printFoundation();

        cout << " " << stockPile.getSize() << "          " << wastePile.getSize() << endl;
        cout << "\n\n\n\n\n                                               Tableau\n\n\n\n\n";
        tableau.printTableau();
    }

    Command parseCommand(const string& input) {
        Command cmd;
        int i = 0;
        int n = input.length();
        string token = "";

        while (i < n && input[i] == ' ') {
            i++;
        }

        while (i < n && input[i] != ' ') {
            token += tolower(input[i]);
            i++;
        }

        if (token == "move" || token == "m") {
            cmd.cmdType = 'm';

            while (i < n && input[i] == ' ') {
                i++;
            }

            if (i + 1 >= n || (input[i] != 't' && input[i] != 'w' && input[i] != 'f') || !isdigit(input[i + 1])) {
                throw invalid_argument("Invalid source");
            }
            cmd.srcType = input[i];
            cmd.srcInd = input[i + 1] - '0' - 1;
            i += 2;

            while (i < n && input[i] == ' ') {
                i++;
            }

            if (i + 1 >= n || (input[i] != 't' && input[i] != 'f') || !isdigit(input[i + 1])) {
                throw invalid_argument("Invalid destination");
            }
            cmd.destType = input[i];
            cmd.destInd = input[i + 1] - '0' - 1;
            i += 2;

            while (i < n && input[i] == ' ') {
                i++;
            }

            if (i < n && isdigit(input[i])) {
                int numCards = 0;
                while (i < n && isdigit(input[i])) {
                    numCards = numCards * 10 + (input[i] - '0');
                    i++;
                }
                if (numCards <= 0) {
                    throw invalid_argument("Invalid number of cards");
                }
                cmd.numCards = numCards;
            }
            else {
                cmd.numCards = 1;
            }
        }
        else if (token == "draw" || token == "d") {
            cmd.cmdType = 'd';
        }
        else if (token == "undo" || token == "u") {
            cmd.cmdType = 'u';
        }
        else {
            throw invalid_argument("Invalid command");
        }

        return cmd;
    }

    bool isValidMove(const Card& sourceCard, char destType, List<Card>* destinationList, Stack<Card>* destinationStack) {
        if (destType == 't') {
            if (destinationList->isEmpty()) {
                return (sourceCard.getRank() == 13);
            }
            else {
                return sourceCard.isValidMove(destinationList->at(destinationList->getSize() - 1));
            }
        }
        else if (destType == 'f') {
            if (destinationStack->isEmpty()) {
                return (sourceCard.getRank() == 1);
            }
            else {
                Card& topFoundationCard = destinationStack->top();
                return (sourceCard.getSuit() == topFoundationCard.getSuit() &&
                    sourceCard.getRank() == topFoundationCard.getRank() + 1);
            }
        }
        return false;
    }

    void moveCards(const Command& cmd) {
        Card sourceCard;
        List<Card>* sourceList = nullptr;
        Stack<Card>* sourceStack = nullptr;
        List<Card>* destinationList = nullptr;
        Stack<Card>* destinationStack = nullptr;

        if (cmd.srcType == 't') {
            if (cmd.srcInd < 0 || cmd.srcInd >= 7 || tableau.isEmpty(cmd.srcInd)) {
                throw invalid_argument("Invalid source tableau");
            }
            sourceList = &tableau.columns[cmd.srcInd];
            if (cmd.numCards > sourceList->getSize()) {
                throw invalid_argument("Not enough cards in source tableau");
            }
            sourceCard = sourceList->at(sourceList->getSize() - cmd.numCards);
            if (!sourceCard.isFaceUp()) {
                throw invalid_argument("Source card is face down");
            }
        }
        else if (cmd.srcType == 'w') {
            if (wastePile.isEmpty()) {
                throw invalid_argument("Waste pile is empty");
            }
            sourceStack = &wastePile.wastePile;
            sourceCard = sourceStack->top();
            if (cmd.numCards != 1) {
                throw invalid_argument("Can only move one card from waste pile");
            }
        }
        else if (cmd.srcType == 'f') {
            if (cmd.srcInd < 0 || cmd.srcInd >= 4 || foundation.isEmpty(cmd.srcInd)) {
                throw invalid_argument("Invalid source foundation");
            }
            sourceStack = &foundation.stacks[cmd.srcInd];
            sourceCard = sourceStack->top();
            if (cmd.numCards != 1) {
                throw invalid_argument("Can only move one card from foundation");
            }
        }
        else {
            throw invalid_argument("Invalid source type");
        }

        if (cmd.destType == 't') {
            if (cmd.destInd < 0 || cmd.destInd >= 7) {
                throw invalid_argument("Invalid destination tableau");
            }
            destinationList = &tableau.columns[cmd.destInd];
        }
        else if (cmd.destType == 'f') {
            if (cmd.destInd < 0 || cmd.destInd >= 4) {
                throw invalid_argument("Invalid destination foundation");
            }
            destinationStack = &foundation.stacks[cmd.destInd];
            if (cmd.numCards != 1) {
                throw invalid_argument("Can only move one card to foundation");
            }
        }
        else {
            throw invalid_argument("Invalid destination type");
        }

        bool validMove = isValidMove(sourceCard, cmd.destType, destinationList, destinationStack);

        if (!validMove) {
            throw invalid_argument("Invalid move");
        }

        List<Card> tempList;
        for (int i = 0; i < cmd.numCards; i++) {
            Card card = (sourceList) ? sourceList->at(sourceList->getSize() - cmd.numCards + i) : sourceStack->top();
            tempList.push(card);
        }

        for (int i = 0; i < cmd.numCards; i++) {
            if (sourceList) {
                sourceList->pop();
            }
            else {
                sourceStack->pop();
            }
        }

        for (int i = 0; i < cmd.numCards; i++) {
            Card card = tempList.at(i);
            if (destinationList) {
                destinationList->push(card);
            }
            else {
                destinationStack->push(card);
            }
        }

        if (cmd.srcType == 't' && !sourceList->isEmpty()) {
            Card& newTopCard = sourceList->at(sourceList->getSize() - 1);
            if (!newTopCard.isFaceUp()) {
                newTopCard.toggleFace();
            }
        }

        undoStack.push(cmd);
        totalMoves++;

        checkGameCompletion();
    }

    void drawCardFromStock() {
        if (!stockPile.isEmpty()) {
            Card drawnCard = stockPile.popCard();
            drawnCard.toggleFace();
            wastePile.pushCard(drawnCard);

            Command drawCommand;
            drawCommand.cmdType = 'd';
            undoStack.push(drawCommand);

            totalMoves++;
        }
        else if (!wastePile.isEmpty()) {
            while (!wastePile.isEmpty()) {
                Card card = wastePile.popCard();
                if (card.isFaceUp()) {
                    card.toggleFace();
                }
                stockPile.pushCard(card);
            }

            Command resetCommand;
            resetCommand.cmdType = 'r';
            undoStack.push(resetCommand);

            totalMoves++;
        }
        else {
            cout << "Both stock and waste piles are empty!" << endl;
        }

        // Toggle the face of the top card in the waste pile if not empty
        if (!wastePile.isEmpty()) {
            if (!wastePile.topCard().isFaceUp()) {
                wastePile.topCard().toggleFace();
            }
        }
    }

    void undoLastMove() {
        if (undoStack.isEmpty()) {
            cout << "No moves to undo." << endl;
            return;
        }

        Command lastCommand = undoStack.pop();
        switch (lastCommand.cmdType) {
        case 'm': {
            List<Card>* sourceList = nullptr;
            Stack<Card>* sourceStack = nullptr;
            List<Card>* destinationList = nullptr;
            Stack<Card>* destinationStack = nullptr;

            if (lastCommand.destType == 't') {
                sourceList = &tableau.columns[lastCommand.destInd];
            }
            else if (lastCommand.destType == 'f') {
                sourceStack = &foundation.stacks[lastCommand.destInd];
            }

            if (lastCommand.srcType == 't') {
                destinationList = &tableau.columns[lastCommand.srcInd];
            }
            else if (lastCommand.srcType == 'w') {
                destinationStack = &wastePile.wastePile;
            }
            else if (lastCommand.srcType == 'f') {
                destinationStack = &foundation.stacks[lastCommand.srcInd];
            }

            List<Card> tempList;
            for (int i = 0; i < lastCommand.numCards; i++) {
                Card card = (sourceList) ? sourceList->pop() : sourceStack->pop();
                tempList.push(card);
            }

            for (int i = lastCommand.numCards - 1; i >= 0; i--) {
                Card card = tempList.at(i);
                if (destinationList) {
                    destinationList->push(card);
                }
                else {
                    destinationStack->push(card);
                }
            }

            if (lastCommand.srcType == 't' && !destinationList->isEmpty()) {
                int size = destinationList->getSize();
                if (size > lastCommand.numCards) {
                    Card& previousTopCard = destinationList->at(size - lastCommand.numCards - 1);
                    if (previousTopCard.isFaceUp()) {
                        previousTopCard.toggleFace();
                    }
                }
            }
            break;
        }
        case 'd':
            if (!wastePile.isEmpty()) {
                Card card = wastePile.popCard();
                card.toggleFace();
                stockPile.pushCard(card);
            }
            break;
        case 'r':
            while (!stockPile.isEmpty()) {
                Card card = stockPile.popCard();
                card.toggleFace();
                wastePile.pushCard(card);
            }
            break;
        }

        totalMoves--;
    }

    void processCommand(const Command& cmd) {
        switch (cmd.cmdType) {
        case 'm':
            moveCards(cmd);
            break;
        case 'd':
            drawCardFromStock();
            break;
        case 'u':
            undoLastMove();
            break;
        default:
            throw invalid_argument("Invalid command type");
        }
    }

    void checkGameCompletion() {
        bool allFoundationsFull = true;
        bool validMovesExist = false;

        for (int i = 0; i < 4; i++) {
            if (foundation.getSize(i) != 13) {
                allFoundationsFull = false;
                break;
            }
        }

        if (allFoundationsFull) {
            gameOver = true;
            cout << "Congratulations! You've won the game in " << totalMoves << " moves!" << endl;
            return;
        }

        // Check for valid moves in tableau
        for (int i = 0; i < 7; i++) {
            if (tableau.isEmpty(i)) continue;
            Card sourceCard = tableau.topCard(i);
            for (int j = 0; j < 7; j++) {
                if (i == j) continue;
                if (tableau.isEmpty(j)) {
                    if (sourceCard.getRank() == 13) {
                        validMovesExist = true;
                        return;
                    }
                }
                else {
                    Card destCard = tableau.topCard(j);
                    if (sourceCard.isValidMove(destCard)) {
                        validMovesExist = true;
                        return;
                    }
                }
            }
        }

        // Check for valid moves from tableau to foundation
        for (int i = 0; i < 7; i++) {
            if (tableau.isEmpty(i)) continue;
            Card sourceCard = tableau.topCard(i);
            for (int j = 0; j < 4; j++) {
                if (foundation.isEmpty(j)) {
                    if (sourceCard.getRank() == 1) {
                        validMovesExist = true;
                        return;
                    }
                }
                else {
                    Card topFoundationCard = foundation.topCard(j);
                    if (sourceCard.getSuit() == topFoundationCard.getSuit() &&
                        sourceCard.getRank() == topFoundationCard.getRank() + 1) {
                        validMovesExist = true;
                        return;
                    }
                }
            }
        }

        // Check for valid moves from waste pile
        if (!wastePile.isEmpty()) {
            Card wasteCard = wastePile.topCard();
            for (int i = 0; i < 7; i++) {
                if (tableau.isEmpty(i)) {
                    if (wasteCard.getRank() == 13) {
                        validMovesExist = true;
                        return;
                    }
                }
                else {
                    Card destCard = tableau.topCard(i);
                    if (wasteCard.isValidMove(destCard)) {
                        validMovesExist = true;
                        return;
                    }
                }
            }
            for (int i = 0; i < 4; i++) {
                if (foundation.isEmpty(i)) {
                    if (wasteCard.getRank() == 1) {
                        validMovesExist = true;
                        return;
                    }
                }
                else {
                    Card topFoundationCard = foundation.topCard(i);
                    if (wasteCard.getSuit() == topFoundationCard.getSuit() &&
                        wasteCard.getRank() == topFoundationCard.getRank() + 1) {
                        validMovesExist = true;
                        return;
                    }
                }
            }
        }

        // Check if there are cards in the stock pile
        if (!stockPile.isEmpty()) {
            validMovesExist = true;
            return;
        }

        if (!validMovesExist) {
            gameOver = true;
            cout << "Game over! No more valid moves. You made " << totalMoves << " moves." << endl;
        }
    }

public:
    Game() : gameOver(false), totalMoves(0) {
        initializeGame();
    }

    void runGame() {
        string input;
        while (!gameOver) {
            try {
                printGameState();
                cout << "Enter command: ";
                getline(cin, input);
                Command cmd = parseCommand(input);
                processCommand(cmd);
            }
            catch (const invalid_argument& e) {
                cout << "Error: " << e.what() << endl;
                cout << "Invalid command. Please try again." << endl;
            }
            catch (const out_of_range& e) {
                cout << "Error: " << e.what() << endl;
                cout << "Invalid command. Please try again." << endl;
            }
            catch (const exception& e) {
                cout << "An unexpected error occurred: " << e.what() << endl;
                cout << "Please try again." << endl;
            }
        }
    }
};

int main() {
    Game game;
    game.runGame();
    return 0;
}