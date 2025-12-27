#ifndef CAR_H
#define CAR_H

#include "Vehicle.h"
class Car : public Vehicle
{
protected:
	string fuelType = "";
	string transmissionType = "";
public:
	Car() {}
	Car(const string& brand, const string& model, const string& licensePlate,
		double pricePerDay, bool isAvailable, int year, double mileage, const string& color, const string& fuelType, const string& transmissionType)
		:Vehicle(brand, model, licensePlate, pricePerDay, isAvailable, year, mileage, color),
		fuelType(fuelType), transmissionType(transmissionType) {
	}
	Car(const Car& other)
		: Vehicle(other), fuelType(other.fuelType), transmissionType(other.transmissionType) {
	}
	Car& operator=(const Car& other) {
		if (this != &other) {
			Vehicle::operator=(other);
			fuelType = other.fuelType;
			transmissionType = other.transmissionType;
		}
		return *this;
	}
	virtual ~Car() = default;
	string getFuelType() const;
	string getTransmissionType() const;
	void setFuelType(string fuel);
	void setTransmissionType(string transmission);
	virtual void printHeader() const override;
	virtual void printInfo() const override;
	virtual double calculateRentPrice(int days) override;
	friend ostream& operator << (ostream& os, const Car& car);
	friend istream& operator >> (istream& is, Car& car);
};

#endif
