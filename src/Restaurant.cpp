#include <iostream>

#include "../include/Restaurant.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <Restaurant.h>

#include "../include/Dish.h"

using namespace std;

Restaurant::Restaurant():numberOfCustomres(0), open(false),tables(), menu(), actionsLog(){
}

Restaurant::Restaurant(const std::string &configFilePath): numberOfCustomres(0), open(false), tables(), menu(), actionsLog() {
    vector<string> helpingDish;
    vector<string> tokens;
    string token;
    ifstream file(configFilePath);
    int locationCounter = 1;
    while (getline(file, token)) {
        if (locationCounter ==1 && token.front() != '#' && token.size()!=0)
            locationCounter++;
        else if (locationCounter == 2 && token.front() != '#' && token.size()!=0) {
            stringstream ss1(token);
            string tmp1;
            while (getline(ss1, tmp1, ',')) {
                auto *table = new Table(stoi(tmp1));
                tables.push_back(table);
            }
            locationCounter++;
        } else if (locationCounter == 3 && (token.front()) != '#' && token.size()!=0) {
            stringstream ss1(token);
            string tmp1;
            while (getline(ss1, tmp1, ',')) {
                helpingDish.push_back(tmp1);
            }
        }
    }

    for (int i = 0; (unsigned) i < helpingDish.size(); i++) {
        if ((unsigned) i != helpingDish.size() && (unsigned) i % 3 == 0) {
            string name = helpingDish[(unsigned) i];
            auto *dataType = new DishType;
            checkDataType(helpingDish[(unsigned) i + 1], *dataType);
            int price = stoi(helpingDish[(unsigned) i + 2]);
            menu.emplace_back(Dish((unsigned) i / 3, name, price, *dataType));
            delete dataType;
        }
    }
}


void Restaurant::start() {
    open = true;
    cout << "Restaurant is now open!"<< endl;
    string userInput;
    getline(cin, userInput);
    vector<Customer *> customerList;
    int tableID;
    while (userInput != "closeall") {
        // open table
        if (userInput.find("open") != string::npos) {
            stringstream ss1(userInput);
            string token1;
            vector<string> vs;
            while (getline(ss1, token1, ' ')) {
                vs.emplace_back(token1);
            }
            tableID = stoi(vs[1]);
            for (int i = 2; (unsigned)i < vs.size(); i++) {
                vector<string> vs2;
                stringstream ss2(vs[i]);
                string token2;
                while (getline(ss2, token2, ',')) {
                    vs2.push_back(token2);
                }
                if (vs2[1] == "veg" && tableID>=0 && !tables[tableID]->isOpen()) {
                    VegetarianCustomer* customer = new VegetarianCustomer(vs2[0], numberOfCustomres);
                    customerList.push_back(customer);
                    numberOfCustomres++;
                }
                if (vs2[1] == "alc" && tableID>=0 && !tables[tableID]->isOpen()) {
                    AlchoholicCustomer* customer = new AlchoholicCustomer(vs2[0], numberOfCustomres);
                    customerList.push_back(customer);
                    numberOfCustomres++;
                }
                if (vs2[1] == "chp" && tableID>=0 && !tables[tableID]->isOpen()) {
                    CheapCustomer* customer = new CheapCustomer(vs2[0], numberOfCustomres);
                    customerList.push_back(customer);
                    numberOfCustomres++;
                }
                if (vs2[1] == "spc" && tableID>=0 && !tables[tableID]->isOpen()) {
                    SpicyCustomer* customer = new SpicyCustomer(vs2[0], numberOfCustomres);
                    customerList.push_back(customer);
                    numberOfCustomres++;
                }
            }

            auto * openTable = new OpenTable(tableID, customerList);
            openTable->act(*this);
            actionsLog.emplace_back(openTable);
            openTable->userInput = userInput;
            customerList.clear();
        }
        // order
        if (userInput.find("order") != string::npos) {
            stringstream ss1(userInput);
            string token1;
            vector<string> vs;
            while (getline(ss1, token1, ' ')) {
                vs.emplace_back(token1);
            }
            int tableId = stoi(vs[1]);
            auto * order = new Order(tableId);
            order->act(*this);
            actionsLog.emplace_back(order);
            order->userInput = userInput;
        }
        // move customer
        if (userInput.find("move") != string::npos) {
            stringstream ss1(userInput);
            string token1;
            vector<string> vs;
            while (getline(ss1, token1, ' ')) {
                vs.push_back(token1);
            }
            int originTable = stoi(vs[1]);
            int destinationTable = stoi(vs[2]);
            int customerID = stoi(vs[3]);
            auto * moveCustomer = new MoveCustomer(originTable, destinationTable, customerID);
            moveCustomer->act(*this);
            actionsLog.emplace_back(moveCustomer);
            moveCustomer->userInput = userInput;
        }
        // close a table
        if (userInput.find("close") != string::npos) {
            stringstream ss1(userInput);
            string token1;
            vector<string> vs;
            while (getline(ss1, token1, ' ')) {
                vs.push_back(token1);
            }
            int tableId = stoi(vs[1]);
//            if (tableId > 0 && tableId<tables.size()) {
            auto *close = new Close(tableId);
            close->act(*this);
            actionsLog.emplace_back(close);
            close->userInput = userInput;
//            }
        }
        // print menu
        if (userInput == "menu"){
            auto * printMenu = new PrintMenu;
            printMenu->act(*this);
            actionsLog.emplace_back(printMenu);
            printMenu->userInput = userInput;
        }
        // print table status
        if (userInput.find("status") != string::npos) {
            stringstream ss1(userInput);
            string token1;
            vector<string> vs;
            while (getline(ss1, token1, ' ')) {
                vs.push_back(token1);
            }
            int tableId = stoi(vs[1]);
            auto *printTableStatus = new PrintTableStatus(tableId);
            printTableStatus->act(*this);
            actionsLog.emplace_back(printTableStatus);
            printTableStatus->userInput = userInput;
        }
        // print action log
        if (userInput == "log"){
            auto * printActionsLog = new PrintActionsLog;
            printActionsLog->act(*this);
            actionsLog.emplace_back(printActionsLog);
            printActionsLog->userInput = userInput;
        }
        // backup
        if (userInput == "backup"){
            auto * backupRestaurant = new BackupRestaurant;
            backupRestaurant->act(*this);
        }
        // restore
        if (userInput == "restore"){
            auto * restoreResturant = new RestoreResturant;
            restoreResturant->act(*this);
            restoreResturant->userInput = userInput;
        }
        getline(cin, userInput);
    }
    auto * closeAll = new CloseAll;
    actionsLog.emplace_back(closeAll);
    closeAll->userInput = userInput;
    closeAll->act(*this);
}

