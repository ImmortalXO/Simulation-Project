#pragma once
#include <string>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

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

	Customer removeCustomerFromQueue() {
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

	double getWaitTimeOfCustomersInQueue() const {
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
	ServerList(int serverCount, int transactionTime) : serverCount(serverCount), transactionTime(transactionTime), totalCustomersServed(0), totalWaitTime(0) {
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

	int getTotalCustomersServed() const {
		return totalCustomersServed;
	}

	int getTotalWaitTime() const {
		return totalWaitTime;
	}

	int getServerTransactionTime() const {
		return transactionTime;
	}

	void assignCustomerToFreeServer(const Customer& newCustomer) {
		int freeServer = findFreeServer();
		if (freeServer != -1) {
			serverList[freeServer].addCustomerToServer(newCustomer);
		}
	}

	// pair<> type to be able to return two values.
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
