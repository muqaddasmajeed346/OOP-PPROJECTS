#include <iostream>
#include <string>
#include <fstream> 
#include "Airline.h"
#include "Exceptions.h"

void displayMenu() {
    std::cout << "\n=========================================\n";
    std::cout << "      SKYLINK AIRWAYS MANAGEMENT         \n";
    std::cout << "=========================================\n";
    std::cout << "1. Manage Flights (Add/View/Search)\n";
    std::cout << "2. Manage Passengers (Register/History)\n";
    std::cout << "3. Book a Ticket\n";
    std::cout << "4. Cancel a Ticket\n";
    std::cout << "5. View Analytical Reports\n";
    std::cout << "6. Exit Application\n";
    std::cout << "Enter selection choices: ";
}

void loadSampleData(Airline& airline) {
    // Injecting 10 sample flights as requested by deliverables
    airline.addFlight(new DomesticFlight("SK-101", "Lahore", "Karachi", "2026-06-02", 50, 120));
    airline.addFlight(new DomesticFlight("SK-102", "Islamabad", "Lahore", "2026-06-02", 40, 95));
    airline.addFlight(new InternationalFlight("SK-201", "Karachi", "Dubai", "2026-06-03", 150, 450, "Visa On Arrival"));
    airline.addFlight(new InternationalFlight("SK-202", "Lahore", "London", "2026-06-05", 200, 850, "Pre-entry Visa Validated"));
    airline.addFlight(new CharterFlight("SK-301", "Islamabad", "Chitral", "2026-06-02", 12, 1200, "Northern Tours PLC"));
    airline.addFlight(new DomesticFlight("SK-103", "Multan", "Karachi", "2026-06-04", 30, 110));
    airline.addFlight(new DomesticFlight("SK-104", "Sialkot", "Sharjah", "2026-06-06", 120, 310));
    airline.addFlight(new InternationalFlight("SK-203", "Islamabad", "New York", "2026-06-10", 250, 1100, "US Visa Entry Tier 1"));
    airline.addFlight(new InternationalFlight("SK-204", "Karachi", "Istanbul", "2026-06-12", 180, 600, "E-Visa Allowed"));
    airline.addFlight(new CharterFlight("SK-302", "Lahore", "Skardu", "2026-06-15", 10, 1500, "Apex Mountaineering"));

    // Injecting 8 sample passengers as requested by deliverables
    airline.registerPassenger(new EconomyPassenger("P-001", "Amina Khan"));
    airline.registerPassenger(new BusinessPassenger("P-002", "Zayd Malik"));
    airline.registerPassenger(new FirstClassPassenger("P-003", "Sarah Ahmed"));
    airline.registerPassenger(new EconomyPassenger("P-004", "Bilal Siddiqui"));
    airline.registerPassenger(new EconomyPassenger("P-005", "Fatima Sana"));
    airline.registerPassenger(new BusinessPassenger("P-006", "Omar Raza"));
    airline.registerPassenger(new FirstClassPassenger("P-007", "Mariam Jameel"));
    airline.registerPassenger(new EconomyPassenger("P-008", "Hamza Ali"));
}

int main() {
    Airline airline;
    
    // System automatically attempts recovery of stored database or loads defaults
    std::ifstream check("airline_data.txt");
    if (check.good()) {
        check.close();
        airline.loadState("airline_data.txt");
    } else {
        loadSampleData(airline);
    }

    int choice = 0;
    while (choice != 6) {
        displayMenu();
        if (!(std::cin >> choice)) {
            std::cout << "Invalid terminal input format. Try again.\n";
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            continue;
        }

        try {
            if (choice == 1) {
                std::cout << "1. List All Flights\n2. Search Flights Route\nSelect: ";
                int sub; std::cin >> sub;
                if (sub == 1) airline.listAllFlights();
                else if (sub == 2) {
                    std::string o, d;
                    std::cout << "Origin: "; std::cin >> o;
                    std::cout << "Destination: "; std::cin >> d;
                    airline.searchFlightsByRoute(o, d);
                }
            } 
            else if (choice == 2) {
                std::string pid;
                std::cout << "Enter unique Passenger ID: "; std::cin >> pid;
                airline.viewPassengerHistory(pid);
            } 
            else if (choice == 3) {
                std::string pid, fnum;
                std::cout << "Enter Passenger ID: "; std::cin >> pid;
                std::cout << "Enter Flight Number: "; std::cin >> fnum;
                airline.bookTicket(pid, fnum);
            } 
            else if (choice == 4) {
                std::string tid; int days;
                std::cout << "Enter Alphanumeric Ticket ID: "; std::cin >> tid;
                std::cout << "Days remaining until scheduled flight departure: "; std::cin >> days;
                airline.cancelTicket(tid, days);
            } 
            else if (choice == 5) {
                std::cout << "1. Current Daily Departures\n2. Track Occupancy Breakdown\n3. Top Revenue Generating Assets\nSelect: ";
                int sub; std::cin >> sub;
                if (sub == 1) {
                    std::string dt; std::cout << "Enter Query Date (YYYY-MM-DD format): "; std::cin >> dt;
                    airline.reportTodayDepartures(dt);
                } else if (sub == 2) airline.reportOccupancyPercentages();
                else if (sub == 3) airline.reportTopHighestRevenueFlights();
            }
        } 
        catch (const FlightFullException& e) {
            std::cerr << "\n[CRITICAL VIOLATION]: " << e.what() << "\n";
        } 
        catch (const InvalidCancellationException& e) {
            std::cerr << "\n[EXECUTION BLOCK]: " << e.what() << "\n";
        }
    }

    // Auto data persistence processing at exit frame
    airline.saveState("airline_data.txt");
    std::cout << "System pipeline terminated normally. All logs exported to database.\n";
    return 0;
}
