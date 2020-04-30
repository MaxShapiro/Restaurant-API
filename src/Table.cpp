
#include "../include/Table.h"
#include "../include/Customer.h"
#include <vector>
#include <iostream>

using namespace std;

    //constructor
    Table::Table(int t_capacity): orderIndex(0),
        capacity(t_capacity),open(false), customersList(),orderList() {
    }
    //copy constructor
    Table::Table(const Table &copy): orderIndex(copy.orderIndex),
         capacity(copy.capacity), open(copy.open), customersList(), orderList() {
        for (const auto &i : copy.orderList)
            orderList.emplace_back(i);
        for (Customer *c: copy.customersList) {
            customersList.emplace_back(c->clone());
        }
    }
    //destructor
    Table:: ~Table(){
        for (auto &i : customersList) {
            if (i != nullptr && !i->getName().empty()) {
                delete i;
                i = nullptr;
            }
        }
        customersList.clear();
    }
//    move constructor
    Table::Table(Table&& other):
        orderIndex(other.orderIndex) ,capacity(other.capacity) ,open(other.open),customersList(other.customersList),
        orderList(other.orderList){
    }
    //copy assignment operator
    Table &Table::operator=(const Table &other) {
        if(this==&other)
            return *this;
        else {
            open=other.open;
            capacity=other.capacity;
            orderIndex = other.orderIndex;
            //clears customerList from this  & copy customerList from other
            for (auto &i : customersList)
                delete i;
            customersList.clear();
            for (auto i : other.customersList)
                customersList.push_back(i);
            //delete&copy orderList
            orderList.clear();
            for (const auto &i : other.orderList)
                orderList.push_back(i);
            return *this;
        }
    }
    //move assignment operator
    Table& Table::operator=(Table &&other) {
        if(this!=&other)
        {
            //changing the ownership on the variables
            orderIndex = other.orderIndex;
            capacity=other.capacity;
            open=other.open;
            customersList = other.customersList;
            for(int i=0;(unsigned) i<customersList.size();i++)
                delete customersList[i];
            customersList.clear();
            for(int i=0;(unsigned)i<other.customersList.size();i++)
                customersList.push_back(other.customersList[i]);
            orderList.clear();
            for(int i=0;(unsigned) i<other.orderList.size();i++){
                OrderPair o (other.orderList[i].first,other.orderList[i].second);
                orderList.push_back(o);
            }
        }
        return *this;
    }

    //methods
    int Table::getCapacity() const{
        return capacity;
    }
    void Table::addCustomer(Customer* customer){
        customersList.push_back(customer);
    }
    void Table::removeCustomer(int id) {
        for(int i=0;(unsigned)i<customersList.size();i++) {
            if (customersList[i]->getId() == id) {
                customersList.erase(customersList.begin() + i);
                break;
            }
        }
    }
    Customer* Table::getCustomer(int id){
        for (Customer* c: customersList){
            if (c->getId()==id)
                return c;
        }
        return nullptr;
    }
    std::vector<Customer*>& Table::getCustomers(){
        return customersList;
    }
    std::vector<OrderPair>& Table::getOrders() {
        return orderList;
    }
    void Table::order(const std::vector<Dish> &menu) {
        for (Customer *c : customersList) {
            std::vector<int> v = c->order(menu);
            for (int i = 0; (unsigned)i < v.size() && v[i]!=-1 ; i++)
                orderList.emplace_back(c->getId(), menu[v[i]]);
        }
    }
    void Table::openTable() {
        open=true;
    }

    void Table::closeTable() {
        for (Customer* c: customersList){
            delete c;
        }
        customersList.clear();
        orderIndex = 0;
        orderList.clear();
        open=false;
    }
    int Table::getBill() {
        int output=0;
        for(OrderPair o : orderList){
            output=output+o.second.getPrice();
        }
        return output;
    }
    bool Table::isOpen() {
        return open;
    }
    //2 auxiliary functions for Move Customer Action
    void Table::addCustomerDishes(Customer* c, vector<Dish> &menu) {
        for (int i: c->orderOfCustomer){
           orderList.emplace_back(OrderPair(c->getId(),menu[i]));
       }
    }
    void Table::removeCustomerDishes(Customer* c,vector<Dish> &menu) {
        vector<OrderPair> output;
        bool exist;
        int i;
        for (i = 0; (unsigned)i < orderList.size(); i++) {  //finding customer dishes
            exist = false;
            if (orderList[i].first == c->getId()) {
                exist = true;
            }
            if (!exist) {
                output.emplace_back(orderList[i].first, orderList[i].second);
            }
        }
        orderList.clear();
        for (const OrderPair &o : output)  //coping the new orderList
            orderList.push_back(o);
        output.clear();
    }
