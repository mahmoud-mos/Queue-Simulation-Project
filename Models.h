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
    bool isVip; // To track vip priority status

public:
    Customer() : id(0), arrivalTime(0), requiredServiceTime(0), waitTime(0), isVip(false) {}
    Customer(int id, int arrivalTime, int requiredServiceTime, bool vip = false)                              // VIP constructor
        : id(id), arrivalTime(arrivalTime), requiredServiceTime(requiredServiceTime), waitTime(0), isVip(vip) // initialization
    {
    }
    // DISPLAY FOR TESTING
    void display() const
    {
        //VIP display
        cout << (isVip ? "[VIP] " : "[Regular] ") << "Customer ID: " << id
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
    // Getters to read values from
    int getId() const { return id; }
    int getArrivalTime() const { return arrivalTime; }
    int getWaitTime() const { return waitTime; }
    bool getIsVip() const { return isVip; } //VIP Getter
};
class Server
{
private:
    int id;
    bool busy;
    int remainingServiceTime;

public:
    // Default constructor if we need an array of Servers
    Server()
    {
        id = 0;
        busy = false;
        remainingServiceTime = 0;
    }
    Server(int id) : id(id), busy(false), remainingServiceTime(0) {}

    // Assigning customer using Pass by Reference (const)
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
    // so main can check if the server is free
    bool isBusy() const { return busy; }
    int getId() const { return id; }

    // DISPLAY FOR TESTING
    void display() const
    {
        cout << "Server ID: " << id
             << " | Busy: " << (busy ? "Yes" : "No")
             << " | Remaining Time: " << remainingServiceTime << endl;
    }
};

#endif