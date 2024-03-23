//#pragma once
#include <iostream>
#include "../include/Customer.h"
#include <vector>
#include <string>
using std::string;
using std::vector;

Customer::Customer(int _id, const string &_name, int _locationDistance, int _maxOrders) : 
    id(_id), name(_name), locationDistance( _locationDistance), maxOrders(_maxOrders), ordersId()
{}

const string& Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

int Customer::getCustomerDistance() const {
    return locationDistance;
}

int Customer:: getMaxOrders() const {
    return maxOrders;
}

int Customer:: getNumOrders() const {
    return ordersId.size();
}

bool Customer::canMakeOrder() const {
    if (getNumOrders() < maxOrders)
        return true;
    return false;
}

const vector<int> & Customer::getOrdersIds() const {
    return ordersId;
}

int Customer::addOrder(int orderId) {
    if (canMakeOrder() ){
        ordersId.push_back(orderId);
        return orderId;
    }
    return -1;
}

SoldierCustomer::SoldierCustomer (int _id, const string & _name, int _locationDistance, int _maxOrders):
    Customer(_id, _name, _locationDistance, _maxOrders)
{}

CivilianCustomer:: CivilianCustomer (int _id, const string & _name, int _locationDistance, int _maxOrders):
    Customer(_id, _name, _locationDistance, _maxOrders)
{}

CivilianCustomer* CivilianCustomer:: clone() const {
    return new CivilianCustomer (*this);
}

SoldierCustomer* SoldierCustomer:: clone() const {
    return new SoldierCustomer (*this);
}
