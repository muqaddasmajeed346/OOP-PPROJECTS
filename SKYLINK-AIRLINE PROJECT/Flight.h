#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>
#include <iostream>

class Flight {
protected:
    std::string flightNumber;
    std::string origin;
    std::string destination;
    std::string departureTime; 
    int totalSeats;
    int availableSeats;
    double baseFare;
    std::string flightType;

public:
    Flight(std::string fn, std::string orig, std::string dest, std::string depTime, int seats, double fare, std::string type);
    virtual ~Flight() = default;

    std::string getFlightNumber() const;
    std::string getOrigin() const;
    std::string getDestination() const;
    std::string getDepartureTime() const;
    int getTotalSeats() const;
    int getAvailableSeats() const;
    double getBaseFare() const;
    std::string getFlightType() const;
    double getRevenue() const;

    void bookSeat();
    void releaseSeat();
//pure virtual funnction::::
    virtual double calculateBaseFare() const = 0;
    virtual void displayDetails(std::ostream& os) const;

    friend std::ostream& operator<<(std::ostream& os, const Flight& flight);
};

class DomesticFlight : public Flight {
public:
    DomesticFlight(std::string fn, std::string orig, std::string dest, std::string depTime, int seats, double fare);
    double calculateBaseFare() const override;
};

class InternationalFlight : public Flight {
private:
    std::string visaRequirements;
public:
    InternationalFlight(std::string fn, std::string orig, std::string dest, std::string depTime, int seats, double fare, std::string visa);
    double calculateBaseFare() const override;
    void displayDetails(std::ostream& os) const override;
    std::string getVisaRequirements() const;
};

class CharterFlight : public Flight {
private:
    std::string contractHolder;
public:
    CharterFlight(std::string fn, std::string orig, std::string dest, std::string depTime, int seats, double fare, std::string holder);
    double calculateBaseFare() const override;
    void displayDetails(std::ostream& os) const override;
    std::string getContractHolder() const;
};

#endif
