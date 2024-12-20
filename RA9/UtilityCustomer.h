#ifndef UTILITYCUSTOMER_H
#define UTILITYCUSTOMER_H

#include <iostream>
using namespace std;

class UtilityCustomer
{
protected:
    int accNum; // Account number

public:
    // Default constructor
    UtilityCustomer() : accNum(-1) {}

    // Parameterized constructor
    UtilityCustomer(int acc) : accNum(acc) {}

    // Setter for account number
    void setAccount(int accountNumber) { accNum = accountNumber; }
    // Getter for account number
    int getAccNum() const { return accNum; }

    // Display account number
    virtual void Display() const { cout << "Account Number: " << getAccNum() << endl; }

    // Overloaded equality operator (parameter is a pointer)
    bool operator==(const UtilityCustomer *UtilityObj) const
    {
        return this->accNum == UtilityObj->getAccNum();
    }

    // Abstract method for calculating bill
    virtual double calculateBill() const = 0;

    // Virtual destructor
    virtual ~UtilityCustomer() {}
};

#endif
