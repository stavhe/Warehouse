//#pragma once
#include <iostream>
#include "../include/Volunteer.h"
#include <vector>
#include <string>
using std::string;
using std::vector;
using namespace std;



Volunteer::Volunteer(int _id, const string & _name) :
  completedOrderId (NO_ORDER), activeOrderId(NO_ORDER) , id (_id), name (_name)
{}

int Volunteer::getId() const {
    return id;
}

const string & Volunteer::getName() const {
    return name;
}

 int Volunteer::getActiveOrderId() const {
    return activeOrderId;
 }

int Volunteer::getCompletedOrderId() const {
    return completedOrderId;
}

bool Volunteer::isBusy() const {
    if (activeOrderId == NO_ORDER)
        return false;
    return true;
}

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int _coolDown) :
    Volunteer(id, name), coolDown ( _coolDown) ,timeLeft(_coolDown)
    {}

CollectorVolunteer* CollectorVolunteer:: clone() const  {
    return new CollectorVolunteer (*this);
}

void CollectorVolunteer::step()  {
    if (decreaseCoolDown()) {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

int CollectorVolunteer::getCoolDown() const {
    return coolDown;
}

int CollectorVolunteer::getTimeLeft() const {
    return timeLeft;
}

bool CollectorVolunteer::decreaseCoolDown() {
    timeLeft -=1;
    if ( timeLeft ==0)
        return true;
    return false;
}

bool CollectorVolunteer::hasOrdersLeft() const  {
    return true;
}

bool CollectorVolunteer::canTakeOrder(const Order &order) const  {
    if((order.getStatus()!= OrderStatus(0))| isBusy())
        return false;
    return true;
}

void CollectorVolunteer::acceptOrder(const Order &order) {
    activeOrderId = order.getId(); 
    timeLeft=coolDown;
}

string CollectorVolunteer::toString() const {
    string busy= "false";
    string orderId= "none";
    string dis= "none";
    if (isBusy()){
        busy="true";
        orderId= to_string(activeOrderId);
        dis= to_string(getTimeLeft());
    }
    return "VolunteerId: " + to_string(getId())  + "  isBusy: " +busy + 
    "  orderId: " + orderId + 
    "  Time left: " + dis+
    "  Orders left: no limit";
}

LimitedCollectorVolunteer:: LimitedCollectorVolunteer(int id, const string &name, int coolDown ,int _maxOrders):
    CollectorVolunteer(id,name,coolDown), maxOrders(_maxOrders), ordersLeft (_maxOrders)
{}

 LimitedCollectorVolunteer * LimitedCollectorVolunteer::clone() const {
    return new LimitedCollectorVolunteer (*this);
}
 
 bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    if(ordersLeft>0)
       return true;
    return false;
}
 
 bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
    if((order.getStatus()!= OrderStatus(0))| isBusy() | !hasOrdersLeft())
        return false;
    return true;
}

void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
    CollectorVolunteer::acceptOrder(order);
    ordersLeft -- ;
}

int LimitedCollectorVolunteer::getMaxOrders() const {
    return maxOrders;
}
 
int LimitedCollectorVolunteer::getNumOrdersLeft() const{
    return ordersLeft;
}

string LimitedCollectorVolunteer::toString() const {
    string busy= "false";
    string orderId= "none";
    string dis= "none";
    if (isBusy()){
        busy="true";
        orderId= to_string(activeOrderId);
        dis= to_string(getTimeLeft());
    }
    return "VolunteerId: " + to_string(getId())  + "  isBusy: " +busy + 
    "  orderId: " + orderId + 
    "  Time left: " + dis+
    "  Orders left: " + to_string(ordersLeft);
}

DriverVolunteer::DriverVolunteer(int id, const string &name, int _maxDistance, int _distancePerStep) :
    Volunteer(id,name), maxDistance(_maxDistance), distancePerStep(_distancePerStep), distanceLeft(0)
{}

DriverVolunteer * DriverVolunteer::clone() const {
    return new DriverVolunteer (*this);
}

int DriverVolunteer::getDistanceLeft() const {
    return distanceLeft;
}

int DriverVolunteer::getMaxDistance() const {
    return maxDistance;
}

int DriverVolunteer::getDistancePerStep() const {
    return distancePerStep;
}

bool DriverVolunteer::decreaseDistanceLeft() {
    distanceLeft -= distancePerStep;
    if (distanceLeft <= 0){
        distanceLeft = 0;
        return true;
    }
    return false;
}

bool DriverVolunteer::hasOrdersLeft() const {
    return true;
}

bool DriverVolunteer::canTakeOrder(const Order &order) const {
    if (isBusy() | (order.getStatus()!=OrderStatus(1)) | (order.getDistance()> maxDistance ))
        return false;
    return true;
}

void DriverVolunteer::acceptOrder(const Order &order) {
    distanceLeft = order.getDistance();
    activeOrderId = order.getId(); 
}

void DriverVolunteer::step() {
    if (decreaseDistanceLeft()) {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}

string DriverVolunteer::toString() const {
    string busy= "false";
    string orderId= "none";
    string dis= "none";
    if (isBusy()){
        busy="true";
        orderId= to_string(activeOrderId);
        dis= to_string(distanceLeft);
    }
    return "VolunteerId:" + to_string(getId())  + "isBusy: " +busy + 
    "orderId: " + orderId + 
    "Distance left: " + dis+
    "Orders left: no limit";
}

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int _maxOrders):
    DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(_maxOrders), ordersLeft(_maxOrders)
{}

LimitedDriverVolunteer *LimitedDriverVolunteer:: clone() const {
    return new LimitedDriverVolunteer(*this);
}

int LimitedDriverVolunteer::getMaxOrders() const {
    return maxOrders;
}

int LimitedDriverVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

bool LimitedDriverVolunteer::hasOrdersLeft() const {
    if (ordersLeft >0)
        return true;
    return false;
}

 bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {
    if (! DriverVolunteer::canTakeOrder(order) | ! hasOrdersLeft() )
        return false;
    return true;
 }

void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    DriverVolunteer::acceptOrder(order);
    ordersLeft -- ;
}

string LimitedDriverVolunteer::toString() const {
    string busy= "false";
    string orderId= "none";
    string dis= "none";
    if (isBusy()){
        busy="true";
        orderId= to_string(activeOrderId);
        dis= to_string(getDistanceLeft());
    }
    return "VolunteerId:" + to_string(getId())  + "isBusy: " +busy + 
    "orderId: " + orderId + 
    "Distance left: " + dis+
    "Orders left: " + to_string(ordersLeft);
}

string CollectorVolunteer::type() const {
    return "CollectorVolunteer";
}
string LimitedCollectorVolunteer::type() const{
    return "LimitedCollectorVolunteer";
}
string DriverVolunteer::type() const {
    return "DriverVolunteer";
}
string LimitedDriverVolunteer::type() const {
    return "LimitedDriverVolunteer";
}

void Volunteer::setActiveOrderId (int _activeOrderId) {
    activeOrderId = _activeOrderId;
}

