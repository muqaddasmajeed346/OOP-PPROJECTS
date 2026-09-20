#ifndef TICKET_H
#define TICKET_H

#include <string>
#include <iostream>

class Ticket {
private:
    std::string ticketID;
    std::string passengerID;
    std::string flightNumber;
    int seatNumber;
    double farePaid;
    std::string bookingStatus; 

public:
    Ticket() = default;
    Ticket(std::string tId, std::string pId, std::string fNum, int seat, double fare, std::string status = "Confirmed");

    std::string getTicketID() const;
    std::string getPassengerID() const;
    std::string getFlightNumber() const;
    int getSeatNumber() const;
    double getFarePaid() const;
    std::string getBookingStatus() const;
    
    void cancelTicket();

    bool operator==(const Ticket& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Ticket& ticket);
};

#endif
