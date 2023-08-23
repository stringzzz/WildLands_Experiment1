/*

    Part of WildLand, an artificial organism environment gambling game, main part (for experiment)
    Copyright (C) 2023 stringzzz, Ghostwarez Co.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.


Wild Land: Organism Environment Simulator Experiment 10,000 Simulations
Tigers vs. Cats vs. Mice vs. Bugs

Food Chain:

1. Tigers
2. Cats
3. Mice
4. Bugs
5. Food

Each organism is capable of eating anything below it on the chain.

*/

#include <iostream>
#include "WLHeader.h"
using namespace std;

int main()
{
    srand(time(0));
    int simCount = 0;
    int simMax = 10000;
    int winCountTigers = 0;
    int winCountCats = 0;
    int winCountMice = 0;
    int winCountBugs = 0;
    
    while (simCount != simMax) {
        Land Plains;
        string winner = "None";
        Plains.growFood(60);
        Plains.displayLand();

        for (int n = 1; n <= 3000; n++) {
            //Loop through 3000 turns, all organisms usually die out before this
            Plains.growFood(3);
            Plains.move();
            Plains.reproduce();
            Plains.die();
            Plains.displayLand();
            Plains.displayPopCounts();
            cout << "Simulation Counts: " << simCount << "/" << simMax << "\n";
            
            if (Plains.gameOver()) {
                winner = Plains.declareWinner();
                if (winner == "Tigers win!\n") {
                    winCountTigers++;
                }
                else if (winner == "Cats win!\n") {
                    winCountCats++;
                }
                else if (winner == "Mice win!\n") {
                    winCountMice++;
                }
                else if (winner == "Bugs win!\n") {
                    winCountBugs++;
                }
                simCount++;
                break;
            }
            
        }
        
    }

    system("clear");
    cout << "\nResults of " << simCount << "/" << simMax << " iterations:\n" <<
    "Tigers: " << winCountTigers << "/" << simMax << " = " << ((double)winCountTigers/(double)simMax*100) << "%\n" <<
    "Cats: " << winCountCats << "/" << simMax << " = " << ((double)winCountCats/(double)simMax*100) << "%\n" <<
    "Mice: " << winCountMice << "/" << simMax << " = " << ((double)winCountMice/(double)simMax*100) << "%\n" <<
    "Bugs: " << winCountBugs << "/" << simMax << " = " << ((double)winCountBugs/(double)simMax*100) << "%\n\n";

    cout << "Press ENTER to exit...";
    cin.get();

    return 0;
}