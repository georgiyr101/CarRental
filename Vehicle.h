#ifndef VEHICLE_H
#define VEHICLE_H
#include <iostream>
#include <string>
using namespace std;
class Vehicle 
{
protected:
	int id = 0;
	string brand = "";
	string model = "";
	string licensePlate = "";
	double pricePerDay = 0.0;
	bool isAvailable = true;
	int year = 0;                   
	double mileage = 0.0;           
	string color = "";              
    static int nextId;

public:
    Vehicle() { id = nextId++; };
    Vehicle(const Vehicle& other)
        : id(other.id), brand(other.brand), model(other.model),
        licensePlate(other.licensePlate), pricePerDay(other.pricePerDay),
        isAvailable(other.isAvailable), year(other.year),
        mileage(other.mileage), color(other.color) {
    }
    Vehicle(const string& brand, const string& model,
        const string& licensePlate, double pricePerDay, bool isAvailable,
        int year = 0, double mileage = 0.0, const string& color = "")
        : id(nextId++), brand(brand), model(model), licensePlate(licensePlate),
        pricePerDay(pricePerDay), isAvailable(isAvailable),
        year(year), mileage(mileage), color(color) {

    }
    Vehicle& operator=(const Vehicle& other) {
        if (this != &other) {
            id = other.id;
            brand = other.brand;
            model = other.model;
            licensePlate = other.licensePlate;
            pricePerDay = other.pricePerDay;
            isAvailable = other.isAvailable;
            year = other.year;
            mileage = other.mileage;
            color = other.color;
        }
        return *this;
    }
    virtual ~Vehicle() = default;
    static void setNextId(int id);
    static int getNextId();
    int getId() const;
    string getBrand() const;
    string getModel() const;
    string getLicensePlate() const;
    double getPricePerDay() const;
    bool getIsAvailable() const;
    int getYear() const;
    double getMileage() const;
    string getColor() const;
    void setId(int id);
    void setBrand(const string& brand);
    void setModel(const string& model);
    void setLicensePlate(const string& plate);
    void setPricePerDay(double rate);
    void setIsAvailable(bool available);
    void setYear(int year);
    void setMileage(double mileage);
    void setColor(const string& color);
    virtual void printHeader() const;
    virtual void printInfo() const;
    virtual double calculateRentPrice(int days);
    friend ostream& operator<<(ostream& os, const Vehicle& v);
    friend istream& operator >> (istream& is, Vehicle& v);
};

#endif
