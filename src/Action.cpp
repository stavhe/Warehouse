//#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include "../include/Action.h"
#include <vector>
#include <string>
using std::string;
using std::vector;
using namespace std;

BaseAction::BaseAction(): errorMsg(""), status(ActionStatus(0)){}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus(0);
}

 void BaseAction::error(string errorMsg){
    status = ActionStatus(1);
    this->errorMsg = errorMsg;
 }

string BaseAction::getErrorMsg() const {
    return errorMsg;
}

SimulateStep ::SimulateStep(int _numOfSteps) : numOfSteps(_numOfSteps) {
}

void SimulateStep::act(WareHouse &wareHouse) {
    for (int i=0; i<numOfSteps; i++){
        vector<Order*> pendingO = wareHouse.getPendingOrders();
        int sizeP = pendingO.size();
        int erased = 0;
        for (int j=0; j<sizeP; j++){
            vector<Volunteer*> vol =  wareHouse.getVolunteers();
            bool found = false;
            if (pendingO[j-erased]->getStatus() == OrderStatus(0)){
               for (int k=0; (k< (int)vol.size()) & !found; k++) {
                    if (((vol[k]->type() == "CollectorVolunteer") | (vol[k]->type() =="LimitedCollectorVolunteer")) & vol[k]->canTakeOrder(*pendingO[j-erased])) {
                        vol[k]->acceptOrder(*pendingO[j-erased]);
                        pendingO[j-erased]->setCollectorId(vol[k]->getId());
                        pendingO[j-erased]->setStatus(OrderStatus(1));
                        wareHouse.addToInProc(pendingO[j-erased]);
                        wareHouse.erasePen( (j-erased));
                        pendingO = wareHouse.getPendingOrders();
                        found = true;
                        erased ++;
                    }
               }
            }
            else {
                if (pendingO[j-erased]->getStatus() == OrderStatus(1)){
               found = false;
               for (int k=0; (k<(int)vol.size() )& !found; k++) {
                    if (((vol[k]->type() == "DriverVolunteer") | (vol[k]->type() =="LimitedDriverVolunteer")) & vol[k]->canTakeOrder(*pendingO[j-erased])) {
                        vol[k]->acceptOrder(*pendingO[j-erased]);
                        pendingO[j-erased]->setDriverId(vol[k]->getId());
                        pendingO[j-erased]->setStatus(OrderStatus(2));
                        wareHouse.addToInProc(pendingO[j-erased]);
                        wareHouse.erasePen((j-erased));
                        pendingO = wareHouse.getPendingOrders();
                        found = true;
                        erased++;
                    }
               }
            }
            }
        }
        vector<Volunteer*> vol =  wareHouse.getVolunteers();
        for (int j=0; j<(int)vol.size(); j++){
            if (vol[j]->isBusy()) {
                vol[j]->step();
            }
        }
        erased =0;
        vector<Order*> inProc = wareHouse.getInProcessOrders();
        sizeP = inProc.size();
        for (int j=0; j<sizeP; j++){
            if (inProc[j-erased]->getStatus() == OrderStatus(1)){
                if (wareHouse.getVolunteer(inProc[j-erased]->getCollectorId()).getActiveOrderId() == NO_ORDER){
                    wareHouse.addToPend(inProc[j-erased]);
                    wareHouse.eraseProc(j-erased);
                    inProc = wareHouse.getInProcessOrders();
                    erased++;
                }
            }
            else {
                if (inProc[j-erased]->getStatus() == OrderStatus(2)){
                if (wareHouse.getVolunteer(inProc[j-erased]->getDriverId()).getActiveOrderId() == NO_ORDER){
                    inProc[j-erased]->setStatus(OrderStatus(3));
                    wareHouse.addToComp(inProc[j-erased]);
                    wareHouse.eraseProc(j-erased);
                    inProc = wareHouse.getInProcessOrders();
                    erased++;
                }
            }
            }
        }

        erased = 0;
        int sizeV = vol.size();
        for (int j=0; j<sizeV; j++){
            if (!vol[j-erased]->hasOrdersLeft() & !vol[j-erased]->isBusy()) {
                wareHouse.eraseVol(j-erased);
                vol =  wareHouse.getVolunteers();
                erased ++;
            }
        }

    }
    complete();
}


