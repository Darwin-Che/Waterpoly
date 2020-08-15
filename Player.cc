#include "Player.h"

// constructors
Player::Player(char symbol, const std::string& name) :symbol(symbol), name(name) {
    money = 1500;
    position = 0;
    lastMoveAmount = 0;
    numCups = 0;
    isJailed = false;
    numJailed = 0;
    debt = 0;
    debtOwner = "";
    isBankrupt = false;
}

Player::Player(char symbol, const std::string& name, int position, int numCups, int money) :symbol(symbol), name(name) {
    this->position = position;
    this->numCups = numCups;
    totalNumCups += numCups;
    this->money = money;
    lastMoveAmount = 0;
}

Player::Player(char symbol, const std::string& name, int position, int numCups, int money, int numJailed) :symbol(symbol), name(name) {
    this->position = position;
    this->numCups = numCups;
    totalNumCups += numCups;
    this->money = money;
    lastMoveAmount = 0;
    this->numJailed = numJailed;
    isJailed = true;
}

// accessors and mutators
char Player::getSymbol() const {
    return symbol;
}

std::string Player::getName() const {
    return name;
}

int Player::getMoney() const {
    return money;
}

void Player::setMoney(const int & money) {
    this->money = money;
}

int Player::getPosition() const {
    return position;
}

// When the position of a player is set, notify observers
void Player::moveForward(const int & moveAmount, const int & totalSquares) {
    position = (position + moveAmount + totalSquares) % totalSquares;
    lastMoveAmount = moveAmount;
    notifyObservers();
}

int Player::getLastMoveAmount() const {
    return lastMoveAmount;
}

int Player::getNumCups() const {
    return numCups;
}

void Player::setNumCups(const int & numCups) {
    int change = numCups - this->numCups;
    totalNumCups += change;
    this->numCups = numCups;
}

bool Player::getIsJailed() const {
    return isJailed;
}

void Player::setIsJailed(const bool & isJailed) {
    this->isJailed = isJailed;
}

int Player::getNumJailed() const {
    return numJailed;
}

void Player::setNumJailed(const int & numJailed) {
    this->numJailed = numJailed;
}

int Player::getDebt() const {
    return debt;
}

void Player::setDebt(const int & debt) {
    this->debt = debt;
}

std::string Player::getDebtOwner() const {
    return debtOwner;
}

void Player::setDebtOwner(const std::string & name) {
    debtOwner = name;
}

bool Player::getIsBankrupt() const {
    return isBankrupt;
}

// check if the player has enough money to pay debt
bool Player::canPayDebt() const {
    return money >= debt;
}

// notify Observers when the player bankrupt
void Player::dropOut() {
    isBankrupt = true;
    notifyObservers();
}

// set up static memebrs
int Player::totalNumCups = 0;

int Player::getTotalNumCups() {
    return totalNumCups;
}

void Player::setTotalNumCups(int t) {
    totalNumCups = t;
}

Info Player::getSubjectInfo(){
    return Info{InfoType::player,{isBankrupt},{position},{symbol},{}};
}

