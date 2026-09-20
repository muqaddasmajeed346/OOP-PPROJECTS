#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class FlightFullException : public std::runtime_error {
public:
    explicit FlightFullException(const std::string& message) : std::runtime_error(message) {}
};

class InvalidCancellationException : public std::runtime_error {
public:
    explicit InvalidCancellationException(const std::string& message) : std::runtime_error(message) {}
};

#endif
