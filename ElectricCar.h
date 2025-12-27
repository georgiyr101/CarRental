#ifndef ELECTRICCAR_H
#define ELECTRICCAR_H

#include "Vehicle.h"

class ElectricCar : public Vehicle
{
private:
    double batteryCapacity = 0.0; 
    double rangePerCharge = 0.0;  
    double chargeTime = 0.0;
public:
    ElectricCar(){}
    ElectricCar(const string& brand, const string& model,
        const string& licensePlate, double pricePerDay, bool isAvailable, int year, double mileage, const string& color,
        double batteryCapacity, double rangePerCharge,
        double chargeTime )
        : Vehicle(brand, model, licensePlate, pricePerDay, isAvailable, year, mileage, color),
        batteryCapacity(batteryCapacity),
        rangePerCharge(rangePerCharge),
        chargeTime(chargeTime) {
    }
    virtual ~ElectricCar() = default;
    double getBatteryCapacity() const;
    double getRangePerCharge() const;
    double getChargeTime() const;
    void setBatteryCapacity(double capacity);
    void setRangePerCharge(double range);
    void setChargeTime(double time);
    virtual void printHeader() const override;
    virtual void printInfo() const override;
    virtual double calculateRentPrice(int days) override;
    friend ostream& operator << (ostream& os, const ElectricCar& ec);
    friend istream& operator >> (istream& is, ElectricCar& ec);
};

#endif
