#include <utility>

#include <iostream>

#include "../include/Dish.h"

using namespace std;
    //constructor
    Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type):
        id(d_id), name(std::move(d_name)), price(d_price), type(d_type) {
    }

int Dish::getId() const {
        return id;
    }

    string Dish::getName() const {
        return name;
    }

    int Dish::getPrice() const {
        return price;
    }

    DishType Dish::getType() const {
        return type;
    }


