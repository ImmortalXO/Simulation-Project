#pragma once
#include <string>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class Customer
{
private:
	int id;
	int wait;
public:
	Customer(int id) : id(id), wait(0) {};
	int getId() const { return id; };
	//void setId(int target) { target = id; };
	int getWait() const { return wait; };
	void incrementWait() { wait++; };
};

class CustomerQueue
{
private:
	queue<Customer> queue;
	int timeBetweenArrivals;
public:
	CustomerQueue() {
		timeBetweenArrivals = 1;
	}

	void addCustomerToQueue(const Customer &customer) {
		queue.push(customer);
	}

	Customer removeCustomerFromQueue() {
		Customer customer = queue.front();
		queue.pop();
		return customer;
	}

	bool hasCustomerArrived(int currentTime) const {
		return currentTime % timeBetweenArrivals == 0;
	}

	void updateCustomerWaitTimes() {
		int size = queue.size();
		for (int i = 0; i < size; ++i) {
			Customer& customer = queue.front();
			customer.incrementWait();
			queue.push(customer);
			queue.pop();
		}
	}

	int getCustomersInQueue() const {
		return queue.size();
	}

	int setTimeBetweenArrivals(int time) {
		timeBetweenArrivals = time;
	}

	int getWaitTimeOfRemainingCustomers() {
		int waitTime = 0;
		std::queue<Customer> temp = queue;
		while (!temp.empty()) {
			waitTime += temp.front().getWait();
			temp.pop();
		}
		return waitTime;
	}
};

class Server
{
private:
	int serverId;
	int status;
	int transactionTime;
	Customer customer;
public:
	Server(int id) : serverId(serverId), status(0), transactionTime(0) {};

	void setServerId(int target) {
		target = serverId;
	}

	int getServerId() const {
		return serverId;
	}

	void setStatus(int currentStatus) {
		status = currentStatus;
	}

	int getStatus() const {
		return status;
	}

	void setTransactionTime(int time) {
		transactionTime = time;
	}

	int getTransactionTime() const {
		return transactionTime;
	}

	void updateTransactionTime() {
		transactionTime--;
	}

	int getCustomerWaitTime() const {
		return customer.getWait();
	}

	int getCustomerId() const {
		return customer.getId();
	}

	void addCustomerToServer(const Customer &newCustomer) {
		customer = newCustomer;
		status = 1;
	}
};

class ServerList
{
private:
	vector<Server> serverList;
	int transactionTime;
	int customersServed;
	int totalWaitTime;
public:
	ServerList(int serverAmount, int transactionTime): transactionTime(transactionTime), customersServed(0), totalWaitTime(0) {
		for (int i = 0; i < serverAmount; ++i) {
			serverList.emplace_back(i + 1); // more optimized way instead of push_back
		}
	};

	void addServerToList(const Server& serv) {
		serverList.push_back(serv);
	}

	int findFreeServer() const {
		for (int i = 0; i < serverList.size(); ++i) {
			if (serverList[i].getStatus() == 0) {
				return i;
			}
		}
		return -1;
	}

	void setTransactionTime(int time) {
		transactionTime = time;
	}

	void assignCustomerToFreeServer(const Customer& newCustomer) {
		int freeServer = findFreeServer();
		if (freeServer != -1) {
			serverList[freeServer].addCustomerToServer(newCustomer);
		}
	}

	void updateBusyServerTransactionTimes() {
		for (auto& s : serverList) {

		}
	}

	int getCustomersServed() const {
		return customersServed;
	}

	int getTotalWaitTimes() const {
		return totalWaitTime;
	}
};
