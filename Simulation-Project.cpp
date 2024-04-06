#include <iostream>
#include <queue>
#include <vector>
using namespace std;
// Customer ADT
class Customer {
private:
    int customerId;
    int waitTime;
public:
    Customer(int id) : customerId(id), waitTime(0) {};
    void setCustomerId(int id) {
        customerId = id;
    }

    int getCustomerId() const {
        return customerId;
    }

    void updateWaitTime() {
        waitTime++;
    }

    int getWaitTime() const {
        return waitTime;
    }
};

class CustomerQueue
{
private:
    queue<Customer> customerQueue;
    int timeBetweenArrivals;
public:
    CustomerQueue() : timeBetweenArrivals(0) {};

    int addCustomerToQueue(const Customer& newCustomer) {
        customerQueue.push(newCustomer);
        return newCustomer.getCustomerId();
    }

    Customer removeCustomerFromQueue(const Customer& newCustomer) {
        Customer cust = customerQueue.front();
        customerQueue.pop();
        return cust;
    }

    bool hasCustomerArrived(int currentTime) {
        return currentTime % timeBetweenArrivals == 0;
    }

    void updateCustomerWaitTimes() {
        int size = customerQueue.size();
        for (int i = 0; i < size; i++) {
            Customer cust = customerQueue.front();
            customerQueue.pop();
            cust.updateWaitTime();
            customerQueue.push(cust);
        }
    }

    int getNumberOfCustomersInQueue() const {
        return customerQueue.size();
    }

    void setTimeBetweenArrivals(int time) {
        timeBetweenArrivals = time;
    }

    int getWaitTimeOfCustomersInQueue() const {
        int totalWaitTime = 0;
        int size = customerQueue.size();
        for (int i = 0; i < size; ++i) {
            Customer cust = customerQueue.front();
            totalWaitTime += cust.getWaitTime();
        }
        return customerQueue.empty() ? 0.0 : static_cast<double>(totalWaitTime) / customerQueue.size();
    }
};

class Server
{
private:
    int serverId;
    int status;
    int transactionTime;
    Customer cust;
public:
    Server(int id) : serverId(id), status(0), transactionTime(0), cust(0) {}

    void setServerId(int id) {
        serverId = id;
    }

    int getServerId() const {
        return serverId;
    }

    void setServerStatus(int stat) {
        status = stat;
    }

    int getServerStatus() const {
        return status;
    }

    void setTransactionTime(int time) {
        transactionTime = time;
    }

    int getTransactionTime() const {
        return transactionTime;
    }

    void updateServerTransactionTime() {
        transactionTime--;
    }

    int getCustomerWaitTime() const {
        return cust.getWaitTime();
    }

    int getCustomerId() const {
        return cust.getCustomerId();
    }

    void addCustomerToServer(const Customer& newCustomer) {
        cust = newCustomer;
        status = 1;
    }

    Customer removeCustomerFromServer() {
        Customer newCustomer = cust;
        cust = Customer(0);
        status = 0;
        return newCustomer;
    }
};

class ServerList
{
private:
    vector<Server> serverList;
    int serverCount;
    int transactionTime;
    int totalCustomersServed;
    int totalWaitTime;
public:
    ServerList(int serverCount, int transactionTime) : serverCount(0), transactionTime(transactionTime), totalCustomersServed(0), totalWaitTime(0) {
        for (int i = 0; i < serverCount; i++) {
            serverList.push_back(Server(i + 1));
        }
    }

    void addServerToList(const Server& server) {
        serverList.push_back(server);
    }

    int findFreeServer() {
        for (int i = 0; i < serverList.size(); ++i) {
            if (serverList[i].getServerStatus() == 0) {
                return i;
            }
        }
        return -1;
    }

    void assignCustomerToFreeServer(const Customer& newCustomer) {
        int freeServer = findFreeServer();
        if (freeServer != -1) {
            serverList[freeServer].addCustomerToServer(newCustomer);
        }
    }

    pair<int, int> updateBusyServerTransactionTimes() {
        for (int i = 0; i < serverList.size(); ++i) {
            if (serverList[i].getServerStatus() == 1) {
                serverList[i].updateServerTransactionTime();
                if (serverList[i].getTransactionTime() == 0) {
                    Customer newCustomer = serverList[i].removeCustomerFromServer();
                    totalCustomersServed++;
                    totalWaitTime += newCustomer.getWaitTime();
                    serverList[i].setServerStatus(0);
                    return make_pair(serverList[i].getServerId(), serverList[i].getCustomerId());
                }
            }
        }
        return make_pair(-1, -1);
    }


};

int main() {

    return 0;
}

/*
#include <iostream>
#include "Customer.h"
#include <queue>
#include <vector>

using namespace std;

int main()
{
    int serverAmount, serverTransactionTime, timeBetweenArrival, simulationTime;

    cout << "Enter the number of time units for the simulation: ";
    cin >> simulationTime;

    cout << "Enter the number of servers: ";
    cin >> serverAmount;

    cout << "Enter the server transaction time units: ";
    cin >> serverTransactionTime;

    cout << "Enter time units between customer arrivals: ";
    cin >> timeBetweenArrival;

    ServerList servers(serverAmount, serverTransactionTime);
    CustomerQueue customerQueue;
    customerQueue.setTimeBetweenArrivals(timeBetweenArrival);

    int currentTime = 0;
    int currentId = 1;
    int leftInQueue = 0;
    int customersArrived = 0;
    int customersServed = 0;

    while (currentTime < simulationTime) {
        servers.updateBusyServerTransactionTimes();
        customerQueue.updateCustomerWaitTimes();

        if (customerQueue.hasCustomerArrived(currentTime)) {
            Customer newCustomer(currentId++);
            int newCustomerId = customerQueue.addCustomerToQueue(newCustomer);
            customersArrived++;
            cout << "Customer number: " << newCustomerId << " arrived at time unit: " << currentTime << endl;
        }

        if (servers.findFreeServer() != -1 && customerQueue.getCustomersInQueue() > 0) {
            Customer removedCustomer = customerQueue.removeCustomerFromQueue();
            servers.assignCustomerToFreeServer(removedCustomer);
            cout << "From server: " << removedCustomer.getId() << " customer " << removedCustomer.getId() << " departed at time " << currentTime << endl;
        }
        currentTime++;
    };

    leftInQueue = customerQueue.getCustomersInQueue();
    customersServed = servers.getCustomersServed();
    int totalWaitTime = servers.getTotalWaitTimes();
    double avgWaitTime = customersServed == 0 ? 0.0 : static_cast<double>(totalWaitTime) / customersServed;

    cout << "---------------------------------------------------------------" << endl;
    cout << "Number of customers left in queue: " << leftInQueue << endl;
    cout << "Number of customers who have arrived: " << customersArrived << endl;
    cout << "Number of customers who completed a transaction: " << customersServed << endl;
    cout << "Average wait time for customers in the queue: " << avgWaitTime << endl;

    return 0;
}
*/