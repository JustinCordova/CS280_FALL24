#ifndef ELECTRICITYCUSTOMER_H
#define ELECTRICITYCUSTOMER_H

#include "UtilityCustomer.h"

class ElectricityCustomer : public UtilityCustomer {
private:
    double kWattHourUsed; // kWh used
    const double pricePerKWh = 0.3; // Constant price

public:
    // Default constructor
    ElectricityCustomer() : UtilityCustomer(), kWattHourUsed(0.0) {}

    // Parameterized constructor
    ElectricityCustomer(double usage, int acc) : UtilityCustomer(acc), kWattHourUsed(usage) {}

    // Getter and setter for kWh used
    double getkWattHourUsed() const { return kWattHourUsed; }
    void setkWattHourUsed(double kWh) { kWattHourUsed = kWh; }
    // Calculate bill
    double calculateBill() const override { return kWattHourUsed * pricePerKWh; }

    // Display method
    void Display() const override {
        cout << "Electric Utility Customer" << endl;
        cout << "Utility Customer "; 
        UtilityCustomer::Display();
        cout << "Electricity Used (in kWattHour): " << getkWattHourUsed() << endl;
        cout << "total charge: " << calculateBill() << endl;
    }
};

#endif
