#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Order.h"
#include "Customer.h"
#include "Action.h"
#include "Volunteer.h"
using namespace std;
class BaseAction;
class Volunteer;
class Action;
class Order;
class Customer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        WareHouse(const string &configFilePath);
        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        vector<Order*> getPendingOrders();
        vector<Order*> getInProcessOrders ();
        vector<Order*> getCompletedOrders();
        vector<Volunteer*> getVolunteers ();
        vector<Customer*> getCustomers ();
        int getOrderCounter () const;
        int getCustomerCounter() const;
        void setOrderCounter () ;
        void setCustomerCounter() ;
        void addCust (Customer* newcust) ;
        void addToPend (Order* ord);
        void addToInProc (Order* ord);
        void addToComp (Order* ord);
        void erasePen ( int index);
        void eraseProc ( int index);
        void eraseVol ( int index);



        WareHouse & operator=(const WareHouse & other);
        WareHouse(const WareHouse & other );
        ~WareHouse() ;
        WareHouse(const WareHouse && other );
        WareHouse & operator=(const WareHouse && other);

    private:
        bool isOpen;
        bool deleted; 
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter;
        Volunteer * falseVolunteer;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;

};