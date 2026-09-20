#include "Airline.h"
#include "Templates.h"
#include "Exceptions.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>

Airline::Airline() : ticketCounter(1000) {}

Airline::~Airline() {
    for (auto f : flights) delete f;
    for (auto p : passengers) delete p;
}

void Airline::addFlight(Flight* f) { flights.push_back(f); }

void Airline::removeFlight(const std::string& flightNum) {
    auto it = std::remove_if(flights.begin(), flights.end(), [&](Flight* f) {
        if (f->getFlightNumber() == flightNum) {
            delete f;
            return true;
        }
        return false;
    });
    flights.erase(it, flights.end());
}

void Airline::searchFlightsByRoute(const std::string& orig, const std::string& dest) const {
    bool found = false;
    for (const auto& f : flights) {
        if (f->getOrigin() == orig && f->getDestination() == dest) {
            std::cout << *f << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "No flights found matching route: " << orig << " -> " << dest << "\n";
}

void Airline::listAllFlights() const {
    for (const auto& f : flights) std::cout << *f << "\n";
}

Flight* Airline::findFlight(const std::string& flightNum) const {
    return genericSearch(flights, [&](Flight* f) { return f->getFlightNumber() == flightNum; });
}

void Airline::registerPassenger(Passenger* p) { passengers.push_back(p); }

void Airline::removePassenger(const std::string& pId) {
    auto it = std::remove_if(passengers.begin(), passengers.end(), [&](Passenger* p) {
        if (p->getID() == pId) {
            delete p;
            return true;
        }
        return false;
    });
    passengers.erase(it, passengers.end());
}

Passenger* Airline::findPassenger(const std::string& pId) const {
    return genericSearch(passengers, [&](Passenger* p) { return p->getID() == pId; });
}

void Airline::viewPassengerHistory(const std::string& pId) const {
    std::cout << "--- Booking History for Passenger " << pId << " ---\n";
    bool found = false;
    for (const auto& pair : tickets) {
        if (pair.second.getPassengerID() == pId) {
            std::cout << pair.second << "\n";
            found = true;
        }
    }
    if (!found) std::cout << "No booking history recorded.\n";
}

void Airline::bookTicket(const std::string& pId, const std::string& flightNum) {
    Passenger* p = findPassenger(pId);
    Flight* f = findFlight(flightNum);

    if (!p || !f) {
        std::cout << "Error: Invalid Passenger ID or Flight Number.\n";
        return;
    }

    if (f->getAvailableSeats() <= 0) {
        throw FlightFullException("Cannot book ticket. Flight " + flightNum + " is fully occupied.");
    }

    for (const auto& pair : tickets) {
        if (pair.second.getPassengerID() == pId && 
            pair.second.getFlightNumber() == flightNum && 
            pair.second.getBookingStatus() == "Confirmed") {
            std::cout << "Error: Passenger already holds a active ticket on this flight.\n";
            return;
        }
    }

    int assignedSeat = f->getTotalSeats() - f->getAvailableSeats() + 1;
    f->bookSeat();

    double calculatedFare = f->calculateBaseFare() * p->getLoyaltyMultiplier();
    std::string tId = "TK-" + std::to_string(++ticketCounter);

    Ticket newTicket(tId, pId, flightNum, assignedSeat, calculatedFare);
    tickets[tId] = newTicket;

    std::cout << "Successfully Booked Ticket! Info:\n" << newTicket << "\n";
}

void Airline::cancelTicket(const std::string& ticketId, int daysBeforeDeparture) {
    auto it = tickets.find(ticketId);
    if (it == tickets.end()) {
        std::cout << "Error: Ticket ID context not found.\n";
        return;
    }

    if (it->second.getBookingStatus() == "Cancelled") {
        throw InvalidCancellationException("Ticket " + ticketId + " has already been cancelled.");
    }

    Flight* f = findFlight(it->second.getFlightNumber());
    Passenger* p = findPassenger(it->second.getPassengerID());

    if (daysBeforeDeparture < 1) {
        throw InvalidCancellationException("Cancellations within 24 hours of departure window are completely restricted.");
    }

    it->second.cancelTicket();
    if (f) f->releaseSeat();

    double refundPercentage = p ? p->getRefundPercentage() : 0.50;
    if (daysBeforeDeparture < 3) {
        refundPercentage *= 0.5; 
    }

    double finalRefund = it->second.getFarePaid() * refundPercentage;
    std::cout << "Cancellation Successful. Refund Processed: $" << finalRefund << "\n";
}

void Airline::reportTodayDepartures(const std::string& date) const {
    std::cout << "\n--- Scheduled Departures for [ " << date << " ] ---\n";
    for (const auto& f : flights) {
        if (f->getDepartureTime().find(date) != std::string::npos) {
            std::cout << *f << "\n";
        }
    }
}

void Airline::reportOccupancyPercentages() const {
    std::cout << "\n--- Flight Occupancy Distribution ---\n";
    for (const auto& f : flights) {
        double occupancy = ((double)(f->getTotalSeats() - f->getAvailableSeats()) / f->getTotalSeats()) * 100.0;
        std::cout << "Flight " << f->getFlightNumber() << ": " << occupancy << "% Occupied\n";
    }
}

void Airline::reportTopHighestRevenueFlights() const {
    std::cout << "\n--- Top Dynamic Performing Flights by Revenue ---\n";
    std::vector<Flight*> sortedFlights = flights;
    std::sort(sortedFlights.begin(), sortedFlights.end(), [](Flight* a, Flight* b) {
        return a->getRevenue() > b->getRevenue();
    });

    int limit = std::min(5, (int)sortedFlights.size());
    for (int i = 0; i < limit; ++i) {
        std::cout << i + 1 << ". Flight " << sortedFlights[i]->getFlightNumber() 
                  << " | Revenue Secured: $" << sortedFlights[i]->getRevenue() << "\n";
    }
}

void Airline::saveState(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out) return;

    out << "---FLIGHTS---\n";
    for (const auto& f : flights) {
        out << f->getFlightType() << "," << f->getFlightNumber() << "," << f->getOrigin() << ","
            << f->getDestination() << "," << f->getDepartureTime() << "," << f->getTotalSeats() << ","
            << f->getBaseFare() << ",";
        if (f->getFlightType() == "International") {
            out << static_cast<InternationalFlight*>(f)->getVisaRequirements();
        } else if (f->getFlightType() == "Charter") {
            out << static_cast<CharterFlight*>(f)->getContractHolder();
        } else {
            out << "None";
        }
        out << "\n";
    }

    out << "---PASSENGERS---\n";
    for (const auto& p : passengers) {
        out << p->getType() << "," << p->getID() << "," << p->getName() << "\n";
    }

    out << "---TICKETS---\n";
    for (const auto& pair : tickets) {
        out << pair.second.getTicketID() << "," << pair.second.getPassengerID() << ","
            << pair.second.getFlightNumber() << "," << pair.second.getSeatNumber() << ","
            << pair.second.getFarePaid() << "," << pair.second.getBookingStatus() << "\n";
    }
}

void Airline::loadState(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) return;

    std::string line, section = "";
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (line == "---FLIGHTS---" || line == "---PASSENGERS---" || line == "---TICKETS---") {
            section = line;
            continue;
        }

        std::stringstream ss(line);
        std::string field;

        if (section == "---FLIGHTS---") {
            std::string type, fn, orig, dest, dep;
            int seats; double fare; std::string extra;
            std::getline(ss, type, ','); std::getline(ss, fn, ',');
            std::getline(ss, orig, ','); std::getline(ss, dest, ',');
            std::getline(ss, dep, ','); std::getline(ss, field, ','); seats = std::stoi(field);
            std::getline(ss, field, ','); fare = std::stod(field);
            std::getline(ss, extra, '\n');

            if (type == "Domestic") addFlight(new DomesticFlight(fn, orig, dest, dep, seats, fare));
            else if (type == "International") addFlight(new InternationalFlight(fn, orig, dest, dep, seats, fare, extra));
            else if (type == "Charter") addFlight(new CharterFlight(fn, orig, dest, dep, seats, fare, extra));

        } else if (section == "---PASSENGERS---") {
            std::string type, id, name;
            std::getline(ss, type, ','); std::getline(ss, id, ','); std::getline(ss, name, '\n');

            if (type == "Economy") registerPassenger(new EconomyPassenger(id, name));
            else if (type == "Business") registerPassenger(new BusinessPassenger(id, name));
            else if (type == "FirstClass") registerPassenger(new FirstClassPassenger(id, name));

        } else if (section == "---TICKETS---") {
            std::string tid, pid, fnum, status;
            int seat; double farePaid;
            std::getline(ss, tid, ','); std::getline(ss, pid, ','); std::getline(ss, fnum, ',');
            std::getline(ss, field, ','); seat = std::stoi(field);
            std::getline(ss, field, ','); farePaid = std::stod(field);
            std::getline(ss, status, '\n');

            tickets[tid] = Ticket(tid, pid, fnum, seat, farePaid, status);
            Flight* f = findFlight(fnum);
            if (f && status == "Confirmed") {
                f->bookSeat();
            }
        }
    }
}
