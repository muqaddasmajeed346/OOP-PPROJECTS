#ifndef AIRLINE_H
#define AIRLINE_H

#include <vector>
#include <map>
#include <string>
#include "Flight.h"
#include "Passenger.h"
#include "Ticket.h"

class Airline {
private:
    std::vector<Flight*> flights;
    std::vector<Passenger*> passengers;
    std::map<std::string, Ticket> tickets; 
    int ticketCounter;

public:
    Airline();
    ~Airline();

    // Flight management
    void addFlight(Flight* f);
    void removeFlight(const std::string& flightNum);
    void searchFlightsByRoute(const std::string& orig, const std::string& dest) const;
    void listAllFlights() const;
    Flight* findFlight(const std::string& flightNum) const;

    // Passenger management
    void registerPassenger(Passenger* p);
    void removePassenger(const std::string& pId);
    void viewPassengerHistory(const std::string& pId) const;
    Passenger* findPassenger(const std::string& pId) const;

    // Booking & Cancellation
    void bookTicket(const std::string& pId, const std::string& flightNum);
    void cancelTicket(const std::string& ticketId, int daysBeforeDeparture);

    // Reports
    void reportTodayDepartures(const std::string& date) const;
    void reportOccupancyPercentages() const;
    void reportTopHighestRevenueFlights() const;

    // Persistence
    void saveState(const std::string& filename) const;
    void loadState(const std::string& filename);
};

#endif
