#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{		
public:
	Restaurant(); // empty constructor
	Restaurant(const std::string &configFilePath); // constructor
	void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();

    Restaurant(const Restaurant &copy); //  copy constructor
    virtual ~Restaurant(); //destructor
    Restaurant& operator = (const Restaurant &other); // copy ass operator
    Restaurant&operator=(Restaurant &&other);    // move ass op
    Restaurant(Restaurant&& other);         //move constructor
    void checkDataType (std::string s , DishType &dataType);
    void addToActionLog(BaseAction* b);
    int numberOfCustomres;
private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif