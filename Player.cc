#include "Player.h"

// constructors
Player::Player(char symbol, std::string name):symbol(symbol), name(name) {
    position = 0;
    oldPosition = 0;
    numCups = 0;
    numJailed = 0;
    isJailed = false;
    debt = 0;
    debtOwner = "";
    isBankrupt = false;
}

Player::Player(char symbol, std::string name, int position, int numCups, int money):
    Player(symbol, name), position(position), numCups(numCups), money(money){
        oldPosition = position;
}

Player::Player(char symbol, std::string name, int position, int numCups, int money, int numJailed):
    Player(symbol, name, position, numCups, money), numJailed(numJailed) {
    isJailed = true;
}

// accessors and mutators
char Player::getSymbol(){ return symbol; }

std::string Player::getName(){ return name; }

int Player::getMoney(){ return money; }

void Player::setMoney(const int & money){ this->money = money; }

int Player::getPosition(){ return position; }

// When the position of a player is set, notify observers
void Player::setPosition(const int & position){ 
    this->position = position; 
    notifyObservers();
}

int Player::getNumCups(){ return numCups; }

void Player::setNumCups(const int & numcups){ this->numCups = numCups; }

bool Player::getIsJailed(){ return isJailed; }

void Player::setIsJailed(const bool & isJaied){ this->isJailed = isJailed; }

int Player::getNumJailed(){ return numJailed; }

void Player::setNumJailed(const int & getNumJailed){ this->numJailed = numJailed; }

int Player::getDebt(){ return debt; 
}
void Player::setDebt(const int & debt){ this->debt = debt; }

std::string Player::getDebtOwner(){ return debtOwner; }

void Player::setDebtOwner(const std::string & name){ debtOwner = name; }

bool Player::getIsBankrupt(){ return isBankrupt; }

// check if the player has enough money to pay debt
bool Player::canPayDebt(){ return money >= debt; }

// notify Observers when the player bankrupt
void Player::dropOut(){
    isBankrupt = true;
    notifyObservers();
}
