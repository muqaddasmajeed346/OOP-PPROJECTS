#include "Passenger.h"

Passenger::Passenger(std::string id, std::string n, std::string t) 
    : passengerID(id), name(n), type(t) {}

std::string Passenger::getID() const { return passengerID; }
std::string Passenger::getName() const { return name; }
std::string Passenger::getType() const { return type; }

void Passenger::display(std::ostream& os) const {
    os << "ID: " << passengerID << " | Name: " << name << " | Type: " << type;
}

EconomyPassenger::EconomyPassenger(std::string id, std::string n) : Passenger(id, n, "Economy") {}
double EconomyPassenger::getBaggageAllowance() const { return 23.0; }
double EconomyPassenger::getLoyaltyMultiplier() const { return 1.0; }
double EconomyPassenger::getRefundPercentage() const { return 0.50; }

BusinessPassenger::BusinessPassenger(std::string id, std::string n) : Passenger(id, n, "Business") {}
double BusinessPassenger::getBaggageAllowance() const { return 40.0; }
double BusinessPassenger::getLoyaltyMultiplier() const { return 1.5; }
double BusinessPassenger::getRefundPercentage() const { return 0.75; }

FirstClassPassenger::FirstClassPassenger(std::string id, std::string n) : Passenger(id, n, "FirstClass") {}
double FirstClassPassenger::getBaggageAllowance() const { return 50.0; }
double FirstClassPassenger::getLoyaltyMultiplier() const { return 2.0; }
double FirstClassPassenger::getRefundPercentage() const { return 0.90; }
