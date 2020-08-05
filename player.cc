#include "player.h"

// constructor
Player::Player(char symbol, std::string name):symbol(symbol), name(name) {}

// accessors and mutators
char Player::getSymbol(){ return symbol; }

std::string Player::getName(){ return name; }

int Player::getMoney(){ return money; }

void Player::setMoney(const int & money){ this->money = money; }

int Player::getPosition(){ return position; }

void Player::setPosition(const int & position){ this->position = position; }

int Player::getNumCups(){ return numCups; }

void Player::setNumCups(const int & numcups){ this->numCups = numCups; }

bool Player::getIsJailed(){ return isJailed; }

void Player::setIsJailed(const bool & isJaied){ this->isJailed = isJailed; }

int Player::getNumJailed(){ return numJailed; }

void Player::setNumJailed(const int & getNumJailed){ this->numJailed = numJailed; }

// notify Observers when the player bankrupt
void Player::dropOut(){
    notifyObservers();
}
