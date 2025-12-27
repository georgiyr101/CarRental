#include "Vehicle.h"
#include <iomanip>
int Vehicle::nextId = 1;
void Vehicle::setNextId(int id) {nextId = id;}
int Vehicle::getNextId() {return nextId;}
ostream& operator<<(ostream& os, const Vehicle& v)
{
    os << v.id << " " << v.brand << " " << v.model << " " << v.licensePlate << " "
       << v.pricePerDay << " " << v.isAvailable << " " << v.year << " " 
       << v.mileage << " " << v.color;
    return os;  
}
istream& operator>>(istream& is, Vehicle& v)
{
    is >> v.id >> v.brand >> v.model >> v.licensePlate
       >> v.pricePerDay >> v.isAvailable >> v.year 
       >> v.mileage >> v.color;
    return is;
}
int Vehicle::getId() const {return id;}
string Vehicle::getBrand() const {return brand;}
string Vehicle::getModel() const {return model;}
string Vehicle::getLicensePlate() const {return licensePlate;}
double Vehicle::getPricePerDay() const {return pricePerDay;}
bool Vehicle::getIsAvailable() const {return isAvailable;}
void Vehicle::setId(int id) { this->id = id; }
void Vehicle::setBrand(const string& brand) { this->brand = brand; }
void Vehicle::setModel(const string& model) { this->model = model; }
void Vehicle::setLicensePlate(const string& plate) { this->licensePlate = plate; }
void Vehicle::setPricePerDay(double price) { this->pricePerDay = price; }
void Vehicle::setIsAvailable(bool available) { this->isAvailable = available; }
int Vehicle::getYear() const { return year; }
double Vehicle::getMileage() const { return mileage; }
string Vehicle::getColor() const { return color; }
void Vehicle::setYear(int year) { this->year = year; }
void Vehicle::setMileage(double mileage) { this->mileage = mileage; }
void Vehicle::setColor(const string& color) { this->color = color; }
void Vehicle::printHeader() const
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
}
void Vehicle::printInfo() const
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
}
double Vehicle::calculateRentPrice(int days) {return pricePerDay * days;}
