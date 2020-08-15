#include "Controller.h"
#include "Model.h"
#include "Board.h"
#include "TextView.h"
#include "GraphicView.h"
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
#include "Dice.h"
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <cstdlib>

using namespace std;

// after Board is built and Model's Map is loaded
// read in Player information and load Model->loadPlayer
// read in Owner and building information to set corresponding information in board and building
// return the name of starting player
std::string loadFile(std::istream &loadfile, std::shared_ptr<Model> model, std::shared_ptr<Board> board, std::vector<std::shared_ptr<View>> views)
{
    /****************  Load Player Info ***************/

    std::vector<std::shared_ptr<Player>> Players;
    int playernum = 0;
    int maxCups = 4;

    if (!(loadfile >> playernum)) {
        throw std::invalid_argument{ "Fail to read in Player Number!\n" };
    }

    std::string rest;
    getline(loadfile, rest);

    for (int i = 0; i < playernum; i++) {
        std::string name{ "" }, symbol{ "" };
        int numCups = -1, money = -1, position = -1;

        // read in basic information
        if (!(loadfile >> name >> symbol >> numCups >> money >> position)) {
            throw std::invalid_argument{ "Fail to read in Player " + std::to_string(i) + " info!\n" };
        }

        // construct player
        auto player = std::make_shared<Player>(symbol[0], name, position, numCups, money);
        std::cout << player->getName() << player->getSymbol() << numCups << player->getMoney() << player->getPosition() << std::endl;

        // check if on gotoTims
        if (position == board->getSquareLocation("GO TO TIMS")) {
            throw std::invalid_argument{ "A Player should not start on GO TO TIMS!\n" };
        }

        // check if in prison
        if (position == board->getSquareLocation("DC Tims Line")) {
            // if in prison
            int intisInJail = 0;
            // read in if isInJail 0/1
            if (!(loadfile >> intisInJail)) {
                throw std::invalid_argument{ "Fail to read in Player " + std::to_string(i) + " info!\n" };
            }
            // check if in Jail
            if (intisInJail == 1) {
                player->setIsJailed(true);
                // read in turns in Jail
                int numsInJail = 0;
                if (!(loadfile >> numsInJail) || numsInJail < 0 || numsInJail > 2) {
                    throw std::invalid_argument{ "Fail to read in Player " + std::to_string(i) + " info!\n" };
                }
                player->setNumJailed(numsInJail);
            }
        }
        // player is successfully constructed
        for (auto view : views) {
            player->attach(view);
            view->addPlayer(symbol[0],position);
        }
        
        Players.emplace_back(player);
        getline(loadfile, rest);
        std::cout << "Player " << name << " successfully loaded!" << std::endl;
    }

    // check totalNumCups
    if (Player::getTotalNumCups() > maxCups) {
        throw std::invalid_argument{ "Too many Tims Cups in game!\n" };
    }

    // Players are loaded, load in Model
    std::cout << "Model loading players" << std::endl;
    model->loadPlayer(Players);

    /*************** Load Ownership ***************/

    // read in building owner information
    int i = 0;
    while (true) {
        std::string buildingName{ "" }, ownerName{ "" };
        int imprLevel = 0;
        if (!(loadfile >> buildingName >> ownerName >> imprLevel)) {
            if (loadfile.eof()) return Players[0]->getName();
            throw std::invalid_argument{ "Fail to read in Ownership information " + std::to_string(i) + "!\n" };
        }

        // check if building exists
        std::shared_ptr<Square> s = board->getSquareBuilding(buildingName);
        if (s.get() == nullptr) {
            throw std::invalid_argument{ "No building named " + buildingName + "\n" };
        }

        if (ownerName != "BANK") {
            // find player pointer
            std::vector<std::shared_ptr<Player>>::iterator owner = std::find_if(Players.begin(), Players.end(), [ownerName](const std::shared_ptr<Player> tp) { return tp->getName() == ownerName;});

            // check if exist player with that name
            if (owner == Players.end()) {
                throw std::invalid_argument{ "No player named " + ownerName + "\n" };
            }

            board->setOwner(buildingName, *owner);
        }

        // set improvementLevel
        if (imprLevel == -1) {
            std::shared_ptr<Building> b = std::dynamic_pointer_cast<Building>(s);
            if (b.get() == nullptr) {
                throw std::invalid_argument{ "You cannot mortgage a non property!\n" };
            }
            b->setIsMortgaged(true);
        } else if (imprLevel >= 1 && imprLevel <= 5) {
            std::shared_ptr<AcademicBuilding> ab = std::dynamic_pointer_cast<AcademicBuilding>(s);
            if (ab.get() == nullptr) {
                throw std::invalid_argument{ "You cannot improve a non academic building!\n" };
            }
            ab->setImprovementLevel(imprLevel);
        }
        else if (imprLevel != 0) {
            throw std::invalid_argument{ "Invalid improvement level!\n" };
        }

        getline(loadfile, rest);
        ++i;
    }
    return Players[0]->getName();
}

