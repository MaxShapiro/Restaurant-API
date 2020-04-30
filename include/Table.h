#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
//#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);  //constructor
    Table(const Table &copy); //copy constructor
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    virtual  ~Table();  //destructor
    int getCapacity() const;
    void addCustomerDishes(Customer* c, std::vector<Dish> &menu);
    void removeCustomerDishes(Customer* c,std::vector<Dish> &menu);
    Table&operator=(Table &&other);     //move assignment operator
    //auxiliary functions for Move Customer Action
    Table(Table&& other);  //move constructor
    int orderIndex; //auxilary function for orderList printing
    Table& operator=(const Table &other);  //copy assignment operator
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif