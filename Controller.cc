#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <memory>
#include <cstdlib>
#include <ctime>
#include "Controller.h"
#include "ModelExcept.h"
#include "Model.h"
#include "Dice.h"

Controller::Controller(std::shared_ptr<Model> t_model, const std::string &startPlayerName)
    : model{t_model}, curPlayerName{startPlayerName}
{
    srand(time(NULL));
}

void Controller::takeTurn(std::istream &in)
{
    std::string command;
    while (in >> command)
    {
        if (command == "roll")
        {
            if (!model->checkPlayerDebt(curPlayerName)){
                model->show("Please pay off debt before rolling!");
                continue;
            }
            
            if (model->playerJailed(curPlayerName) && Dice::canRoll)
            {
                model->show("You are in Jail currently, so please listen to the prison officials!");
                model->playerProceed(curPlayerName, 0);
            }
            else
            {
                if (Dice::canRoll)
                {
                    std::pair<int, int> res = Dice::roll();
                    int d1 = res.first;
                    int d2 = res.second;
                    model->show("Player " + curPlayerName + " rolls: " + std::to_string(d1) + " and " + std::to_string(d2) + ". ");
        
                    if (d1 == d2)
                    {
                        model->show("This is a double! ");
                        model->show("You have already rolled " + std::to_string(Dice::numDoubles) + " doubles!.");
                        // prevent roll double three times
                        if (Dice::numDoubles < 3)
                        {
                            bool toJail = model->playerProceed(curPlayerName, d1 + d2);
                            Dice::canRoll = !toJail;
                        }
                        else
                        {
                            Dice::canRoll = false;
                            // need to call models method to go to Times line directly
                            model->show("You consecutively rolled 3 doubles, sent to Jail!");
                            model->gotoTims(curPlayerName);
                        }
                    }
                    else
                    {
                        Dice::canRoll = false;
                        model->playerProceed(curPlayerName, d1 + d2);
                    }
                }
                else
                {
                    model->show("You are not permitted to roll!");
                }
            }
        }
        else if (command == "next")
        {
            if (Dice::canRoll)
            {
                model->show("You must first roll!");
                continue;
            }
            std::string nextPlayer = model->nextPlayerName(curPlayerName);
            if (nextPlayer != curPlayerName)
            {
                Dice::clear();
                curPlayerName = nextPlayer;
            }
            model->show("Current Active Player: " + curPlayerName);
        }
        else if (command == "trade")
        {
            std::string receiver, arg1, arg2;
            in >> receiver >> arg1 >> arg2;
            int a1 = -1, a2 = -1;
            bool n1 = false, n2 = false;
            try
            {
                a1 = std::stoi(arg1);
                n1 = true;
            }
            catch (const std::exception &e)
            {
            }
            try
            {
                a2 = std::stoi(arg2);
                n2 = true;
            }
            catch (const std::exception &e)
            {
            }
            if (n1 && n2)
            {
                model->show("Can not trade money for money!");
            }
            if (n1)
            {
                // buy
                model->trade(receiver, curPlayerName, arg2, a1, true);
            }
            else if (n2)
            {
                // sell
                model->trade(curPlayerName, receiver, arg1, a2, false);
            }
            else
            {
                model->trade(curPlayerName, receiver, arg1, arg2);
            }
        }
        else if (command == "improve")
        {
            std::string arg1, arg2;
            in >> arg1 >> arg2;
            if (arg2 == "sell")
            {
                model->improve(curPlayerName, arg1, false);
            }
            else if (arg2 == "buy")
            {
                model->improve(curPlayerName, arg1, true);
            }
            else
            {
                model->show("Invalid second argument! Options: \"Sell\" or \"Buy\" ");
            }
        }
        else if (command == "mortgage")
        {
            std::string arg;
            in >> arg;
            model->mortgage(curPlayerName, arg, true);
        }
        else if (command == "unmortgage")
        {
            std::string arg;
            in >> arg;
            model->mortgage(curPlayerName, arg, false);
        }
        else if (command == "bankrupt")
        {
            std::string nextName = model->nextPlayerName(curPlayerName, true);
            std::string doubleKill{""};
            std::cout << nextName;
            try
            {
                bool success = model->bankrupt(curPlayerName, doubleKill);
                if (success)
                {
                    if (doubleKill == "")
                    {
                        curPlayerName = nextName;
                        Dice::clear();
                    }
                    else
                    {
                        curPlayerName = doubleKill;
                        Dice::clear();
                        Dice::canRoll = false;
                    }
                }
            }
            catch (ModelExcept &e)
            {
                std::cout << e.getmessage() << std::endl;
                return;
            }
            model->show("Current Active Player: " + curPlayerName);
        }
        else if (command == "assets")
        {
            model->getInfo(curPlayerName);
        }
        else if (command == "all")
        {
            model->getInfo();
        }
        else if (command == "save")
        {
            std::string arg;
            in >> arg;
            std::ofstream outfile{arg};
            model->save(outfile, curPlayerName);
        }
        else
        {
            model->show("inValid Command!");
        }
    }
}
