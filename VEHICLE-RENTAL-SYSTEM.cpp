#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <iomanip>

// =================================================================
// 1. VEHICLE HIERARCHY
// =================================================================
class Vehicle {
protected:
    std::string makeModel;
    double dailyRate;
    bool isAvailable;

public:
    Vehicle(std::string model, double rate) 
        : makeModel(model), dailyRate(rate), isAvailable(true) {}
    
    virtual ~Vehicle() = default;

    std::string getMakeModel() const { return makeModel; }
    bool checkAvailable() const { return isAvailable; }
    void setAvailable(bool status) { isAvailable = status; }
    double getDailyRate() const { return dailyRate; }

    // Virtual method for polymorphic pricing logic (Requirement R4)
    virtual double calculateCost(int days) const {
        return dailyRate * days;
    }

    // Virtual method for polymorphic display (Requirement R1)
    virtual void printDetails() const {
        std::cout << makeModel << " (Daily Rate: $" << dailyRate << ")";
    }
};

class Car : public Vehicle {
private:
    int seats;

public:
    Car(std::string model, double rate, int seatsCount) 
        : Vehicle(model, rate), seats(seatsCount) {}

    void printDetails() const override {
        Vehicle::printDetails();
        std::cout << " [Type: Car, Seats: " << seats << "]";
    }
};

class Motorbike : public Vehicle {
public:
    Motorbike(std::string model, double rate) : Vehicle(model, rate) {}

    // Motorbikes receive a 10% discount for rentals longer than 7 days
    double calculateCost(int days) const override {
        double total = dailyRate * days;
        if (days > 7) {
            total *= 0.90; // 10% discount
        }
        return total;
    }

    void printDetails() const override {
        Vehicle::printDetails();
        std::cout << " [Type: Motorbike]";
    }
};

class Truck : public Vehicle {
private:
    double payloadCapacity; // in tonnes

public:
    Truck(std::string model, double rate, double capacity) 
        : Vehicle(model, rate), payloadCapacity(capacity) {}

    // Trucks incur a 20% surcharge on top of the daily rate
    double calculateCost(int days) const override {
        double baseTotal = dailyRate * days;
        return baseTotal * 1.20; // 20% surcharge
    }

    void printDetails() const override {
        Vehicle::printDetails();
        std::cout << " [Type: Truck, Payload: " << payloadCapacity << " tons]";
    }
};

// =================================================================
// 2. CUSTOMER CLASS
// =================================================================
class Customer {
private:
    std::string name;
    std::string customerID;
    bool hasActiveRental;

public:
    Customer(std::string cName, std::string id) 
        : name(cName), customerID(id), hasActiveRental(false) {}

    std::string getName() const { return name; }
    std::string getID() const { return customerID; }
    bool canRent() const { return !hasActiveRental; }
    void setHasActiveRental(bool status) { hasActiveRental = status; }
};

// =================================================================
// 3. RENTAL RECORD CLASS
// =================================================================
class Rental {
private:
    Customer* customer;
    Vehicle* vehicle;
    int days;
    bool isClosed;
    double finalCost;

public:
    Rental(Customer* c, Vehicle* v, int duration) 
        : customer(c), vehicle(v), days(duration), isClosed(false) {
        // Runtime Polymorphism in action here to compute cost based on vehicle type!
        finalCost = vehicle->calculateCost(days); 
    }

    Customer* getCustomer() const { return customer; }
    Vehicle* getVehicle() const { return vehicle; }
    bool checkClosed() const { return isClosed; }
    
    void closeRental() {
        isClosed = true;
    }

    void printRentalRecord() const {
        std::cout << "Customer: " << customer->getName() << " (ID: " << customer->getID() << ")\n";
        std::cout << "  Vehicle: ";
        vehicle->printDetails();
        std::cout << "\n  Duration: " << days << " days | Total Cost: $" << finalCost;
        std::cout << " | Status: " << (isClosed ? "CLOSED" : "ACTIVE") << "\n";
    }
};

// =================================================================
// 4. CENTRAL RENTAL SYSTEM (Data Owner)
// =================================================================
class RentalSystem {
private:
    std::vector<std::unique_ptr<Vehicle>> fleet;
    std::vector<Customer> customers;
    std::vector<Rental> rentals;

public:
    // R1: Add vehicles to the fleet
    void addVehicle(std::unique_ptr<Vehicle> v) {
        fleet.push_back(std::move(v));
    }

