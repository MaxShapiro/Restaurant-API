#include "../include/Action.h"
#include "../include/Table.h"
#include "../include/Restaurant.h"

#include <iostream>
#include <unordered_map>
#include <Action.h>

extern Restaurant* backup;

// constructor
BaseAction::BaseAction():userInput{}, errorMsg{}, status(PENDING){
}
//methods
ActionStatus BaseAction::getStatus() const {
    return status;
}
void BaseAction::act(Restaurant &restaurant) {
}
void BaseAction::complete() {
    status=COMPLETED;
}
void BaseAction::error(std::string errorMsg) {
    status=ERROR;
    setErrorMsg(errorMsg);
    std::cout<<errorMsg<<std::endl;
}
std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

BaseAction::~BaseAction() = default;

void BaseAction::setStatus(ActionStatus status) {
    BaseAction::status = status;
}

void BaseAction::setErrorMsg(const std::string &errorMsg) {
    BaseAction::errorMsg = errorMsg;
}

std::string BaseAction::getErrMsg() {
    return errorMsg;
}


//OpenTable
//constructor
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):
        tableId(id) , customers(customersList){
}

//copy constructor
OpenTable::OpenTable(const OpenTable &copy):BaseAction(copy), tableId(copy.tableId), customers(){
    for(int i=0; (unsigned)i<copy.customers.size(); i++)
        customers.push_back(copy.customers[i]);
    if (copy.getStatus()==COMPLETED)
        this->complete();
    else if (copy.getStatus() == ERROR) {
        this->setStatus(ERROR);
        this->getErrorMsg()=copy.getErrorMsg();
    }
    else
        this->getErrorMsg()=copy.getErrorMsg();
    this->userInput = copy.userInput;
}
BaseAction *OpenTable::clone() {
    return new OpenTable(*this);
}
//move constructor
OpenTable::OpenTable(OpenTable &&other ) : tableId(other.tableId)
    , customers( other.customers){}
//copy assignment operator
OpenTable &OpenTable::operator=(const OpenTable &other) {
    if (other.getStatus() == COMPLETED)
        complete();
    else
        this->getErrorMsg()=other.getErrorMsg();
    return *this;
}
//destructor
OpenTable::~OpenTable() = default;

void OpenTable::act(Restaurant &restaurant) {
    if (tableId+1>restaurant.getNumOfTables() || tableId<0) {  //checks if table exist
        error("Error: Table does not exist or is already open");
    }
    else {
        Table* t = restaurant.getTable(tableId);  //checks if table is already open
        if (t->isOpen()){
            error("Error: Table does not exist or is already open");
        }
        else {
            t->openTable();
            for (Customer *c : customers)  //assign the customer to the table
                t->addCustomer(c);
            complete();
        }
    }

}

std::string OpenTable::toString() const {
    return "This is an open table action";
}

//Order
//
//constructor
Order::Order(int id):tableId(id) {
}

void Order::act(Restaurant &restaurant) {
    if (tableId+1>restaurant.getNumOfTables() || tableId<0) {  //checks if table exist
        error("Error: Table does not exist or is not open");
    }
    else {
        Table* t = restaurant.getTable(tableId);  //checks if table is not  open
        if (!t->isOpen()) {
            error("Error: Table does not exist or is not open");
        }
        else {
            std::string name;
            std::string dish;
            std::string output;
            t->order(restaurant.getMenu());
            //taking the orders and print
            int i;
            for(i = t->orderIndex; (unsigned)i < t->getOrders().size(); i++){
                name = t->getCustomer(t->getOrders()[i].first)->getName();
                dish=t->getOrders()[i].second.getName();
                output = output + name + " ordered " + dish+"\n";
            }
            t->orderIndex = i;
            std::cout<<output;
            complete();
        }
    }
}

std::string Order::toString() const {
    return "This is an order action";
}

BaseAction *Order::clone() {
    return new Order(*this);
}

//MoveCustomer
//
//constructor
MoveCustomer::MoveCustomer(int src, int dst, int customerId):
        srcTable(src),dstTable(dst),id(customerId){
}

void MoveCustomer::act(Restaurant &restaurant) {
    if (id<0 || srcTable<0 || dstTable<0
            || dstTable>restaurant.getNumOfTables() || srcTable>restaurant.getNumOfTables()) {
        error("Error: Cannot move customer");  //exceptions
    } else {
        Table* origin = restaurant.getTable(srcTable);
        Table* dest = restaurant.getTable(dstTable);
        if ((!dest->isOpen()) | (!origin->isOpen()) | (unsigned)(dest->getCustomers().size() >= (unsigned)dest->getCapacity())
            | (origin->getCustomer(id) == nullptr)) {  //exceptions
            error("Error: Cannot move customer");
        } else {
            Customer *c = origin->getCustomer(id);  //moving the customer
            dest->addCustomerDishes(c, restaurant.getMenu());
            dest->addCustomer(c);
            dest->orderIndex = static_cast<int>(dest->orderIndex + c->orderOfCustomer.size());
            origin->removeCustomerDishes(c, restaurant.getMenu());
            origin->removeCustomer(id);
            origin->orderIndex = static_cast<int>(origin->orderIndex - c->orderOfCustomer.size());
            if (origin->getCustomers().empty()) {  //closing table if empty
                origin->closeTable();
            }
            complete();
        }
    }
}

std::string MoveCustomer::toString() const {
    return "This is a move customer action";
}

BaseAction *MoveCustomer::clone() {
    return new MoveCustomer(*this);
}