SimulateStep * SimulateStep::clone() const {
    return new SimulateStep (*this);
}

 std::string SimulateStep::toString() const {
    return ("SimulateStep " + to_string(numOfSteps));
 }

AddOrder:: AddOrder(int id): customerId(id) {
}

void AddOrder:: act(WareHouse &wareHouse) {
    if (customerId >= wareHouse.getCustomerCounter()){
        error("Cannot place this order");
        cout<< getErrorMsg() << endl;
    }
    else {
        if (! wareHouse.getCustomer(customerId).canMakeOrder()){
        error("Cannot place this order");
        cout<< getErrorMsg() << endl;
         }
        else {
            wareHouse.addOrder(new Order(wareHouse.getOrderCounter(), customerId, wareHouse.getCustomer(customerId).getCustomerDistance()));
            complete();
            wareHouse.getCustomer(customerId).addOrder(wareHouse.getOrderCounter());
            wareHouse.setOrderCounter();
        }
    }
}

AddOrder *AddOrder:: clone() const {
    return new AddOrder (*this);
}

string AddOrder:: toString() const {
    return ("Order " + to_string(customerId));
}


AddCustomer:: AddCustomer(const string & _customerName, const string &_customerType, int _distance, int _maxOrders) :
 customerName(_customerName), customerType(stringToCustomerType(_customerType)), distance(_distance), maxOrders(_maxOrders) {
 }

void AddCustomer::act(WareHouse &wareHouse) {
    if (customerType == CustomerType(0)){
        wareHouse.addCust(new SoldierCustomer(wareHouse.getCustomerCounter(), customerName, distance , maxOrders));
    }
    else {
        wareHouse.addCust(new CivilianCustomer(wareHouse.getCustomerCounter(), customerName, distance , maxOrders));
    }
    complete();
    wareHouse.setCustomerCounter();
}

CustomerType AddCustomer:: stringToCustomerType (const string &_customerType) const {
    if (_customerType == "soldier")
        return CustomerType(0);
    else
        return CustomerType(1);
}


AddCustomer *AddCustomer:: clone() const {
    return new AddCustomer (*this);
}

string AddCustomer:: toString() const {
    string st= " civillian "; 
    if(customerType== CustomerType(0))
       st= " soldier ";
    return ("Customer " + customerName + st + to_string(distance) + " " + to_string(maxOrders));
}

PrintOrderStatus:: PrintOrderStatus(int id): orderId(id) {
}

void PrintOrderStatus:: act(WareHouse &wareHouse)  {
    if (orderId >= wareHouse.getOrderCounter()){
        error("Order does not exist");
        cout<< getErrorMsg() << endl;
    }
    else {
        cout << "Order id : " + to_string(orderId) << endl;
        cout << "Order status: " + wareHouse.getOrder(orderId).stringStatus() << endl;
        cout << "Customer id : " +  to_string(wareHouse.getOrder(orderId).getCustomerId()) << endl;
        if (wareHouse.getOrder(orderId).getCollectorId() != -1)
            cout << "Collector: " + to_string(wareHouse.getOrder(orderId).getCollectorId()) << endl;
        else
            cout << "Collector: None" << endl;
        if (wareHouse.getOrder(orderId).getDriverId() != -1)
            cout << "Driver: " + to_string(wareHouse.getOrder(orderId).getDriverId()) << endl;
        else
            cout << "Driver: None" << endl;
        complete();
    }
}

PrintOrderStatus * PrintOrderStatus::clone() const {
    return new PrintOrderStatus (*this);
}

string PrintOrderStatus:: toString() const {
return ("OrderStatus " +to_string(orderId));
}    

PrintCustomerStatus:: PrintCustomerStatus(int _customerId) : customerId(_customerId){
}

