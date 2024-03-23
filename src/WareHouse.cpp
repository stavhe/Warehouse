//#pragma once
#include "../include/WareHouse.h"
using std::string;
using std::vector;


WareHouse::WareHouse(const string &configFilePath):
isOpen(false),deleted(false), customerCounter(0), volunteerCounter(0), orderCounter(0),
falseVolunteer(new CollectorVolunteer(-1,"",-1)), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(),completedOrders(), customers() {

std::ifstream file(configFilePath);
std::string line;
while(std::getline(file, line)) 
    {
        std::stringstream   linestream(line);
        std::string         data1; // customer/volenteer
        std::string         data2; //name of customer/volnteer
        std::string         data3; //type/role
        int                 val1;  //volunteer- cooldown/max distance, costumer-distance
        int                 val2;  //volunteer- distemce per step(only drivers), customer-max order
        int                 val3;  //volunteer- max order (optional)

        // If you have truly tab delimited data use getline() with third parameter.
        // If your data is just white space separated data
        // then the operator >> will do (it reads a space separated word into a string).
        std::getline(linestream, data1, ' ');  // read up-to the first tab (discard tab).
        if(data1 == "customer" || data1 == "volunteer"){
            // Read the integers using the operator >>
            linestream >>data2 >> data3 >> val1 >> val2 >> val3;
            if(data1 == "customer" && data3 == "civilian"){
                customers.push_back(new CivilianCustomer(customerCounter, data2, val1, val2));
                customerCounter++;
            }
            if(data1 == "customer" && data3 == "soldier"){
                customers.push_back(new SoldierCustomer(customerCounter, data2, val1, val2));
                customerCounter++;
            }
            if(data1 == "volunteer" && data3 == "collector"){
                volunteers.push_back(new CollectorVolunteer(volunteerCounter, data2, val1));
                volunteerCounter++;
            }
            if(data1 == "volunteer" && data3 == "driver"){
                volunteers.push_back(new DriverVolunteer(volunteerCounter, data2, val1, val2));
                volunteerCounter++;
            }
            if(data1 == "volunteer" && data3 == "limited_collector"){
                volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter, data2, val1, val2));
                volunteerCounter++;
            }
            if(data1 == "volunteer" && data3 == "limited_driver"){
                volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter, data2, val1, val2,val3));
                volunteerCounter++;
            }
        }
}
}

WareHouse:: WareHouse(const WareHouse & other ):
isOpen(other.isOpen), deleted(other.deleted), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter),
falseVolunteer(other.falseVolunteer->clone()), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(){
    for (int i=0; i<(int) other.actionsLog.size(); i++){
        actionsLog.push_back(other.actionsLog[i]->clone());
    }
    for (int i=0; i<(int) other.volunteers.size(); i++){
        volunteers.push_back(other.volunteers[i]->clone());
    }
    for (int i=0; i<(int) other.customers.size(); i++){
        customers.push_back(other.customers[i]->clone());
    }
    for (int i=0; i<(int) other.pendingOrders.size(); i++){
        pendingOrders.push_back(new Order(*other.pendingOrders[i]));
    }
     for (int i=0; i<(int) other.inProcessOrders.size(); i++){
        inProcessOrders.push_back(new Order(*other.inProcessOrders[i]));
    }
     for (int i=0; i<(int) other.completedOrders.size(); i++){
        completedOrders.push_back(new Order(*other.completedOrders[i]));
    }
}

WareHouse & WareHouse:: operator=(const WareHouse & other){
    if(&other!=this){
       isOpen=other.isOpen;
       deleted= other.deleted;
       customerCounter=other.customerCounter;
       volunteerCounter=other.volunteerCounter;
       orderCounter=other.orderCounter;
       delete falseVolunteer;
       falseVolunteer = nullptr;
       falseVolunteer=other.falseVolunteer->clone();
    for (int i=0; i<(int) actionsLog.size(); i++){
        delete actionsLog[i];
        actionsLog[i] = nullptr;
    }   
    actionsLog.clear();
    for (int i=0; i<(int) other.actionsLog.size(); i++){
        actionsLog.push_back(other.actionsLog[i]->clone());
    }
    for (int i=0; i<(int) volunteers.size(); i++){
        delete volunteers[i];
        volunteers[i] = nullptr;
    }
    volunteers.clear();
    for (int i=0; i<(int) other.volunteers.size(); i++){
        volunteers.push_back(other.volunteers[i]->clone());
    }
    for (int i=0; i<(int) customers.size(); i++){
        delete customers[i];
        customers[i] = nullptr;
    }
    customers.clear();
    for (int i=0; i<(int) other.customers.size(); i++){
        customers.push_back(other.customers[i]->clone());
    }
    for (int i=0; i<(int) pendingOrders.size(); i++){
        delete pendingOrders[i];
        pendingOrders[i] = nullptr;
    }
    pendingOrders.clear();
    for (int i=0; i<(int) other.pendingOrders.size(); i++){
        pendingOrders.push_back(new Order(*other.pendingOrders[i]));
    }
    for (int i=0; i<(int) inProcessOrders.size(); i++){
        delete inProcessOrders[i];
        inProcessOrders[i] = nullptr;
    }
    inProcessOrders.clear();
    for (int i=0; i<(int) other.inProcessOrders.size(); i++){
        inProcessOrders.push_back(new Order(*other.inProcessOrders[i]));
    }
    for (int i=0; i<(int) completedOrders.size(); i++){
        delete completedOrders[i];
        completedOrders[i] = nullptr;
    }
    completedOrders.clear();
    for (int i=0; i<(int) other.completedOrders.size(); i++){
        completedOrders.push_back(new Order(*other.completedOrders[i]));
    }
  }  
  return *this;   
}

