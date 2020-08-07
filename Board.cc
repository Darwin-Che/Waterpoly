#include "Board.h"
#include "Building.h" 

using namespace std;

// constructor
Board::Board(std::vector<std::shared_ptr<Player>> ownershipList,
        std::vector<std::shared_ptr<Square>> board,
        std::map<string,std::vector<std::shared_ptr<Square> > >  monopolyBlock):
        ownershipList(ownershipList), board(board), monopolyBlock(monopolyBlock) {}

// return the number of squares
int Board::getTotalSquareNum(){
    return board.size();
}

// return player's net asset woth
int Board::calcNetworth(std::string playername){
    int worth = 0;
    for (int i=0; i<board.size(); i++){
        if(ownershipList[i]->getName() == playername){
            shared_ptr<Building> build = dynamic_pointer_cast<Building>(board[i]);
            worth+=build->calculateNetworth();
        }
    }
    return worth;
}

// return the squares owned by the player
std::vector<std::shared_ptr<Square>> Board::getAssets(std::string playername){
    std::vector<std::shared_ptr<Square>> assets;
    for (int i=0; i<board.size(); i++){
        if(ownershipList[i]->getName() == playername){
            assets.push_back(board[i]);
        }
    }
    return assets;
}

// return the owner of the building
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

// set the owner to the building
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

// find the square by name
std::shared_ptr<Square> Board::getSquareBuilding(std::string buildingname){
    for (auto it : board){
        if(it->getName() == buildingname){
            return it;
        }
    }
    return shared_ptr<Square>(nullptr);
}

// find the square by location
std::shared_ptr<Square> Board::getSquare(int location){
    return board[location];
}

// find the square by name
int Board::getSquareLocation(std::string buildingname){
    int count =0;
    for (auto it : board){
        if(it->getName() == buildingname){
            return count;
        }
        count ++;
    }
    return -1;
}

// check if two buildings have the same owner
bool Board::sameOwner(string building1, string building2){
    shared_ptr<Player> owner1 = getOwner(building1);
    shared_ptr<Player> owner2 = getOwner(building2);
    if (owner1 == nullptr || owner2 == nullptr){
        return false;
    }
    return owner1->getSymbol() == owner2->getSymbol();
}

// check if a building is in a certain block    
bool Board::inBlock(string building, vector<shared_ptr<Square>> block){
    for (auto square : block){
        if(square->getName() == building){
            return true;
        }
    }
    return false;
}

// check if the owner of the building owns the entire block
bool Board::inMonopoly(std::string buildingname){
    for (auto monopoly : monopolyBlock){
        if (inBlock(buildingname, monopoly.second)){
            for (auto square: monopoly.second){
                if(!sameOwner(buildingname, square->getName())){
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

// return the number of neibours owned by owner
int Board::numNeighbourOwned(std::string buildingname){
    int count = 0;
    for (auto monopoly : monopolyBlock){
        if (inBlock(buildingname, monopoly.second)){
            for (auto square: monopoly.second){
                if(sameOwner(buildingname, square->getName())){
                    count++;
                }
            }
            return count;
        }
    }
    return count;
}

// return the information for saving the file
std::string Board::saveInfo(){
    string info="";
    for (auto it:board){
        string squareinfo = it->saveInfo();
        if(squareinfo != ""){
            info = info + it->getName() + " ";
            auto owner = getOwner(it->getName());
            if (owner != nullptr){
                info = info + owner->getName() + squareinfo + "\n";
            }
            else{
                info = info + "BANK" + squareinfo + "\n";
            }
        }
    }
    return info;
}

