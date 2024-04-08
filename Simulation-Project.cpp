#include <iostream>
#include "Customer.h"
#include <queue>
#include <vector>

using namespace std;

int main()
{
    int simulationTime, numServers, serverTransactionTime, timeBetweenArrivals;

    std::cout << "Enter the number of time units for the simulation: ";
    std::cin >> simulationTime;

    std::cout << "Enter the number of servers: ";
    std::cin >> numServers;

    std::cout << "Enter the server transaction time units: ";
    std::cin >> serverTransactionTime;

    std::cout << "Enter time units between customer arrivals: ";
    std::cin >> timeBetweenArrivals;

    ServerList serverList(numServers, serverTransactionTime);
    CustomerQueue customerQueue;
    customerQueue.setTimeBetweenArrivals(timeBetweenArrivals);

    int currentTime = 0;
    int customerId = 1;
    int customersLeftInQueue = 0;
    int customersArrived = 0;
    int customersServed = 0;

    while (currentTime < simulationTime) {
        pair<int, int> departedCustomer = serverList.updateBusyServerTransactionTimes();
        if (departedCustomer.first != -1) {
            cout << "From server: " << departedCustomer.first << " customer " << departedCustomer.second << " departed at time unit " << currentTime + serverList.getServerTransactionTime() << endl;
        }
        serverList.updateBusyServerTransactionTimes();

        // Increment wait time for customers in the queue
        customerQueue.updateCustomerWaitTimes();

        // Check if a customer has arrived
        if (customerQueue.hasCustomerArrived(currentTime)) {
            Customer newCustomer(customerId++);
            int newCustomerId = customerQueue.addCustomerToQueue(newCustomer);
            customersArrived++;
            cout << "Customer number: " << newCustomerId << " arrived at time unit: " << currentTime << endl;
        }

        // Assign customers from the queue to free servers
        while (customerQueue.getNumberOfCustomersInQueue() > 0 && serverList.findFreeServer() != -1) {
            Customer c = customerQueue.removeCustomerFromQueue();
            serverList.assignCustomerToFreeServer(c);
            int serverId = serverList.findFreeServer()+1;
            std::cout << "From server: " << serverId << " customer " << c.getCustomerId() << " departed at time unit " << currentTime + serverList.getServerTransactionTime() << endl;
        }

        currentTime++;
    }

    customersLeftInQueue = customerQueue.getNumberOfCustomersInQueue();
    customersServed = serverList.getTotalCustomersServed();
    int totalWaitTime = serverList.getTotalWaitTime();
    double averageWaitTime = customerQueue.getWaitTimeOfCustomersInQueue();

    std::cout << "---------------------------------------------------------------" << std::endl;
    std::cout << "Number of customers left in queue: " << customersLeftInQueue << std::endl;
    std::cout << "Number of customers that arrived: " << customersArrived << std::endl;
    std::cout << "Number of customers who completed a transaction: " << customersServed << std::endl;
    std::cout << "Average wait time for customers in the queue: " << averageWaitTime << std::endl;

    return 0;
}