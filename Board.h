#ifndef _BOARD_H_
#define _BOARD_H_
#include "Square.h"
#include "Player.h"
#include <vector>
#include <map>
#include <memory>

class Board {
    std::vector<std::shared_ptr<Player>> ownershipList;
    std::vector<std::shared_ptr<Square>> board;
    std::map<std::string,std::vector<std::shared_ptr<Square> > >  monopolyBlock;

    // check if two buildings have the same owner
    bool sameOwner(std::string building1, std::string building2);
    // check if a building is in a certain block
    bool inBlock(std::string building, std::vector<std::shared_ptr<Square>> block);
  public:
    // constructor 
    Board(std::vector<std::shared_ptr<Player>> ownershipList,
        std::vector<std::shared_ptr<Square>> board,
        std::map<std::string,std::vector<std::shared_ptr<Square> > >  monopolyBlock);

    // return the number of squares
    int getTotalSquareNum();

    // return player's net asset woth
    int calcNetworth(std::shared_ptr<Player> player);

    // return the squares owned by the player
    std::vector<std::shared_ptr<Square>> getAssets(std::string player);

    // return the owner of the building
    std::shared_ptr<Player> getOwner(std::string buildingname);

    // set the owner to the building
    void setOwner(std::string buildingname, std::shared_ptr<Player> player);

    // find the square by name
    std::shared_ptr<Square> getSquareBuilding(std::string buildingname);

    // find the square by location
    std::shared_ptr<Square> getSquare(int location);

    // find the square's location
    int getSquareLocation(std::string buildingname);

    // check if the owner of the building owns the entire block
    bool inMonopoly(std::string buildingname);

    // return the number of neibours owned by owner
    int numNeighbourOwned(std::string buildingname);

    // return the information for saving the file
    std::string saveInfo();

};

#endif
