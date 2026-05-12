// Name: Abdulrahman Magdy Saad   ,  ID: 320250281  , Person: B
#ifndef MODELS_H
#define MODELS_H
#include <iostream>
using namespace std;

class Customer
{
private:
    int id;
    int arrivalTime;
    int requiredServiceTime;
    int waitTime;

public:
    Customer() : id(0), arrivalTime(0), requiredServiceTime(0), waitTime(0) {} // EDIT (4)
    Customer(int id, int arrivalTime, int requiredServiceTime)                 // EDIT (5)
        : id(id), arrivalTime(arrivalTime), requiredServiceTime(requiredServiceTime), waitTime(0)
    {
    }
    // DISPLAY FOR TESTING ( EDIT 1 )
    void display() const
    {
        cout << "Customer ID: " << id
             << " | Arrival: " << arrivalTime
             << " | Required Service: " << requiredServiceTime
             << " | Wait Time: " << waitTime << endl;
    }
    void setWaitTime(int waitTime)
    {
        this->waitTime = waitTime;
    }
    int getRequiredServiceTime() const
    {
        return requiredServiceTime;
    }
    // ADDED: Getters so we can read these values!
    int getId() const { return id; }
    int getArrivalTime() const { return arrivalTime; }
    int getWaitTime() const { return waitTime; }
};
class Server
{
private:
    int id;
    bool busy;
    int remainingServiceTime;

public:
    // ADDED: Default constructor just in case we need an array of Servers
    Server()
    {
        id = 0;
        busy = false;
        remainingServiceTime = 0;
    }
    Server(int id) : id(id), busy(false), remainingServiceTime(0) {} // EDIT (6)
    // Assigning customer using Pass by Reference (const) for better performance (EDIT 2 )
    void assignCustomer(const Customer &customer)
    {
        busy = true;
        remainingServiceTime = customer.getRequiredServiceTime();
    }
    void tick()
    {
        if (busy)
        {
            remainingServiceTime--;
        }
        if (remainingServiceTime <= 0)
        {
            busy = false;
            remainingServiceTime = 0;
        }
    }
    // ADDED: Getters so Person C can check if the server is free!
    bool isBusy() const { return busy; }
    int getId() const { return id; }
    // DISPLAY FOR TESTING ( EDIT 3)
    void display() const
    {
        cout << "Server ID: " << id
             << " | Busy: " << (busy ? "Yes" : "No")
             << " | Remaining Time: " << remainingServiceTime << endl;
    }
};

#endif
