This console application is an individual Problem-Based Learning (PBL) assignment designed in C++
 using Object-Oriented Programming principles.

Project File Structure

Exceptions.h -   Custom error handling classes for handling full flights or bad cancellations.
Passenger.h and Passenger.cpp    - Abstract base passenger structure with subclass tiers.
Flight.h and Flight.cpp -   Abstract flight tracking blueprint containing domestic, international, and 
charter structures.
Ticket.h and Ticket.cpp -   Seat allocation asset mapping relationships between passengers and flights.
Templates.h -   Generic search algorithm utility.
Airline.h and Airline.cpp -  Core orchestration component managing state, logic processing, and reporting.
main.cpp -    User interface control menu loop framework.

How to Build and Run in Dev-C++?
Launch the Dev-C++ IDE.
Click File,
 select Open, and open the Skylink.dev project file from this directory.
Look at the workspace tabs and ensure your active editor view tab is set on main.cpp.
Press F11 on your keyboard 
Data Persistence Note:
On its very first run, the system automatically populates the workspace with 10 sample flights and 8 
sample passengers for demonstration.
When you exit the menu using option 6, the current state of the application automatically saves to a 
file named airline_data.txt, which reloads automatically when the application is restarted next time.