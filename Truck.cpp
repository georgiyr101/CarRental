#include "Truck.h"
#include <iomanip>

double Truck::getMaxLoad() const {return maxLoad;}
double Truck::getCargoVolume() const {return cargoVolume;}
void Truck::setMaxLoad(double load) {this->maxLoad = load;}
void Truck::setCargoVolume(double volume) { this->cargoVolume = volume; }
void Truck::printHeader() const 
{
    cout << left << setw(6) << "ID"
        << left << setw(12) << "Brand"
        << left << setw(12) << "Model"
        << left << setw(15) << "License"
        << left << setw(10) << "Price"
        << left << setw(6) << "Year"
        << left << setw(10) << "Mileage"
        << left << setw(10) << "Color"
        << left << setw(12) << "Available";
    cout << left << setw(15) << "Max Load";
    cout << left << setw(15) << "Cargo Volume";
    cout << endl;
}

void Truck::printInfo() const 
{
    cout << left << setw(6) << this->getId()
        << left << setw(12) << brand
        << left << setw(12) << model
        << left << setw(15) << licensePlate
        << left << setw(10) << pricePerDay
        << left << setw(6) << year
        << left << setw(10) << mileage
        << left << setw(10) << color
        << left << setw(12) << (isAvailable ? "yes" : "no");
    cout << left << setw(15) << maxLoad;
    cout << left << setw(15) << cargoVolume;
    cout << endl;
}

double Truck::calculateRentPrice(int days) {return pricePerDay * days;}

ostream& operator<<(ostream& os, const Truck& truck) 
{
    os << static_cast<const Vehicle&>(truck);
    os << " " << truck.maxLoad << " " << truck.cargoVolume;
    return os;
}

istream& operator>>(istream& is, Truck& truck)
{
    is >> static_cast<Vehicle&>(truck);
    cout << "Max load (tons): ";
    is >> truck.maxLoad;
    cout << "Cargo volume (cubic meters): ";
    is >> truck.cargoVolume;
    return is;
}