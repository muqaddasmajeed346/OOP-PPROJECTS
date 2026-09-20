#include "Ticket.h"

Ticket::Ticket(std::string tId, std::string pId, std::string fNum, int seat, double fare, std::string status)
    : ticketID(tId), passengerID(pId), flightNumber(fNum), seatNumber(seat), farePaid(fare), bookingStatus(status) {}

std::string Ticket::getTicketID() const { return ticketID; }
std::string Ticket::getPassengerID() const { return passengerID; }
std::string Ticket::getFlightNumber() const { return flightNumber; }
int Ticket::getSeatNumber() const { return seatNumber; }
double Ticket::getFarePaid() const { return farePaid; }
std::string Ticket::getBookingStatus() const { return bookingStatus; }

void Ticket::cancelTicket() { bookingStatus = "Cancelled"; }

bool Ticket::operator==(const Ticket& other) const {
    return this->ticketID == other.ticketID;
}

std::ostream& operator<<(std::ostream& os, const Ticket& ticket) {
    os << "Ticket ID: " << ticket.ticketID << " | Flight: " << ticket.flightNumber 
       << " | Passenger: " << ticket.passengerID << " | Seat: " << ticket.seatNumber 
       << " | Paid: $" << ticket.farePaid << " | Status: " << ticket.bookingStatus;
    return os;
}
