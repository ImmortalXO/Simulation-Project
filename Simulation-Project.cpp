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