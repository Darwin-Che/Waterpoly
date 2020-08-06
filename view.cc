#include "view.h"
#include "player.h"
#include "AcademicBuilding.h"

//constructor
View::View(int height,int width):height(height), width(width) {
    improvements = std::vector<int>(2*(width+height),-1);
}

// take a 1-d location and calculate its 2-d position on the board
std::vector<int> View::get2Dlocation(int location){
    if (location < 0) {location = 0; }
    else if ( location >= 2*(width+height) ) {location = 2*(width+height)-1; }
    if(location <= width ){
        return std::vector<int>{height, width-location};
    }
    else if (location <= width+height ){
        return std::vector<int>{width+height-location, 0};
    }
    else if (location <=2*width+height ){
        return std::vector<int>{0, location-(width+height) };
    }
    else{
        return std::vector<int>{location-(2*width+height), width };
    }
}

// init the players at location 0 
void View::addPlayer(const char & ID)){
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
        if ( player->getMoney()<0 ){
            players.erase( player->getSymbol() );
        }
        else{
            players[ player->getSymbol() ] = player->getPosition();
        }
    }
    else if( acbuilding != nullptr ){
        improvements[acbuilding->getLocation()] = acbuilding->getImprovementLevel();
    }
}
