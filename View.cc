#include "View.h"
#include "Player.h"
#include "AcademicBuilding.h"

//constructor
View::View(int height,int width):height(height), width(width) {
    actualH=height+1;
    actualW=width+1;
    improvements = std::vector<int>(2*(width+height),1);
}

// take a 1-d location and calculate its 2-d position on the board
std::vector<int> View::get2Dlocation(int location){
    if (location < 0) {location = 0; }
    else if ( location >= 2*(width+height) ) {location = 2*(width+height)-1; }
    if(location <= width ){
        std::vector<int> position {height, width-location};
        return position;
    }
    else if (location <= width+height ){
        std::vector<int> position {width+height-location, 0};
        return position;
    }
    else if (location <=2*width+height ){
        std::vector<int> position {0, location-(width+height) };
        return position;
    }
    else{
       std::vector<int> position {location-(2*width+height), width };
       return position;
    }
}

// init the players at location 0 
void View::addPlayer(const char & ID){
    players[ID]=0;
} 

// add a square
void View::addSquare(const std::string & name){
    squareName.emplace_back(name);
}

// When notified. A View could only be notified by a player or an academic building
void View::notify( Subject & whoNotified ){
    Player *player = dynamic_cast<Player *>( &whoNotified );
    AcademicBuilding *acbuilding = dynamic_cast<AcademicBuilding *>( &whoNotified );
    if ( player != nullptr ){
        // If View is notified by a player
        if ( player->getIsBankrupt() ){
            // If the player bankrupt
            players.erase( player->getSymbol() );
        }
        else{
            // otherwise the player's position has changed
            players[ player->getSymbol() ] = player->getPosition();
        }
    }
    else if( acbuilding != nullptr ){
        // If the view is notified by a academic building
        improvements[acbuilding->getLocation()] = acbuilding->getImprovementLevel();
    }
}

