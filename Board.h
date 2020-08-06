#ifndef _BOARD_H_
#define _BOARD_H_
#include "Square.h"
#include "Player.h"
#include <vector>
#include <memory>

class Board {
    std::vector<std::shared_ptr<Player>> ownershipList;
    std::vector<std::shared_ptr<Square>> board;
    std::vector<std::vector<std::shared_ptr<Square> > >  monopolyBlock;

    bool sameOwner(std::string building1, std::string building2);
    bool inBlock(std::string building, std::vector<std::shared_ptr<Square>> block);
  public:
    Board(std::vector<std::shared_ptr<Player>> ownershipList,
        std::vector<std::shared_ptr<Square>> board,
        std::vector<std::vector<std::shared_ptr<Square> > >  monopolyBlock);

    int calcNetworth(std::string playername);

    std::shared_ptr<Player> getOwner(std::string buildingname);

    void setOwner(std::string buildingname, std::shared_ptr<Player> player);

    std::shared_ptr<Square> getSquareBuilding(std::string buildingname);

    std::shared_ptr<Square> getSquare(int location);

    bool inMonopoly(std::string buildingname);

    int numNeighbourOwned(std::string buildingname);
};

#endif
