#ifndef GASCUSTOMER_H
#define GASCUSTOMER_H

#include "UtilityCustomer.h"

class GasCustomer : public UtilityCustomer {
private:
    double usage; // Usage in cubic meters
    const double pricePerCubicMeter = 4.25; // Constant price

public:
    // Default constructor
    GasCustomer() : UtilityCustomer(), usage(0.0) {}

    // Parameterized constructor with usage first, then account number
    GasCustomer(double usage, int acc) : UtilityCustomer(acc), usage(usage) {}

    // Getter and setter for usage
    double getUsage() const { return usage; }
    void setUsage(double u) { usage = u; }

    // Calculate bill
    double calculateBill() const override { return usage * pricePerCubicMeter; }

    // Display method
    void Display() const override {
        cout << "Gas Utility Customer" << endl;
        cout << "Utility Customer "; 
        UtilityCustomer::Display();
        cout << "Gas Used (in cubic meters): " << getUsage() << endl;
        cout << "total charge: " << calculateBill() << endl;
    }
};

#endif
