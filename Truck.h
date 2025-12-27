#ifndef TRUCK_H
#define TRUCK_H

#include "Vehicle.h"

class Truck : public Vehicle
{
private:
    double maxLoad = 0;     
    double cargoVolume = 0; 
public:
    Truck(){}
    Truck(const string& brand, const string& model,
        const string& licensePlate, double pricePerDay, bool isAvailable, int year, double mileage, const string& color,
        double maxLoad, double cargoVolume)
        : Vehicle(brand, model, licensePlate, pricePerDay, isAvailable, year, mileage, color),
        maxLoad(maxLoad), cargoVolume(cargoVolume) {}
    virtual ~Truck() = default;
    double getMaxLoad() const;
    double getCargoVolume() const;
    void setMaxLoad(double load);
    void setCargoVolume(double volume);
    virtual void printHeader() const override;
    virtual void printInfo() const override;
    virtual double calculateRentPrice(int days) override;
    friend ostream& operator<<(ostream& os, const Truck& truck);
    friend istream& operator>>(istream& is, Truck& truck);
};

#endif
