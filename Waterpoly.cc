#include "Controller.h"
#include "Model.h"
#include "Board.h"
#include "TextView.h"
#include "Player.h"
#include "Square.h"
#include "AcademicBuilding.h"
#include "Gym.h"
#include "Residence.h"
#include "AcademicBuildingStrategy.h"
#include "CollectOSAPStrategy.h"
#include "CoopFeeStrategy.h"
#include "DCTimsLineStrategy.h"
#include "GooseNestingStrategy.h"
#include "GoToTimsStrategy.h"
#include "GymStrategy.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(){
    std::ifstream infile{ "intial_information.txt" };
    int boardH;
    int boardW;
    infile >> boardH;
    infile >> boardW;
    auto view = make_shared<TextView>(boardH,boardW);

    std::vector<std::shared_ptr<Square>> board;
    std::map<std::string,std::vector<std::shared_ptr<Square> > >  monopolyBlock;
    std::vector<VisitStrategy> strategies;

    std::string command;
    string name;
    string description;
    int squareNum=0;
    while (infile >> command){
        if ( command == "square"){
            infile >> name;
            getline(infile,description);
            board.push_back(make_shared<Square>(name,squareNum,description));
            squareNum++;
        }
        else if(command == "gym"){
            string blockName;
            int purchaseCost;
            infile >> name;
            infile >> blockName;
            infile >> purchaseCost;
            getline(infile,description);
            shared_ptr<Gym> gym=make_shared<Gym>(name,squareNum,description,purchaseCost, false);
            board.push_back(gym);
            if(monopolyBlock.count(blockName) == 0){
                std::vector<std::shared_ptr<Square>> block;
                block.push_back(gym);
                monopolyBlock[blockName] = block;
            }
            else{
                monopolyBlock[blockName].push_back(gym);
            } 
        }
        else if(command == "residence"){
            string blockName;
            int purchaseCost;
            infile >> name;
            infile >> blockName;
            infile >> purchaseCost;
            getline(infile,description);
            shared_ptr<Residence> residence=make_shared<Residence>(name,squareNum,description,purchaseCost, false);
            board.push_back(residence);
            if(monopolyBlock.count(blockName) == 0){
                std::vector<std::shared_ptr<Square>> block;
                block.push_back(residence);
                monopolyBlock[blockName] = block;
            }
            else{
                monopolyBlock[blockName].push_back(residence);
            } 
        }
        else if(command == "academicbuilding"){
            string blockName;
            int purchaseCost;
            int improvementCost;
            int maximprovement;
            vector<int> improvementfee;

            infile >> name;
            infile >> blockName;
            infile >> purchaseCost;
            infile >> improvementCost;
            infile >> maximprovement;

            for (int i=0;i<maximprovement; i++){
                int fee;
                infile >> fee;
                improvementfee.push_back(fee);
            }

            getline(infile,description);

            auto acbuilding=make_shared<AcademicBuilding>(name,squareNum,description,purchaseCost, false,improvementCost,improvementfee);
            board.push_back(acbuilding);
            if(monopolyBlock.count(blockName) == 0){
                std::vector<std::shared_ptr<Square>> block;
                block.push_back(acbuilding);
                monopolyBlock[blockName] = block;
            }
            else{
                monopolyBlock[blockName].push_back(acbuilding);
            } 
            acbuilding->attach(View);
        }
    }
}