void PrintCustomerStatus:: act(WareHouse &wareHouse) {
    if (customerId >= wareHouse.getCustomerCounter()){
        error("Customer does not exist");
        cout<< getErrorMsg() << endl;
    }
    else {
        cout << "Customer id: " + to_string(customerId) << endl;
        vector<int> ordersId = wareHouse.getCustomer(customerId).getOrdersIds();
        for (int i=0; i<(int)ordersId.size(); i++){
             cout << "Order id: " + to_string(ordersId[i]) << endl;             
             cout << "Order status: " + wareHouse.getOrder(ordersId[i]).stringStatus() << endl;
        }
        cout << "Num Orders left: " + to_string((wareHouse.getCustomer(customerId).getMaxOrders()-ordersId.size())) << endl;
        complete();
    }
}

 PrintCustomerStatus * PrintCustomerStatus::clone() const {
    return new PrintCustomerStatus (*this);
 }

 string PrintCustomerStatus:: toString() const {
return ("CustomerStatus " +to_string(customerId));
}  

PrintVolunteerStatus:: PrintVolunteerStatus(int id): volunteerId(id) {
}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    if (wareHouse.getVolunteer(volunteerId).getId() == -1){
        error("Volunteer does not exist");
        cout<< getErrorMsg() << endl;
    }
    else {
       cout<< wareHouse.getVolunteer(volunteerId).toString() << endl; 
    }

}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const {
    return new PrintVolunteerStatus(*this);
}


 string PrintVolunteerStatus:: toString() const {
return ("VolunteerStatus " +to_string(volunteerId));
} 

 PrintActionsLog::PrintActionsLog(){}

 void PrintActionsLog::act(WareHouse &wareHouse){
    for (int i=0;i<(int)wareHouse.getActions().size(); i++){
        string st = wareHouse.getActions()[i]->toString();
        string st2 =  wareHouse.getActions()[i]->status_message();
        cout << st + "  " + st2 << endl;
    }
    complete();
 }


string PrintActionsLog:: toString() const {
return ("log ");
}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}

Close:: Close(){}     
   
void Close:: act(WareHouse &wareHouse){
   for (int i=0;i<(int)wareHouse.getPendingOrders().size(); i++){
    string st= wareHouse.getPendingOrders()[i]->toString();
    cout << st + " "<< endl;   
    }
    for (int i=0;i<(int)wareHouse.getInProcessOrders().size(); i++){
    string st= wareHouse.getInProcessOrders()[i]->toString();
    cout << st +" " << endl;   
    }
    for (int i=0;i<(int)wareHouse.getCompletedOrders().size(); i++){
    string st= wareHouse.getCompletedOrders()[i]->toString();
    cout << st + " "<< endl;   
    }
    complete();
    wareHouse.close();
}

Close * Close::clone() const {
    return new Close(*this);
}

string Close::toString() const {
    return "close";
}

BackupWareHouse:: BackupWareHouse(){}

extern WareHouse* backup;

void BackupWareHouse::act(WareHouse &wareHouse) {
if (backup!= nullptr){
    delete backup;
    backup = nullptr;
}
backup =new WareHouse(wareHouse);
complete();
}

BackupWareHouse *BackupWareHouse::clone() const {
  return new BackupWareHouse(*this);
}

string BackupWareHouse:: toString() const {
return "backup";
}

RestoreWareHouse:: RestoreWareHouse(){}

void RestoreWareHouse:: act(WareHouse &wareHouse) {
    if (backup == nullptr){
        error ("No backup available");
        cout<< getErrorMsg() << endl;
    }
    else{
        wareHouse=*backup;
        complete();
    }

}

RestoreWareHouse * RestoreWareHouse::clone() const {
    return new RestoreWareHouse(*this);
}

string RestoreWareHouse:: toString() const {
    return "restore";
}

string BaseAction:: status_message () const{
    if (getStatus()== ActionStatus(0))
        return "COMPLETED ";
    else
        return "ERROR";
}


    

    

 
















