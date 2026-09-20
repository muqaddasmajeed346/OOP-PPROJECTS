#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>
#include <iostream>

class Passenger {
protected:
    std::string passengerID;
    std::string name;
    std::string type;
public:
    Passenger(std::string id, std::string n, std::string t);
    virtual ~Passenger() = default;

    std::string getID() const;
    std::string getName() const;
    std::string getType() const;

    virtual double getBaggageAllowance() const = 0;
    virtual double getLoyaltyMultiplier() const = 0;
    virtual double getRefundPercentage() const = 0;
    virtual void display(std::ostream& os) const;
};

class EconomyPassenger : public Passenger {
public:
    EconomyPassenger(std::string id, std::string n);
    double getBaggageAllowance() const override;
    double getLoyaltyMultiplier() const override;
    double getRefundPercentage() const override;
};

class BusinessPassenger : public Passenger {
public:
    BusinessPassenger(std::string id, std::string n);
    double getBaggageAllowance() const override;
    double getLoyaltyMultiplier() const override;
    double getRefundPercentage() const override;
};

class FirstClassPassenger : public Passenger {
public:
    FirstClassPassenger(std::string id, std::string n);
    double getBaggageAllowance() const override;
    double getLoyaltyMultiplier() const override;
    double getRefundPercentage() const override;
};

#endif
