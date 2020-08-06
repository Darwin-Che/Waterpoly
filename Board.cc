#include "Board.h" 

using namespace std;
 
 Board::Board(std::vector<std::shared_ptr<Player>> ownershipList,
        std::vector<std::shared_ptr<Square>> board,
        std::vector<std::vector<std::shared_ptr<Square> > >  monopolyBlock):
        ownershipList(ownershipList), board(board), monopolyBlock(monopolyBlock) {}

int Board::calcNetworth(std::string playername){

}

std::shared_ptr<Player> Board::getOwner(std::string buildingname){
    int i=0;
    for (auto it : board){
        if(it->getName() == buildingname){
            return ownershipList[i];
        }
        i++;
    }
    return shared_ptr<Player>(nullptr);
}

void Board::setOwner(std::string buildingname, std::shared_ptr<Player> player){
    int i=0;
    for (auto it : board){
        if(it->getName() == buildingname){
            ownershipList[i]=player;
            break;
        }
        i++;
    }
}

std::shared_ptr<Square> Board::getSquareBuilding(std::string buildingname){
    for (auto it : board){
        if(it->getName() == buildingname){
            return it;
        }
    }
    return shared_ptr<Square>(nullptr);
}

std::shared_ptr<Square> Board::getSquare(int location){
    return board[location];
}

bool Board::sameOwner(string building1, string building2){
    shared_ptr<Player> owner1 = getOwner(building1);
    shared_ptr<Player> owner2 = getOwner(building2);
    if (owner1 == nullptr || owner2 == nullptr){
        return false;
    }
    return owner1->getSymbol() == owner2->getSymbol();
}
    
bool Board::inBlock(string building, vector<shared_ptr<Square>> block){
    for (auto square : block){
        if(square->getName() == building){
            return true;
        }
    }
    return false;
}

bool Board::inMonopoly(std::string buildingname){
    for (auto monopoly : monopolyBlock){
        if (inBlock(buildingname, monopoly)){
            for (auto square: monopoly){
                if(!sameOwner(buildingname, square->getName())){
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

int Board::numNeighbourOwned(std::string buildingname){
    int count = 0;
    for (auto monopoly : monopolyBlock){
        if (inBlock(buildingname, monopoly)){
            for (auto square: monopoly){
                if(sameOwner(buildingname, square->getName())){
                    count++;
                }
            }
            return count;
        }
    }
    return count;
}
