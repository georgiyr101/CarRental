#include "ElectricCar.h"
#include <iomanip>

double ElectricCar::getBatteryCapacity() const {return batteryCapacity;}

double ElectricCar::getRangePerCharge() const {return rangePerCharge;}

double ElectricCar::getChargeTime() const {return chargeTime;}

void ElectricCar::setBatteryCapacity(double capacity) { this->batteryCapacity = capacity; }

void ElectricCar::setRangePerCharge(double range) { this->rangePerCharge = range; }

void ElectricCar::setChargeTime(double time) { this->chargeTime = time; }

void ElectricCar::printHeader() const 
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
    cout << left << setw(20) << "Ёмкость батареи";
    cout << left << setw(20) << "Запас хода";
    cout << left << setw(15) << "Время зарядки";
}

void ElectricCar::printInfo() const 
{
    cout << left << setw(5) << this->getId()
        << left << setw(12) << brand
        << left << setw(12) << model
        << left << setw(15) << licensePlate
        << left << setw(10) << pricePerDay
        << left << setw(6) << year
        << left << setw(10) << mileage
        << left << setw(10) << color
        << left << setw(12) << (isAvailable ? "yes" : "no");
    cout << left << setw(20) << batteryCapacity << " kWh";
    cout << left << setw(20) << rangePerCharge << " km";
    cout << left << setw(15) << chargeTime << " hours";
}

ostream& operator<<(ostream& os, const ElectricCar& ec) 
{
    os << static_cast<const Vehicle&>(ec);
    os << " " << ec.batteryCapacity << " " << ec.rangePerCharge << " " << ec.chargeTime;
    return os;
}

istream& operator>>(istream& is, ElectricCar& ec) 
{
    is >> static_cast<Vehicle&>(ec);
    cout << "Ёмкость батареи (кВт·ч): ";
    is >> ec.batteryCapacity;
    cout << "Запас хода (км): ";
    is >> ec.rangePerCharge;
    cout << "Время полной зарядки (часы): ";
    is >> ec.chargeTime;
    return is;
}

double ElectricCar::calculateRentPrice(int days) 
{
    return pricePerDay * days;
}
