#ifndef PREMIUMCAR_H
#define PREMIUMCAR_H

#include "Car.h"

class PremiumCar : public Car {
    double classCoefficient = 3.0;
    bool hasDriver;
public:
    PremiumCar() {}
    PremiumCar(const string& brand, const string& model, const string& licensePlate,
        double pricePerDay, bool isAvailable, int year, double mileage, const string& color, const string& fuelType,
        const string& transmissionType, bool hasDriver)
        : Car(brand, model, licensePlate, pricePerDay, isAvailable, year, mileage, color, fuelType, transmissionType), hasDriver(hasDriver) {}
    virtual ~PremiumCar() = default;

    virtual double calculateRentPrice(int days) override;
    bool getHasDriver() const;
    void setHasDriver(bool driver);
    friend ostream& operator<<(ostream& os, const PremiumCar& car);
    friend istream& operator>>(istream& is, PremiumCar& car);
    virtual void printHeader() const override;
    virtual void printInfo() const override;
};

#endif
