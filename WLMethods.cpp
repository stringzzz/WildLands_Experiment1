/*

    Part of WildLand, an artificial organism environment, class methods part
    Copyright (C) 2021 stringzzz, Ghostwarez Co.

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

*/

#include <iostream>
#include <vector>
#include <ctime>
#include "WLHeader.h"
using namespace std;

Land::Land() {
    //Add initial organisms to vector
    Organism b1 = { 'b', 0, 19, 20 };
    Organisms.push_back(b1);
    
    Organism m1 = { 'm', 0, 0, 20 };
    Organisms.push_back(m1);
    
    Organism c1 = { 'c', 19, 19, 20 };
    Organisms.push_back(c1);
    
    Organism t1 = { 't', 19, 0, 20 };
    Organisms.push_back(t1);
}

void Land::move() {
    //Organism moves at random, can eat food or other ogranisms according to food chain rules
    for (int i = 0; i < Organisms.size(); i++) {
        if (Organisms[i].HP == 0) {
            continue;
        }
        
        bool cont1 = 0;
        int m, n;
        bool checked[4] = { 0, 0, 0, 0 }; //For checking off directions already checked
        bool validMove = 0;
        
        while (!validMove) {
            int direction = rand() % 4;
            if (checked[direction]) {
                continue;
            }
            
            switch (direction) {
            case 0:
                //Up
                m = -1;
                n = 0;
                break;
            case 1:
                //Right
                m = 0;
                n = 1;
                break;
            case 2:
                //Left
                m = 0;
                n = -1;
                break;
            case 3:
                //Down
                m = 1;
                n = 0;
                break;
            }
            
            if ((Organisms[i].x + m > 19) || (Organisms[i].y + n > 19) || (Organisms[i].x + m < 0) || (Organisms[i].y + n < 0)) {
                //Prevent moving past the boundaries
                if (!checked[direction]) {
                    checked[direction] = 1;
                }
                if (checked[0] && checked[1] && checked[2] && checked[3]) {
                    cont1 = 1;
                    break;
                }
                continue;
            }
            
            if (Landscape[Organisms[i].x + m][Organisms[i].y + n] == ' ') {
                validMove = 1;
                break;
            }
            
            if (Landscape[Organisms[i].x + m][Organisms[i].y + n] == '.') {
                //Anything can eat '.', plants
                Organisms[i].HP += 5;
                validMove = 1;
                break;
            }
            
            if (Organisms[i].type == 't') {
                //Tiger, can eat cats, mice, bugs
                if (Landscape[Organisms[i].x + m][Organisms[i].y + n] == 'c') {
                    Organisms[i].HP += 5;
                    int index = searchCoords('c', Organisms[i].x + m, Organisms[i].y + n);
                    Organisms[index].HP = 0;
                }
                else if (Landscape[Organisms[i].x + m][Organisms[i].y + n] == 'm') {
                    Organisms[i].HP += 5;
                    int index = searchCoords('m', Organisms[i].x + m, Organisms[i].y + n);
                    Organisms[index].HP = 0;
                }
                else if (Landscape[Organisms[i].x + m][Organisms[i].y + n] == 'b') {
                    Organisms[i].HP += 5;
                    int index = searchCoords('b', Organisms[i].x + m, Organisms[i].y + n);
                    Organisms[index].HP = 0;
                }
                else {
                    if (!checked[direction]) {
                        checked[direction] = 1;
                    }
                    if (checked[0] && checked[1] && checked[2] && checked[3]) {
                        cont1 = 1;
                        break;
                    }
                    continue;
                }
            }
            else if (Organisms[i].type == 'c') {
                //Cat, can eat mice, bugs
                if (Landscape[Organisms[i].x + m][Organisms[i].y + n] == 'm') {
                    Organisms[i].HP += 5;
                    int index = searchCoords('m', Organisms[i].x + m, Organisms[i].y + n);
                    Organisms[index].HP = 0;
                }
                else if (Landscape[Organisms[i].x + m][Organisms[i].y + n] == 'b') {
                    Organisms[i].HP += 5;
                    int index = searchCoords('b', Organisms[i].x + m, Organisms[i].y + n);
                    Organisms[index].HP = 0;
                }
                else {
                    if (!checked[direction]) {
                        checked[direction] = 1;
                    }
                    if (checked[0] && checked[1] && checked[2] && checked[3]) {
                        cont1 = 1;
                        break;
                    }
                    continue;
                }
            }
            else if (Organisms[i].type == 'm') {
                //Mice, can eat bugs
                if (Landscape[Organisms[i].x + m][Organisms[i].y + n] == 'b') {
                    Organisms[i].HP += 5;
                    int index = searchCoords('b', Organisms[i].x + m, Organisms[i].y + n);
                    Organisms[index].HP = 0;
                }
                else {
                    if (!checked[direction]) {
                        checked[direction] = 1;
                    }
                    if (checked[0] && checked[1] && checked[2] && checked[3]) {
                        cont1 = 1;
                        break;
                    }
                    continue;
                }
            }
            else {
                if (!checked[direction]) {
                    checked[direction] = 1;
                }
                if (checked[0] && checked[1] && checked[2] && checked[3]) {
                    cont1 = 1;
                    break;
                }
                continue;
            }
            validMove = 1;
        }
        
        if (cont1) {
            continue;
        }
        
        //Move the organism to new location
        Landscape[Organisms[i].x][Organisms[i].y] = ' ';
        Organisms[i].x += m;
        Organisms[i].y += n;
        Organisms[i].HP--;
        Landscape[Organisms[i].x][Organisms[i].y] = Organisms[i].type;
    }
    
}

