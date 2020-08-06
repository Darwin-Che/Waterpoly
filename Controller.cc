#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "Controller.h"
#include "ModelFail.h"

struct cState
{
    int numDoubleRoll;
    bool canRoll;
};

void Controller::takeTurn(std::istream in)
{
    std::string command;
    while (in >> command)
    {
        if (command == "roll")
        {
            int d1; // the first dice number
            int d2; // the second dice number
            model->show("Player " + playerList[cur] + "rolls :" + std::to_string(d1) + " and " + std::to_string(d2) + ". ");
            if (d1 == d2)
            {
                model->show("This is a double! ");
                model->show("You have already rolled " + std::to_string(cstate->numDoubleRoll) + " doubles!.");
                // prevent roll double three times
                if (cstate->numDoubleRoll < 2)
                {
                    cstate->numDoubleRoll++;
                }
                else
                {
                    cstate->numDoubleRoll++;
                    cstate->canRoll = false;
                    // need to call models method to go to Times line directly
                    model->gotoTims(playerList[cur]);
                }
            }
            else
            {
                cstate->canRoll = false;
            }
            // notify view to display the dice number
            model->playerProceed(playerList[cur], d1 + d2);
        }
        else if (command == "next")
        {
            // message is given by model
            if (model->canNext(playerList[cur]))
            {
                cur++;
                if (cur == playerList.size())
                    cur = 0;
            }
            cstate->canRoll = true;
            cstate->numDoubleRoll = 0;
            model->show("Current Active Player: " + playerList[cur]);
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
                model->trade(receiver, playerList[cur], arg2, a1);
            }
            else if (n2)
            {
                // sell
                model->trade(playerList[cur], receiver, arg1, a2);
            }
            else
            {
                model->trade(playerList[cur], receiver, arg1, arg2);
            }
        }
        else if (command == "improve")
        {
            std::string arg1, arg2;
            in >> arg1, arg2;
            if (arg2 == "sell")
            {
                model->improve(playerList[cur], arg1, false);
            }
            else if (arg2 == "buy")
            {
                model->improve(playerList[cur], arg1, true);
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
            model->improve(playerList[cur], arg, true);
        }
        else if (command == "unmortgage")
        {
            std::string arg;
            in >> arg;
            model->improve(playerList[cur], arg, false);
        }
        else if (command == "bankrupt")
        {
            bool success = false;
            try
            {
                model->bankrupt(playerList[cur]);
                success = true;
            }
            catch (ModelFail &e)
            {
            }
            if (success)
            {
                playerList.erase(playerList.begin() + cur);
                model->show("Current Active Player: " + playerList[cur]);
            }
        }
        else if (command == "asset")
        {
            model->getInfo(playerList[cur]);
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
            model->save(outfile);
        }
        else
        {
            model->show("inValid Command!");
        }
    }
}
