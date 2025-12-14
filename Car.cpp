#include "Car.h"
#include <iomanip>

string Car::getFuelType() const {return fuelType;}

string Car::getTransmissionType() const {return transmissionType;}

void Car::setFuelType(string fuel) { this->fuelType = fuel; }

void Car::setTransmissionType(string transmission) { this->transmissionType = transmission; }

void Car::printHeader() const
{
    cout << left << setw(5) << "ID"
        << left << setw(12) << "Brand"
        << left << setw(12) << "Model"
        << left << setw(15) << "License"
        << left << setw(10) << "Price"
        << left << setw(6) << "Year"
        << left << setw(10) << "Mileage"
        << left << setw(10) << "Color"
        << left << setw(12) << "Available";
    cout << left << setw(15) << "Fuel Type";
    cout << left << setw(15) << "TransmissionType";
    cout << endl;
}

void Car::printInfo() const
{
    cout << left << setw(5) << id
        << left << setw(12) << brand
        << left << setw(12) << model
        << left << setw(15) << licensePlate
        << left << setw(10) << pricePerDay
        << left << setw(6) << year
        << left << setw(10) << mileage
        << left << setw(10) << color
        << left << setw(12) << (isAvailable ? "yes" : "no");
    cout << left << setw(15) << fuelType;
    cout << left << setw(15) << transmissionType;
    cout << endl;
}

double Car::calculateRentPrice(int days)
{
    return pricePerDay * days;
}

ostream& operator<<(ostream& os, const Car& car)
{
    os << static_cast<const Vehicle&>(car);
    os << " " << car.fuelType << " " << car.transmissionType;
    return os;
}

istream& operator>>(istream& is, Car& car)
{
    // Читаем базовый класс Vehicle
    is >> static_cast<Vehicle&>(car);
    // Читаем поля Car
    is >> car.fuelType >> car.transmissionType;
    return is;
}