void Land::reproduce() {
    //If total population size is less than or equal to 120, can reproduce
    //An organism can only reproduce if it has enough health for it's type, Tigers >= 27, Cats >= 25, Mice >= 25, Bugs >= 23
    
    int size = Organisms.size();
    
    if (size > 120) {
        return;
    }
    
    for (int i = 0; i < size; i++) {
    
        if (((Organisms[i].HP >= 27 && Organisms[i].type == 't') || (Organisms[i].HP >= 25 && (Organisms[i].type == 'c' || Organisms[i].type == 'm')) || (Organisms[i].HP >= 23 && Organisms[i].type == 'b'))) {
            int m, n;
            bool cont1 = 0;
            bool checked[8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; //For checking off locations already checked
            bool validReproduce = 0;
            
            while (!validReproduce) {
                int location = rand() % 8;
                if (checked[location]) {
                    continue;
                }

                switch (location) {
                case 0:
                    //Above
                    m = -1;
                    n = 0;
                    break;
                case 1:
                    //Top left corner
                    m = -1;
                    n = -1;
                    break;
                case 2:
                    //Top right corner
                    m = -1;
                    n = 1;
                    break;
                case 3:
                    //Left
                    m = 0;
                    n = -1;
                    break;
                case 4:
                    //Right
                    m = 0;
                    n = 1;
                    break;
                case 5:
                    //Below
                    m = 1;
                    n = 0;
                    break;
                case 6:
                    //Bottom left corner
                    m = 1;
                    n = -1;
                    break;
                case 7:
                    //Bottom right corner
                    m = 1;
                    n = 1;
                    break;
                }
                
                if ((Organisms[i].x + m > 19) || (Organisms[i].y + n > 19) || (Organisms[i].x + m < 0) || (Organisms[i].y + n < 0)) {
                
                    //Prevent reproducing past boundaries
                    if (!checked[location]) {
                        checked[location] = 1;
                    }
                    if (checked[0] && checked[1] && checked[2] && checked[3] && checked[4] && checked[5] && checked[6] && checked[7]) {
                        cont1 = 1;
                        break;
                    }
                    continue;
                }
                
                if (Landscape[Organisms[i].x + m][Organisms[i].y + n] == 't' || Landscape[Organisms[i].x + m][Organisms[i].y + n] == 'c' || Landscape[Organisms[i].x + m][Organisms[i].y + n] == 'm' || Landscape[Organisms[i].x + m][Organisms[i].y + n] == 'b' || Landscape[Organisms[i].x + m][Organisms[i].y + n] == '.') {
                    
                    //Can't reproduce to already filled locations
                    if (!checked[location]) {
                        checked[location] = 1;
                    } 
                    
                }
                
                if (checked[0] && checked[1] && checked[2] && checked[3] && checked[4] && checked[5] && checked[6] && checked[7] && checked[8]) {

                    cont1 = 1;
                    break;
                 }
                 
                 validReproduce = 1;
                 continue;
                 
            }
            
            if (cont1) {
                continue;
            }
            
            //Create new copy of organism
            Organism o2 = { Organisms[i].type, Organisms[i].x + m, Organisms[i].y + n, 20 };
            Organisms.push_back(o2);
            Landscape[Organisms[i].x + m][Organisms[i].y + n] = Organisms[i].type;
            
            //Add to population count
            if (Organisms[i].type == 't') {
                tigers++;
            }
            else if (Organisms[i].type == 'c') {
                cats++;
            }
            else if (Organisms[i].type == 'm') {
                mice++;
            }
            else {
                bugs++;
            }
            
        }
    }
}

void Land::die() {
    //Loop through all organisms and remove those with zero HP
    for (int i = Organisms.size() - 1; i >= 0; i--) {
    
        if (Organisms.size() == 1) {
            survivor = Organisms[0].type;
        }
        
        if (Organisms[i].HP == 0) {
            int indexes = searchCoordsAll(Organisms[i].x, Organisms[i].y);
            
            if (indexes == 1) {
                //If one organism in location, remove it's character from Landscape, otherwise organism that ate it is in that spot
                Landscape[Organisms[i].x][Organisms[i].y] = ' ';
            }
            
            //Lower population counts for removed organisms
            if (Organisms[i].type == 't') {
                tigers--;
            }
            else if (Organisms[i].type == 'c') {
                cats--;
            }
            else if (Organisms[i].type == 'm') {
                mice--;
            }
            else {
                bugs--;
            }
            
            Organisms.erase(Organisms.begin() + i);
            
            if (Organisms.size() == 0) {
                break;
            }
            
        }
    }
}

bool Land::gameOver() {
    //Return true if no organisms left
    if (Organisms.size() == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

void Land::growFood(int amount) {
    //Choose random empty locations to grow food '.'
    for (int n = 1; n <= amount; n++) {
        int i = rand() % 20;
        int j = rand() % 20;
        if (Landscape[i][j] == ' ') {
            Landscape[i][j] = '.';
        }
    }
}

void Land::displayLand() {

    system("clear"); //Comment out for Windows use
    //system("cls"); //WINDOWS USAGE
    
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            cout << Landscape[i][j] << ' ';
        }
        cout << "\n";
    }
}

void Land::displayPopCounts() {
    cout << "Tigers: " << tigers << "\nCats: " << cats << "\nMice: " << mice << "\nBugs: " << bugs << "\n";
}

int Land::searchCoords(char type, int x, int y) {
    //Returns index of organism at certain coordinates, for setting HP of eaten ones to zero
    for (int i = 0; i < Organisms.size(); i++) {
        if (Organisms[i].type == type && Organisms[i].x == x && Organisms[i].y == y) {
            return i;
        }
    }
    
    return 0;
}

int Land::searchCoordsAll(int x, int y) {
    //Returns number of organims with matched coordinates, for use in die() when determining if an 
    //empty spot should replace dead organism, or leave it if another ate it
    int amount = 0;
    for (int i = 0; i < Organisms.size(); i++) {
        if (Organisms[i].x == x && Organisms[i].y == y) {
            amount++;
        }
    }
    
    return amount;
}

string Land::declareWinner() {
    //Survivor is the last organism remaining
    string winner;
    
    if (survivor == 't') {
        winner = "Tigers";
    }
    else if (survivor == 'c') {
        winner = "Cats";
    }
    else if (survivor == 'm') {
        winner = "Mice";
    }
    else if (survivor == 'b') {
        winner = "Bugs";
    }
    else {
        return "Nobody wins!\n";
    }
    
    return (winner + " win!\n");
}

void betAftermath(char choice, string winner, int& credits, int bet) {
    int winnings = 0;
    bool win = 0;
    
    if (choice == 't' && winner == "Tigers win!\n") {
        winnings = bet * 2;
        win = 1;
    }
    else if (choice == 'c' && winner == "Cats win!\n") {
        winnings = bet * 3;
        win = 1;
    }
    else if (choice == 'm' && winner == "Mice win!\n") {
        winnings = bet * 4;
        win = 1;
    }
    else if (choice == 'b' && winner == "Bugs win!\n") {
        winnings = bet * 5;
        win = 1;
    }
    else if (winner == "Nobody wins!\n") {
        cout << "Nobody won!\n";
    }
    
    if (win) {
        cout << "You won " << winnings << "!\n";
        credits += winnings;
    }
    else {
        cout << "You lost " << bet << "!\n";
        credits -= bet;
    }
}
