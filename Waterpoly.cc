
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
#include "ResidenceStrategy.h"
#include "SLCStrategy.h"
#include "TuitionStrategy.h"
#include "NeedlesHallStrategy.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
    std::string loadfilename;
    bool testing = false;
    int argct = 1;
    while (argct < argc)
    {
        if (std::string(argv[argct]) == "-load")
        {
            argct++;
            if (argct <= argc)
            {
                loadfilename = std::string(argv[argct]);
            }
            else
            {
                std::cerr << "No filename after -load!" << std::endl;
                throw std::logic_error("No filename after -load!");
            }
        }
        else if (std::string(argv[argct]) == "-testing")
        {
            testing = true;
        }
        argct++;
    }

    std::ifstream infile{"initial_information.txt"};
    int boardH;
    int boardW;
    infile >> boardH;
    infile >> boardW;
    std::string throwaway;
    getline(infile, throwaway);
    auto view = make_shared<TextView>(boardH, boardW);

    std::vector<std::shared_ptr<Square>> board;
    std::map<std::string, std::vector<std::shared_ptr<Square>>> monopolyBlock;
    std::vector<shared_ptr<VisitStrategy>> strategies;
    vector<shared_ptr<Player>> ownershipList;

    std::string command;
    string name;
    string description;
    int squareNum = 0;
    while (getline(infile, command))
    {
        getline(infile, name);
        getline(infile, description);
        if (command == "square")
        {
            auto square = make_shared<Square>(name, squareNum, description);
            board.push_back(square);
            view->addSquare(name, -1);
            if (name == "SLC")
            {
                strategies.push_back(make_shared<SLCStrategy>());
            }
            else if (name == "COLLECT OSAP")
            {
                strategies.push_back(make_shared<CollectOSAPStrategy>());
            }
            else if (name == "GO TO TIMS")
            {
                strategies.push_back(make_shared<GoToTimsStrategy>());
            }
            else if (name == "DC Tims Line")
            {
                strategies.push_back(make_shared<DCTimsLineStrategy>());
            }
            else if (name == "Goose Nesting")
            {
                strategies.push_back(make_shared<GooseNestingStrategy>());
            }
            else if (name == "TUITION")
            {
                strategies.push_back(make_shared<TuitionStrategy>());
            }
            else if (name == "COOP FEE")
            {
                strategies.push_back(make_shared<CoopFeeStrategy>());
            }
            else if (name == "NEEDLES HALL")
            {
                strategies.push_back(make_shared<NeedlesHallStrategy>());
            }
        }
        else if (command == "gym")
        {
            string blockName;
            int purchaseCost;
            getline(infile, blockName);
            infile >> purchaseCost;
            getline(infile, command);
            shared_ptr<Gym> gym = make_shared<Gym>(name, squareNum, description, purchaseCost, false);
            board.push_back(gym);
            if (monopolyBlock.count(blockName) == 0)
            {
                std::vector<std::shared_ptr<Square>> block;
                block.push_back(gym);
                monopolyBlock[blockName] = block;
            }
            else
            {
                monopolyBlock[blockName].push_back(gym);
            }
            view->addSquare(name,-1);
            strategies.push_back(make_shared<GymStrategy>());
        }
        else if (command == "residence")
        {
            string blockName;
            int purchaseCost;
            getline(infile, blockName);
            infile >> purchaseCost;
            getline(infile, command);
            shared_ptr<Residence> residence = make_shared<Residence>(name, squareNum, description, purchaseCost, false);
            board.push_back(residence);
            if (monopolyBlock.count(blockName) == 0)
            {
                std::vector<std::shared_ptr<Square>> block;
                block.push_back(residence);
                monopolyBlock[blockName] = block;
            }
            else
            {
                monopolyBlock[blockName].push_back(residence);
            }
            view->addSquare(name,-1);
            strategies.push_back(make_shared<ResidenceStrategy>());
        }
        else if (command == "academicbuilding")
        {
            string blockName;
            int purchaseCost;
            int improvementCost;
            int maximprovement;
            vector<int> improvementfee;
            getline(infile, blockName);
            infile >> purchaseCost;
            getline(infile, command);
            infile >> improvementCost;
            getline(infile, command);
            infile >> maximprovement;
            getline(infile, command);

            for (int i = 0; i <= maximprovement; i++)
            {
                int fee;
                infile >> fee;
                getline(infile, command);
                improvementfee.push_back(fee);
            }

            auto acbuilding = make_shared<AcademicBuilding>(name, squareNum, description, purchaseCost, false, improvementCost, improvementfee);
            board.push_back(acbuilding);
            if (monopolyBlock.count(blockName) == 0)
            {
                std::vector<std::shared_ptr<Square>> block;
                block.push_back(acbuilding);
                monopolyBlock[blockName] = block;
            }
            else
            {
                monopolyBlock[blockName].push_back(acbuilding);
            }
            acbuilding->attach(view);
            view->addSquare(name,0);
            strategies.push_back(make_shared<AcademicBuildingStrategy>());
        }
        ownershipList.push_back(shared_ptr<Player>());
        squareNum++;
    }

    for (int i = 0; i < board.size(); i++)
    {
        cout << i << " " << board[i]->getName() << endl;
    }

    for (auto it : monopolyBlock)
    {
        cout << it.first << ": ";
        for (auto t : it.second)
        {
            cout << t->getName() << " ";
        }
        cout << endl;
    }

    std::vector<shared_ptr<Player>> Players;
    int playernum = 0;
    istream &in = cin;
    ostream &out = cout;

    while (playernum < 6 || playernum > 8)
    {
        cout << "Please choose enter the number of players (6~8):" << endl;
        if (!(in >> playernum))
        {
            if (in.eof())
                break;
            in.clear();
            in.ignore();
        }
    }

    for (int i = 0; i < playernum; i++)
    {
        string name, symbol;
        cout << "(Player #" << (i + 1) << ") Please enter your symbol and name:" << endl;
        cin >> symbol >> name;
        auto player = make_shared<Player>(symbol[0], name);
        player->attach(view);
        view->addPlayer(symbol[0]);
        Players.push_back(player);
        getline(cin, name);
    }

    auto boardMap = make_shared<Board>(ownershipList, board, monopolyBlock);
    auto model = make_shared<Model>(in, out);
    model->loadPlayer(Players);
    model->loadMap(boardMap, strategies);
    model->setView(view);
    Controller game{model, Players[0]->getName(), testing};
    view->drawBoard();
    game.takeTurn(in);
}