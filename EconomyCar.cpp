#include "EconomyCar.h"
#include "Input.h"
#include <iomanip>

double EconomyCar::calculateRentPrice(int days) { return getPricePerDay() * days * classCoefficient; }

ostream& operator<<(ostream& os, const EconomyCar& car)
{
    os << static_cast<const Car&>(car);
    return os;
}

istream& operator>>(istream& is, EconomyCar& car)
{
    is >> static_cast<Car&>(car);
    return is;
}