//Close
//
//constructor
Close::Close(int id):tableId(id) {
}

void Close::act(Restaurant &restaurant) {
    if(tableId+1>restaurant.getNumOfTables()||tableId<0) {   //exceptions
        error("Error: Table does not exist or is not open");
    }
    else {
        Table* t = restaurant.getTable(tableId);
        if(!t->isOpen()){
            error("Error: Table does not exist or is not open");
            }
        else {
            //prints the bill and closing the table
            std::cout << "Table " << tableId << " was closed. Bill " << t->getBill() << "NIS" << std::endl;
            t->closeTable();
            restaurant.numberOfCustomres = static_cast<int>(restaurant.numberOfCustomres - restaurant.getTable(tableId)->getCustomers().size());
            complete();
        }
    }
}

std::string Close::toString() const {
    return "This is a close action";
}

BaseAction *Close::clone() {
    return new Close(*this);
}

//CloseAll
//
//constructor
CloseAll::CloseAll() = default;

void CloseAll::act(Restaurant &restaurant) {
    for(int i=0;i<restaurant.getNumOfTables();i++){
        Table* t= restaurant.getTable(i);
        if(t->isOpen()){
            //printing the bills and closing the tables
            std::cout<<"Table "<<i<<" was closed. Bill "<<t->getBill()<<"NIS"<<std::endl;
            t->closeTable();
        }
    }
    complete();
}

std::string CloseAll::toString() const {
    return "This is a close all action";
}

BaseAction *CloseAll::clone() {
    return new CloseAll(*this);
}

//PrintMenu
//
//constructor
PrintMenu::PrintMenu() = default;

void PrintMenu::act(Restaurant &restaurant) {
    for(const Dish &d : restaurant.getMenu()){
        if(d.getType()==0)
            std::cout<<d.getName()<<" "<<"VEG "<<d.getPrice()<<"NIS"<<std::endl;
        else if(d.getType()==1)
            std::cout<<d.getName()<<" "<<"SPC "<<d.getPrice()<<"NIS"<<std::endl;
        else if (d.getType()==2)
            std::cout<<d.getName()<<" "<<"BVG "<<d.getPrice()<<"NIS"<<std::endl;
        else
            std::cout<<d.getName()<<" ALC "<<d.getPrice()<<"NIS"<<std::endl;
    }
    complete();
}

std::string PrintMenu::toString() const {
    return "This is a menu print action";
}

BaseAction *PrintMenu::clone() {
    return new PrintMenu(*this);
}

//PrintTableStatus
//
//constructor
PrintTableStatus::PrintTableStatus(int id): tableId(id) {
}

void PrintTableStatus::act(Restaurant &restaurant) {
    Table* t= restaurant.getTable(tableId);
    if(!t->isOpen())
        std::cout<<"Table "<<tableId<<" status: closed"<<std::endl;
    else{
        std::cout<<"Table "<<tableId<<" status: open\n"<<"Customers:" <<std::endl;
        for(Customer* c: t->getCustomers())
            std::cout<<c->getId()<<" "<<c->getName()<<std::endl;
        std::cout<<"Orders:"<<std::endl;
        for(OrderPair o : t->getOrders())
            std::cout<<o.second.getName()<<" "<<o.second.getPrice()<<"NIS "<<o.first<<std::endl;
        std::cout << "Current Bill: " << t->getBill() << "NIS" << std::endl;

    }
    complete();
}

std::string PrintTableStatus::toString() const {
    return "This is a status action";
}

BaseAction *PrintTableStatus::clone() {
    return new PrintTableStatus(*this);
}

//PrintActionsLog
//
//constructor
PrintActionsLog::PrintActionsLog() = default;

void PrintActionsLog::act(Restaurant &restaurant) {
    std::string output;
    for(BaseAction* b : restaurant.getActionsLog()) {
        if (b->getStatus() == PENDING) {
            output = "Pending";
            std::cout << b->userInput << " " << output << std::endl;
        } else if (b->getStatus() == COMPLETED) {
            output = "Completed";
            std::cout << b->userInput << " " << output << std::endl;
        } else {
            output = b->getErrMsg();
            std::cout << b->userInput << " " << output << std::endl;
        }
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    return "This is a print log action";
}

BaseAction *PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

//BackupRestaurant
//
//constructor
BackupRestaurant::BackupRestaurant() = default;


//copy constructor
BackupRestaurant::BackupRestaurant(const BackupRestaurant &copy): BaseAction(copy){
}

void BackupRestaurant::act(Restaurant &restaurant){
    if (backup != nullptr)
        delete backup;
    backup = new Restaurant(restaurant);
    complete();
    restaurant.addToActionLog(this);
    this->userInput = "backup";
}

std::string BackupRestaurant::toString() const {
    return "This is a backup action";
}

BaseAction *BackupRestaurant::clone() {
    return new BackupRestaurant(*this);
}

//RestoreResturant
//constructor
RestoreResturant::RestoreResturant() = default;

RestoreResturant::RestoreResturant(const RestoreResturant &other) = default;

void RestoreResturant::act(Restaurant &restaurant) {
    if (backup == nullptr) {
        error("Error: No backup available");
        restaurant.addToActionLog(this);
    }
    else{
        restaurant = *backup;
        complete();
        restaurant.addToActionLog(this);
    }
}

std::string RestoreResturant::toString() const {
    return "This is a restore action";
}

BaseAction *RestoreResturant::clone() {
    return new RestoreResturant(*this);
}

