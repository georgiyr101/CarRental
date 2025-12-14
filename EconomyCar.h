#ifndef ECONOMYCAR_H
#define ECONOMYCAR_H

#include "Car.h"

class EconomyCar : public Car {
    double classCoefficient = 2.0;
public:
    EconomyCar(){}
    EconomyCar(const string& brand, const string& model, const string& licensePlate,
        double pricePerDay, bool isAvailable, int year, double mileage, const string& color, const string& fuelType,
        const string& transmissionType)
        : Car(brand, model, licensePlate, pricePerDay, isAvailable, year, mileage, color,  fuelType, transmissionType) {}
    virtual ~EconomyCar() = default;

    virtual double calculateRentPrice(int days) override;

    friend ostream& operator<<(ostream& os, const EconomyCar& car);
    friend istream& operator>>(istream& is, EconomyCar& car);
};

#endif