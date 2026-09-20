#include "Flight.h"

Flight::Flight(std::string fn, std::string orig, std::string dest, std::string depTime, int seats, double fare, std::string type)
    : flightNumber(fn), origin(orig), destination(dest), departureTime(depTime), totalSeats(seats), availableSeats(seats), baseFare(fare), flightType(type) {}

std::string Flight::getFlightNumber() const { return flightNumber; }
std::string Flight::getOrigin() const { return origin; }
std::string Flight::getDestination() const { return destination; }
std::string Flight::getDepartureTime() const { return departureTime; }
int Flight::getTotalSeats() const { return totalSeats; }
int Flight::getAvailableSeats() const { return availableSeats; }
double Flight::getBaseFare() const { return baseFare; }
std::string Flight::getFlightType() const { return flightType; }
double Flight::getRevenue() const { return (totalSeats - availableSeats) * calculateBaseFare(); }

void Flight::bookSeat() { if (availableSeats > 0) availableSeats--; }
void Flight::releaseSeat() { if (availableSeats < totalSeats) availableSeats++; }

void Flight::displayDetails(std::ostream& os) const {
    os << "[" << flightType << "] Flight " << flightNumber << ": " << origin << " -> " << destination 
       << " | Departs: " << departureTime << " | Seats Available: " << availableSeats << "/" << totalSeats 
       << " | Base Fare: $" << calculateBaseFare();
}

std::ostream& operator<<(std::ostream& os, const Flight& flight) {
    flight.displayDetails(os);
    return os;
}

DomesticFlight::DomesticFlight(std::string fn, std::string orig, std::string dest, std::string depTime, int seats, double fare)
    : Flight(fn, orig, dest, depTime, seats, fare, "Domestic") {}
double DomesticFlight::calculateBaseFare() const { return baseFare; }

InternationalFlight::InternationalFlight(std::string fn, std::string orig, std::string dest, std::string depTime, int seats, double fare, std::string visa)
    : Flight(fn, orig, dest, depTime, seats, fare, "International"), visaRequirements(visa) {}
double InternationalFlight::calculateBaseFare() const { return baseFare + 150.0; } 
void InternationalFlight::displayDetails(std::ostream& os) const {
    Flight::displayDetails(os);
    os << " | Visa req: " << visaRequirements;
}
std::string InternationalFlight::getVisaRequirements() const { return visaRequirements; }

CharterFlight::CharterFlight(std::string fn, std::string orig, std::string dest, std::string depTime, int seats, double fare, std::string holder)
    : Flight(fn, orig, dest, depTime, seats, fare, "Charter"), contractHolder(holder) {}
double CharterFlight::calculateBaseFare() const { return baseFare * 1.2; } 
void CharterFlight::displayDetails(std::ostream& os) const {
    Flight::displayDetails(os);
    os << " | Holder: " << contractHolder;
}
std::string CharterFlight::getContractHolder() const { return contractHolder; }