WareHouse:: ~WareHouse() {
if(!deleted){
    for (int i=0; i<(int) actionsLog.size(); i++){
        delete actionsLog[i];
        actionsLog[i] = nullptr;
    }   
    for (int i=0; i<(int) volunteers.size(); i++){
        delete volunteers[i];
        volunteers[i] = nullptr;
    }
    for (int i=0; i<(int) customers.size(); i++){
        delete customers[i];
        customers[i] = nullptr;
    }
    for (int i=0; i<(int) pendingOrders.size(); i++){
        delete pendingOrders[i];
        pendingOrders[i] = nullptr;
    }
    for (int i=0; i<(int) inProcessOrders.size(); i++){
        delete inProcessOrders[i];
        inProcessOrders[i] = nullptr;
    }
    for (int i=0; i<(int) completedOrders.size(); i++){
        delete completedOrders[i];
        completedOrders[i] = nullptr;
    }
    delete falseVolunteer;
    falseVolunteer = nullptr;
}
}

WareHouse:: WareHouse(const WareHouse && other ):
isOpen(other.isOpen) ,deleted(true), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter),
falseVolunteer(other.falseVolunteer), actionsLog(), volunteers(), pendingOrders(), inProcessOrders(), completedOrders(), customers(){
    actionsLog= move (other.actionsLog);
    volunteers= move (other.volunteers);
    customers= move (other.customers);
    pendingOrders= move (other.pendingOrders); 
    inProcessOrders= move (other.inProcessOrders);
    completedOrders=move (other.completedOrders);
    

}
WareHouse & WareHouse:: operator=(const WareHouse && other){
    if(&other!=this){
       isOpen=other.isOpen;
       deleted=true; 
       customerCounter=other.customerCounter;
       volunteerCounter=other.volunteerCounter;
       orderCounter=other.orderCounter;
        delete falseVolunteer;
        falseVolunteer = nullptr;
        falseVolunteer=other.falseVolunteer->clone();
    for (int i=0; i<(int) actionsLog.size(); i++){
        delete actionsLog[i];
        actionsLog[i] = nullptr;
    }   
    actionsLog.clear();
    actionsLog = move (other.actionsLog);
    for (int i=0; i<(int) volunteers.size(); i++){
        delete volunteers[i];
        volunteers[i] = nullptr;
    }
    volunteers.clear();
    volunteers= move (other.volunteers);
    for (int i=0; i<(int) customers.size(); i++){
        delete customers[i];
        customers[i] = nullptr;
    }
    customers.clear();
    customers= move (other.customers);
    for (int i=0; i<(int) pendingOrders.size(); i++){
        delete pendingOrders[i];
        pendingOrders[i] = nullptr;
    }
    pendingOrders.clear();
    pendingOrders= move (other.pendingOrders); 
    for (int i=0; i<(int) inProcessOrders.size(); i++){
        delete inProcessOrders[i];
        inProcessOrders[i] = nullptr;
    }
    inProcessOrders.clear();
    inProcessOrders= move (other.inProcessOrders);
    for (int i=0; i<(int) completedOrders.size(); i++){
        delete completedOrders[i];
        completedOrders[i] = nullptr;
    }
    completedOrders.clear();
    completedOrders=move (other.completedOrders);
  }  
  return *this;   
}
   
void  WareHouse::addOrder(Order* order) {
    if (order->getStatus() == OrderStatus(0) )
        pendingOrders.push_back(order);
    else {
         if ((order->getStatus() == OrderStatus(1)) | (order->getStatus() == OrderStatus(2) ))
           inProcessOrders.push_back(order);
        else
            completedOrders.push_back(order);
    }
}

Customer &  WareHouse::getCustomer(int customerId) const {
    for (int i=0; i< (int)customers.size(); i++){
        if(customers[i]->getId()== customerId) { 
            return *customers[i];
        }  
    }
    Customer* c = new SoldierCustomer(-1, "fail", -1, -1);
    return *c;
}

Volunteer & WareHouse::getVolunteer(int volunteerId) const { 
    for (int i=0; i<(int) volunteers.size(); i++){
        if(volunteers[i]->getId()== volunteerId) 
            return *volunteers[i];
    }
    return *falseVolunteer;
}

