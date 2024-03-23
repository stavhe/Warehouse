//#pragma once
#include <iostream>
#include "../include/Order.h"
#include <string>
using std::string;
using namespace std;


Order::Order(int _id, int _customerId, int _distance) :
    id( _id),
    customerId(_customerId),
    distance(_distance),
    status(OrderStatus(0)),
    collectorId(NO_VOLUNTEER),
    driverId(NO_VOLUNTEER)
    {}

int Order::getId() const {
    return id;
}

int Order::getCustomerId() const {
    return customerId;
}

void Order::setStatus(OrderStatus status){
    this->status = status;
}

void Order::setCollectorId(int collectorId) {
    this->collectorId = collectorId;
}

void Order::setDriverId(int driverId) {
    this->driverId = driverId;
}

 int Order::getCollectorId() const {
    return collectorId;
 }

int Order::getDriverId() const {
    return driverId;
}  

int Order::getDistance () const {
    return distance;
}
        
OrderStatus Order::getStatus() const{
    return status;
}

const string Order::toString() const{
    string dId = "";
    if (driverId == NO_VOLUNTEER)
        dId = "None";
    else
         dId = "" + getDriverId();

    string cId = "" + getCollectorId();
    if (collectorId == NO_VOLUNTEER)
        cId = "None";
    
    return "OrderId: " + to_string(getId())  + 
    "  CustomerId: " + to_string(getCustomerId()) +
    "  OrderStatus: " + stringStatus() ;
}  

string Order::stringStatus () const {
    string stdStatus = "";
    OrderStatus currStatus = getStatus();
    if (currStatus == OrderStatus(0))
        stdStatus = "Pending";
    else {
        if (currStatus == OrderStatus(1))
            stdStatus = "Collecting";
        else {
            if (currStatus == OrderStatus(2))
                stdStatus = "Delivering";
            else
                stdStatus = "Completed";
        }   
    }
    return stdStatus;
}