int Restaurant::getNumOfTables() const {
    if (!tables.empty())
        return static_cast<int>(tables.size());
    else return 0;
}

Table *Restaurant::getTable(int ind) {
    return tables[ind];
}

const std::vector<BaseAction *> &Restaurant::getActionsLog() const {
    return actionsLog;
}

std::vector<Dish> &Restaurant::getMenu() {
    return menu;
}

//copy constructor
Restaurant::Restaurant(const Restaurant &copy): numberOfCustomres(copy.numberOfCustomres) , open(copy.open), tables(), menu(), actionsLog() {
    for (Table* t: copy.tables) {
        if (t!= nullptr)
            tables.emplace_back(new Table(*t));
    }
    for (Dish d: copy.menu) {
        menu.emplace_back(d);
    }
    for (BaseAction* b: copy.actionsLog) {
        if (b!= nullptr) {
            actionsLog.emplace_back(b->clone());
        }
    }
}

Restaurant::~Restaurant() {
    for (Table* t: tables) {
        delete t;
    }
    for (BaseAction* b: actionsLog) {
        delete b;
    }
}
//move assignment operator
Restaurant &Restaurant::operator=(Restaurant &&other) {
    if(this!=&other)
    {
     numberOfCustomres=other.numberOfCustomres;
     open = other.open;
     for(int i=0; (unsigned)i<tables.size();i++)
         delete tables[i];
     tables.clear();
     for(int i=0; (unsigned)i<other.tables.size();i++)
         tables.push_back(other.tables[i]);
     menu.clear();
     for(int i=0; (unsigned)i<other.menu.size();i++)
         menu.push_back(menu[i]);
     actionsLog.clear();
     for(int i=0; (unsigned)i<other.actionsLog.size();i++)
         actionsLog.push_back(other.actionsLog[i]);
    }
    return *this;
}

//move consrtuctor
Restaurant::Restaurant(Restaurant &&other) : numberOfCustomres(other.numberOfCustomres)
 , open(other.open) , tables(other.tables) , menu(other.menu) , actionsLog(other.actionsLog){

}
//copy ass op
Restaurant &Restaurant::operator=(const Restaurant &other) {
    for (BaseAction *b: actionsLog)
        delete b;
    actionsLog.clear();
    for (BaseAction *b: other.actionsLog)
        actionsLog.emplace_back(b->clone());
    for (Table *t: tables) {
        delete t;
    }
    tables.clear();
    for (Table *t: other.tables) {
        tables.emplace_back(new Table(*t));
    }
    return *this;

}

void Restaurant::checkDataType(string s, DishType &dataType) { //auxiliary function for costructor
    if (s == "ALC")
        dataType = ALC;
    else if (s == "VEG")
        dataType = VEG;
    else if (s == "BVG")
        dataType = BVG;
    else
        dataType = SPC;
}

void Restaurant::addToActionLog(BaseAction *b) {
    actionsLog.emplace_back(b);
}