Order & WareHouse::getOrder(int orderId) const {
    for (int i=0; i<(int) pendingOrders.size(); i++){
        if(pendingOrders[i]->getId()== orderId) 
            return *pendingOrders[i];
    }
    for (int i=0; i<(int) inProcessOrders.size(); i++){
        if(inProcessOrders[i]->getId()== orderId) 
            return *inProcessOrders[i];
    }
    for (int i=0; i<(int) completedOrders.size(); i++){
        if(completedOrders[i]->getId()== orderId) 
            return *completedOrders[i];
    }
    Order* o = new Order(-1,-1,-1);
    return *o;
}

void  WareHouse::open() {
    isOpen = true;
 }

vector<Order*> WareHouse::getPendingOrders(){
    return pendingOrders;
}
vector<Order*> WareHouse::getInProcessOrders (){
    return inProcessOrders;
}
vector<Order*> WareHouse::getCompletedOrders(){
    return completedOrders;
}

vector<Volunteer*> WareHouse::getVolunteers () {
    return volunteers;
}

int WareHouse::getOrderCounter () const {
    return orderCounter;
}

int WareHouse:: getCustomerCounter() const {
    return customerCounter;
}

void WareHouse::setOrderCounter () {
    orderCounter += 1;
}

void WareHouse:: setCustomerCounter() {
    customerCounter += 1;
}

vector<Customer*> WareHouse::getCustomers () {
    return customers;
}

const vector<BaseAction*> & WareHouse::getActions() const{
    return actionsLog;
}

void  WareHouse::addAction(BaseAction* action){
    actionsLog.push_back(action);
}


void  WareHouse::start(){
    cout<< "Warehouse is open!" << endl;
    std::string userAction="";
    BaseAction* newact= nullptr;
    open();
    while (userAction!= "close"){
        cout<< "enter action" << endl;
        std::getline(cin, userAction);
        std::stringstream linestream (userAction); 
        std::string data1; //action name
        int val1;  //id/numOfStep
        std:: ifstream userAction;
        std::getline(userAction, data1, ' ');  // read up-to the first tab (discard tab).
         linestream >>data1 ;
        if( data1 != "customer"){
            linestream >>val1 ;
            if(data1 == "backup"){
                newact= new BackupWareHouse();
                newact->act(*this);
                addAction((*newact).clone());
                delete newact;
                newact = nullptr;
            }
            if(data1 == "log"){
                newact= new PrintActionsLog();
                newact->act(*this);
                addAction((*newact).clone());
                delete newact;
                newact = nullptr;
            }
            if(data1 == "restore"){
                newact= new RestoreWareHouse();
                newact->act(*this);
                addAction((*newact).clone());
                delete newact;
                newact = nullptr;
            }
            if(data1 == "order"){
                newact= new AddOrder(val1);
                newact->act(*this);
                addAction((*newact).clone());
                delete newact;
                newact = nullptr;
            }
            if(data1 == "customerStatus"){
                newact= new PrintCustomerStatus(val1);
                 newact->act(*this);
                addAction((*newact).clone());
                delete newact;
                newact = nullptr;
            }
            if(data1 == "volunteerStatus"){
                newact= new PrintVolunteerStatus(val1);
                newact->act(*this);
                addAction(newact);
            }
            if(data1 == "orderStatus"){
                newact= new PrintOrderStatus(val1);
                newact->act(*this);
                addAction((*newact).clone());
                delete newact;
                newact = nullptr;
            }
            if(data1 == "step"){
                newact= new SimulateStep(val1);
                newact->act(*this);
                addAction((*newact).clone());
                delete newact;
                newact = nullptr;
            }  

        }
        else{ 
        if (data1 == "customer"){
        std::string data2; //customer name
        std::string data3; //customer type
        int val2;  //customer distance
        int val3;  //customer maxOrder
        linestream >>data2>>data3>>val2>>val3;
        newact = new AddCustomer(data2, data3, val2, val3);
        newact->act(*this);
        addAction((*newact).clone());
        delete newact;
        newact = nullptr;
        }  
        }
    }
    newact= new Close();
    newact->act(*this);
    addAction((*newact).clone());
    delete newact;
    newact = nullptr;
}

void  WareHouse::close(){
    isOpen= false;
}

void WareHouse::addCust (Customer* newcust) {
    customers.push_back(newcust);
}

void WareHouse:: addToPend (Order* ord){
    pendingOrders.push_back(ord);
}

void WareHouse:: addToInProc (Order* ord){
    inProcessOrders.push_back(ord);
}

void WareHouse:: addToComp (Order* ord){
    completedOrders.push_back(ord);
}

void WareHouse:: erasePen ( int index){
    pendingOrders.erase((pendingOrders.begin()+(index)));
}

void WareHouse:: eraseProc ( int index) {
    inProcessOrders.erase((inProcessOrders.begin()+(index)));
}

void WareHouse::eraseVol ( int index) {
    volunteers.erase(volunteers.begin() + (index));
}


