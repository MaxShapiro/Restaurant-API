#include <utility>

#include <utility>

#include <utility>

#include <utility>

#include <utility>

#include <iostream>
#include <vector>
#include <algorithm>
#include <Customer.h>


#include "../include/Table.h"
#include "../include/Dish.h"
#include "../include/Customer.h"

using namespace std;
//Constructor
Customer::Customer(std::string c_name, int c_id): orderOfCustomer(),
        name(std::move(c_name)),id(c_id){
}
//methods

std::string Customer::getName() const {
    return name;
}
int Customer::getId() const {
    return id;
}

Customer::~Customer() = default;


//VegetarianCustomer
//
//VegetarianCustomer constructor
VegetarianCustomer::VegetarianCustomer(std::string name, int id) : Customer(std::move(name), id) {
}
//methods
std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> output;
    int numOfVegDishes = 0;
    for (int i = 0; (unsigned)i < menu.size(); i++) {
        if (menu[i].getType() == VEG) {
            numOfVegDishes++;
        }
    }
    int minIDFood = -1;
    int maxPrice = -1;
    int BVGID = -1;
    for (int i = 0; (unsigned)i < menu.size(); i++) {
        if(menu[i].getType() == VEG){         //searching for the smallest ID veg dish
            if(minIDFood==-1)               //for thr first initializing minID
                minIDFood=menu[i].getId();
            else if(menu[i].getId()<minIDFood){
                minIDFood=menu[i].getId();
            }
        }
        else if(menu[i].getType()==BVG && numOfVegDishes>0){    //searching for the most e veg expansive BVG
            if(menu[i].getPrice()>maxPrice) {
                maxPrice = menu[i].getPrice();
                BVGID = menu[i].getId();
            }
        }
    }
    output.push_back(minIDFood);
    output.push_back(BVGID);
    for (int i: output)
        orderOfCustomer.push_back(i);
    return output;
}

std::string VegetarianCustomer::toString() const {
    return "Customer " + getName() + ", " + "He likes vegetarian food.";
}

Customer *VegetarianCustomer::clone() {
    return new VegetarianCustomer(*this);
}

VegetarianCustomer::~VegetarianCustomer() = default;


// CheapCustomer
//
// CheapCustomer constructor
CheapCustomer::CheapCustomer(std::string name, int id) :Customer(std::move(name), id), firstOrder(true) {
}
//methods
std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> output;
    if (firstOrder) {
        int cheapest = 0;
        int resultID = 0;
        bool firstLoop = true;
        for (int i = 0;(unsigned)i< menu.size(); i++) { //looking for the cheapest dish in the menu
            if (firstLoop) {
                firstLoop = false;
                cheapest = menu[i].getPrice();
                resultID = menu[i].getId();
            } else if (menu[i].getPrice() < cheapest) {
                cheapest = menu[i].getPrice();
                resultID = menu[i].getId();
            }
        }
        output.push_back(resultID);
    }
    firstOrder= false;
    for (int i: output)
        orderOfCustomer.push_back(i);
    return  output;
}

std::string CheapCustomer::toString() const {
    return "Customer " + getName() + ", " + "He likes cheap food.";
}

Customer *CheapCustomer::clone() {
    return new CheapCustomer(*this);
}

CheapCustomer::~CheapCustomer() = default;

//Spicy Customer
//
//spicy customer constructor
SpicyCustomer::SpicyCustomer(std::string name, int id) : Customer(std::move(name), id), firstOrder(true) {
}
//methods
std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu) {
    std::vector<int> output;
    int numberOfSpicy = 0;
    for (const auto &i : menu) {
        if (i.getType() == SPC) {
            numberOfSpicy++;
        }
    }
    if (numberOfSpicy>0) {
        if (firstOrder) {
            firstOrder = false;
            int expansive = -1;
            int resultFoodID = -1;
            bool firstLoop = true;
            for (const auto &i : menu) {  //looking for the most expansive spicy dish
                if (i.getType() == SPC) {
                    if (firstLoop) {
                        firstLoop = false;
                        resultFoodID = i.getId();
                        expansive = i.getPrice();
                    } else if (expansive < i.getPrice()) {
                        resultFoodID = i.getId();
                        expansive = i.getPrice();
                    }
                }
            }
            for (int i: output)
                orderOfCustomer.push_back(i);
            output.push_back(resultFoodID);
        } else { // not first order
            int cheapest = -1;
            int resultBVGID = -1;
            for (const auto &i : menu) {
                if (i.getType() == BVG) {  //searching for the cheapest beverage
                    if (cheapest == -1) {             //first loop
                        cheapest = i.getPrice();
                        resultBVGID = i.getId();
                    } else if (cheapest > i.getPrice()) {
                        cheapest = i.getPrice();
                        resultBVGID = i.getId();
                    }
                }
            }
            output.push_back(resultBVGID);
        }
        for (int i: output)
            orderOfCustomer.push_back(i);
    }
    return output;
}

std::string SpicyCustomer::toString() const {
    return "Customer " + getName() + ", " + "He likes spicy food.";
}

Customer *SpicyCustomer::clone() {
    return new SpicyCustomer(*this);
}

SpicyCustomer::~SpicyCustomer() = default;

//AlchoholicCustomer
//
//AlchoholicCustomer constructor
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id) : Customer(std::move(name), id), alcMenu(), orderPair(), orderNumber(0), cheapestPrice(0), cheapestId(0), currentCheapestIndex(0) {

}
std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu) {

    std::vector<int> output;
    // if it is the first order, well make an alcoholoc menu
    if (orderNumber == 0){
        orderNumber++;
//         making menu
        for (const Dish &d: menu){
            if (d.getType() ==  ALC){
                orderPair.first = d.getPrice();
                orderPair.second = d.getId();
                alcMenu.emplace_back(orderPair);
            }
        }
        // sorting the menu;
        std::sort(alcMenu.begin(), alcMenu.end());
        //finding cheapest
        cheapestPrice = alcMenu[0].first;
        cheapestId = alcMenu[0].second;
        output.emplace_back(cheapestId);
        orderOfCustomer.emplace_back(alcMenu[currentCheapestIndex].second);
        currentCheapestIndex++;
    } else if ((unsigned)currentCheapestIndex == (unsigned)alcMenu.size()){
        output.emplace_back(-1);
    } else {
        output.emplace_back(alcMenu[currentCheapestIndex].second);
        orderOfCustomer.emplace_back(alcMenu[currentCheapestIndex].second);
        currentCheapestIndex++;
    }
    return output;
}

std::string AlchoholicCustomer::toString() const {
    return "Customer " + getName() + ", " + "He likes alcohol.";
}


Customer *AlchoholicCustomer::clone() {
    return new AlchoholicCustomer(*this);
}

AlchoholicCustomer::~AlchoholicCustomer() = default;