    // R2: Register customers
    void registerCustomer(const std::string& name, const std::string& id) {
        customers.emplace_back(name, id);
        std::cout << "Customer " << name << " registered successfully.\n";
    }

    // R3: Process a rental
    void processRental(const std::string& customerID, const std::string& modelName, int days) {
        Customer* targetCustomer = nullptr;
        for (auto& c : customers) {
            if (c.getID() == customerID) {
                targetCustomer = &c;
                break;
            }
        }

        if (!targetCustomer) {
            std::cout << "Error: Customer ID not found.\n";
            return;
        }

        if (!targetCustomer->canRent()) {
            std::cout << "Error: Customer " << targetCustomer->getName() << " already has an active rental.\n";
            return;
        }

        Vehicle* targetVehicle = nullptr;
        for (auto& v : fleet) {
            if (v->getMakeModel() == modelName) {
                targetVehicle = v.get();
                break;
            }
        }

        if (!targetVehicle) {
            std::cout << "Error: Vehicle model not found.\n";
            return;
        }

        if (!targetVehicle->checkAvailable()) {
            std::cout << "Error: Vehicle " << modelName << " is already rented.\n";
            return;
        }

        // Fulfill the rental requirements
        targetVehicle->setAvailable(false);
        targetCustomer->setHasActiveRental(true);
        rentals.emplace_back(targetCustomer, targetVehicle, days);
        std::cout << "Successfully rented " << modelName << " to " << targetCustomer->getName() << " for " << days << " days.\n";
    }

    // R5: Return a vehicle
    void returnVehicle(const std::string& customerID) {
        for (auto& r : rentals) {
            if (r.getCustomer()->getID() == customerID && !r.checkClosed()) {
                r.closeRental();
                r.getVehicle()->setAvailable(true);
                r.getCustomer()->setHasActiveRental(false);
                std::cout << "Vehicle successfully returned by Customer ID: " << customerID << "\n";
                return;
            }
        }
        std::cout << "Error: No active rental record found for Customer ID: " << customerID << "\n";
    }

    // R6: Print summary report
    void printSummary() const {
        std::cout << "\n=========================================\n";
        std::cout << "         RENTAL SYSTEM SUMMARY           \n";
        std::cout << "=========================================\n";
        
        std::cout << "\n--- Active Rentals ---\n";
        bool hasActive = false;
        for (const auto& r : rentals) {
            if (!r.checkClosed()) {
                r.printRentalRecord();
                hasActive = true;
            }
        }
        if (!hasActive) std::cout << "No current active rentals.\n";

        std::cout << "\n--- Fleet Status ---\n";
        int availableCount = 0;
        int rentedCount = 0;
        for (const auto& v : fleet) {
            if (v->checkAvailable()) {
                availableCount++;
            } else {
                rentedCount++;
            }
        }
        std::cout << "Available Vehicles: " << availableCount << "\n";
        std::cout << "Rented Vehicles:    " << rentedCount << "\n";
        std::cout << "=========================================\n\n";
    }
};

// =================================================================
// 5. MAIN SIMULATION (Requirement 4a)
// =================================================================
int main() {
    std::cout << std::fixed << std::setprecision(2);
    RentalSystem system;

    // 1. Add at least 3 vehicles (one of each type)
    system.addVehicle(std::unique_ptr<Vehicle>(new Car("Toyota Camry", 50.0, 5)));
    system.addVehicle(std::unique_ptr<Vehicle>(new Motorbike("Yamaha R6", 30.0)));
    system.addVehicle(std::unique_ptr<Vehicle>(new Truck("Ford F-150", 80.0, 1.5)));

    std::cout << "\n-----------------------------------------\n";

    // 2. Register 2 customers
    system.registerCustomer("Alice Smith", "C001");
    system.registerCustomer("Bob Jones", "C002");

    std::cout << "\n-----------------------------------------\n";

    // 3. Process 2 rentals
    // Rental 1: Bob rents the Truck for 3 days (Should show a 20% surcharge)
    system.processRental("C002", "Ford F-150", 3);
    
    // Rental 2: Alice rents the Motorbike for 10 days (Should show a 10% discount)
    system.processRental("C001", "Yamaha R6", 10);

    // 4. Print Summary to show active state
    system.printSummary();

    // 5. Return 1 vehicle
    std::cout << "Processing a Return...\n";
    system.returnVehicle("C002"); // Bob returns his truck

    // 6. Print Summary again to show updated state
    system.printSummary();

    return 0;
}