bool findPlayer(char symbol, string name, vector<shared_ptr<Player>> Players, ostream &out) {
    for (auto p:Players) {
        if (p->getName() == name) {
            out << "The name " << name << " already exists" << endl;
            return true;
        } else if (p->getSymbol() == symbol) {
            out << "The symbol " << symbol << " already exists" << endl;
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    std::string loadfilename{ "" };
    bool testing = false;
    int argct = 1;
    bool vText = false;
    bool vGraph = false;
    std::vector<int> houseRules;
    while (argct < argc) {
        if (std::string(argv[argct]) == "-load") {
            argct++;
            if (argct <= argc) {
                loadfilename = std::string(argv[argct]);
            } else {
                std::cerr << "No filename after -load!" << std::endl;
                throw std::logic_error("No filename after -load!");
            }
        } else if (std::string(argv[argct]) == "-testing") {
            testing = true;
        } else if (std::string(argv[argct]).substr(0, 5) == "-seed") {
            try {
                std::string seed = std::string(argv[argct]).substr(5);
                int seedNum = std::stoi(seed);
                srand(seedNum);
            } catch (std::exception& e) {
                std::cerr << "Command line argument is incorrectly specified." << std::endl;
                return 1;
            }

        } else if (std::string(argv[argct]) == "-vText"){
            vText = true;
        } else if (std::string(argv[argct]) == "-vGraph"){
            vGraph = true;
        } else if (std::string(argv[argct]).substr(0, 5) == "-rule") {
            try {
                std::string choice = std::string(argv[argct]).substr(5);
                int choiceNum = std::stoi(choice);
                if (choiceNum < 1 || choiceNum > 4) throw std::exception();
                houseRules.emplace_back(choiceNum);
            } catch (std::exception& e) {
                std::cerr << "Command line argument is incorrectly specified." << std::endl;
                return 1;
            }
        }
        argct++;
    }

    std::ifstream infile{ "initial_information.txt" };
    int boardH;
    int boardW;
    infile >> boardH;
    infile >> boardW;
    std::string throwaway;
    getline(infile, throwaway);

    if (!vText && !vGraph) {
        vText = true;
    }
    vector<shared_ptr<View>> views;
    if (vText) {
        views.push_back( make_shared<TextView>(boardH, boardW) );
    }
    if (vGraph) {
        views.push_back(  make_shared<GraphicsView>(boardH, boardW) );
    }

    std::vector<std::shared_ptr<Square>> board;
    std::map<std::string, std::vector<std::shared_ptr<Square>>> monopolyBlock;
    std::vector<shared_ptr<VisitStrategy>> strategies;
    vector<shared_ptr<Player>> ownershipList;

    std::string command;
    string name;
    string description;
    int squareNum = 0;
    while (getline(infile, command)) {
        getline(infile, name);
        getline(infile, description);
        if (command == "square") {
            auto square = make_shared<Square>(name, squareNum, description);
            board.push_back(square);
            for (auto v: views) {
                v->addSquare(name, View::nonBuilding);
            }
            if (name == "SLC") {
                strategies.push_back(make_shared<SLCStrategy>());
            } else if (name == "COLLECT OSAP") {
                strategies.push_back(make_shared<CollectOSAPStrategy>());
            } else if (name == "GO TO TIMS") {
                strategies.push_back(make_shared<GoToTimsStrategy>());
            } else if (name == "DC Tims Line") {
                strategies.push_back(make_shared<DCTimsLineStrategy>());
            } else if (name == "Goose Nesting") {
                strategies.push_back(make_shared<GooseNestingStrategy>());
            } else if (name == "TUITION") {
                strategies.push_back(make_shared<TuitionStrategy>());
            } else if (name == "COOP FEE") {
                strategies.push_back(make_shared<CoopFeeStrategy>());
            } else if (name == "NEEDLES HALL") {
                strategies.push_back(make_shared<NeedlesHallStrategy>());
            }
        } else if (command == "gym") {
            string blockName;
            int purchaseCost;
            getline(infile, blockName);
            infile >> purchaseCost;
            getline(infile, command);
            shared_ptr<Gym> gym = make_shared<Gym>(name, squareNum, description, purchaseCost, false);
            board.push_back(gym);
            if (monopolyBlock.count(blockName) == 0) {
                std::vector<std::shared_ptr<Square>> block;
                block.push_back(gym);
                monopolyBlock[blockName] = block;
            } else {
                monopolyBlock[blockName].push_back(gym);
            }
            for (auto v: views) {
                gym->attach(v);
                v->addSquare(name, View::nonAcademic);
            }
            strategies.push_back(make_shared<GymStrategy>());
        } else if (command == "residence") {
            string blockName;
            int purchaseCost;
            getline(infile, blockName);
            infile >> purchaseCost;
            getline(infile, command);
            shared_ptr<Residence> residence = make_shared<Residence>(name, squareNum, description, purchaseCost, false);
            board.push_back(residence);
            if (monopolyBlock.count(blockName) == 0) {
                std::vector<std::shared_ptr<Square>> block;
                block.push_back(residence);
                monopolyBlock[blockName] = block;
            } else {
                monopolyBlock[blockName].push_back(residence);
            }
            for (auto v: views) {
                residence->attach(v);
                v->addSquare(name, View::nonAcademic);
            }
            strategies.push_back(make_shared<ResidenceStrategy>());
        } else if (command == "academicbuilding") {
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

            for (int i = 0; i <= maximprovement; i++) {
                int fee;
                infile >> fee;
                getline(infile, command);
                improvementfee.push_back(fee);
            }

            auto acbuilding = make_shared<AcademicBuilding>(name, squareNum, description, purchaseCost, false, improvementCost, improvementfee);
            board.push_back(acbuilding);
            if (monopolyBlock.count(blockName) == 0) {
                std::vector<std::shared_ptr<Square>> block;
                block.push_back(acbuilding);
                monopolyBlock[blockName] = block;
            } else {
                monopolyBlock[blockName].push_back(acbuilding);
            }
            for (auto v: views) {
                acbuilding->attach(v);
                v->addSquare(name, View::academic);
            }
            strategies.push_back(make_shared<AcademicBuildingStrategy>());
        }
        ownershipList.push_back(shared_ptr<Player>());
        squareNum++;
    }
    /*
    for (int i = 0; i < board.size(); i++) {
        cout << i << " " << board[i]->getName() << endl;
    }

    for (auto it : monopolyBlock) {
        cout << it.first << ": ";
        for (auto t : it.second) {
            cout << t->getName() << " ";
        }
        cout << endl;
    }
    */
    /*************** initiate board and model ****************/

    istream &in = cin;
    ostream &out = cout;
    auto boardMap = make_shared<Board>(ownershipList, board, monopolyBlock);
    std::shared_ptr<Model> model;
    if (houseRules.size() == 0)
        model = make_shared<Model>(in, out, "Default", houseRules);
    else
        model = make_shared<Model>(in, out, "HouseRules", houseRules);
    model->loadMap(boardMap, strategies);
    std::string startName = "";

    /************** branch on whether load file ***************/

    if (loadfilename == "") {
        // no load file
        std::vector<shared_ptr<Player>> Players;
        int playernum = 0;

        while (playernum < 2 || playernum > 8) {
            cout << "Please choose enter the number of players (2~8):" << endl;
            if (!(in >> playernum)) {
                if (in.eof())
                    break;
                in.clear();
                in.ignore();
            }
        }

        for (int i = 0; i < playernum; i++) {
            string name, symbol;
            cout << "(Player #" << (i + 1) << ") Please enter your symbol and name:" << endl;
            cin >> symbol >> name;
            while (findPlayer(symbol[0], name, Players, out)) {
                cout << "(Player #" << (i + 1) << ") Please enter your symbol and name:" << endl;
                cin >> symbol >> name;
            }
            auto player = make_shared<Player>(symbol[0], name);
            for (auto v: views) {
                player->attach(v);
                v->addPlayer(symbol[0],0);
            };
            Players.push_back(player);
            getline(cin, name);
        }
        startName = Players[0]->getName();
        model->loadPlayer(Players);
    } else {
        // have load file
        std::ifstream infile{ loadfilename };
        startName = loadFile(infile, model, boardMap, views);
    }

    Dice::init(testing);
    Controller game{ model, startName };
    for (auto v: views) {
        v->drawBoard();
    }
    game.takeTurn(in);

}


