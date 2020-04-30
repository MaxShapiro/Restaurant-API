#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Dish.h"

class Customer{
public:
	Customer(std::string c_name, int c_id); //constructor
	virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
	virtual std::string toString() const = 0;
	int getId() const;
	virtual ~Customer();
	virtual Customer* clone() = 0;
	std::string getName() const;
	std::vector<int> orderOfCustomer; //keeps for each customer his list of orders
private:
	const std::string name;
	const int id;
};


class VegetarianCustomer : public Customer {
public:
	VegetarianCustomer(std::string name, int id);
	std::vector<int> order(const std::vector<Dish> &menu);
	std::string toString() const;
	~VegetarianCustomer() override;
	virtual Customer* clone() override;
private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
	std::vector<int> order(const std::vector<Dish> &menu);
	std::string toString() const;
	~CheapCustomer() override;
	virtual Customer* clone() override;
private:
	bool firstOrder;
};


class SpicyCustomer : public Customer {
public:
	SpicyCustomer(std::string name, int id);
	std::vector<int> order(const std::vector<Dish> &menu);
	std::string toString() const;
	~SpicyCustomer() override;
	virtual Customer* clone() override;
private:
	bool firstOrder;
};


class AlchoholicCustomer : public Customer {
public:
	AlchoholicCustomer(std::string name, int id);
	std::vector<int> order(const std::vector<Dish> &menu);
	std::string toString() const;
	~AlchoholicCustomer() override;
	virtual Customer* clone() override;
private:
    std::vector<std::pair<int,int>> alcMenu; // <Price, id>
    std::pair<int, int> orderPair; // <Price, id>
	int orderNumber;
	int cheapestPrice;
	int cheapestId;
	int currentCheapestIndex;
};


#endif