#include "PremiumCar.h"
#include <iomanip>

bool PremiumCar::getHasDriver() const {
    return hasDriver;
}

void PremiumCar::setHasDriver(bool driver) {
    hasDriver = driver;
}

void PremiumCar::printHeader() const {
    Car::printHeader();
    cout << left << setw(12) << "Has Driver";
    cout << endl;
}

void PremiumCar::printInfo() const {
    cout << left << setw(5) << getId()
        << left << setw(12) << getBrand()
        << left << setw(12) << getModel()
        << left << setw(15) << getLicensePlate()
        << left << setw(10) << getPricePerDay()
        << left << setw(6) << getYear()
        << left << setw(10) << getMileage()
        << left << setw(10) << getColor()
        << left << setw(12) << (getIsAvailable() ? "yes" : "no")
        << left << setw(15) << getFuelType()
        << left << setw(15) << getTransmissionType()
        << left << setw(12) << (hasDriver ? "yes" : "no")
        << endl;
}

double PremiumCar::calculateRentPrice(int days) {
    double price = getPricePerDay() * days;
    if (hasDriver) price += (pricePerDay * 0.2);
    return price;
}

ostream& operator<<(ostream& os, const PremiumCar& car)
{
    os << static_cast<const Car&>(car);
    os << " " << car.hasDriver;
    return os;
}

istream& operator>>(istream& is, PremiumCar& car)
{
    is >> static_cast<Car&>(car);
    is >> car.hasDriver;
    return is;
}

