#include "BitcoinExchange.hpp"
#include <fstream>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <cctype>

BitcoinExchange::BitcoinExchange(){}

BitcoinExchange::BitcoinExchange(const BitcoinExchange& other) : _rates(other._rates) {}

BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
	{
		_rates = other._rates;
	}
	return *this;
}

BitcoinExchange::~BitcoinExchange(){}

void BitcoinExchange::parseRates(){
    std::ifstream file("data.csv");
    if (!file) 
        throw std::runtime_error("Error: could not open the data.csv file.");
    std::string line;
    std::string date;
    std::string rate;
    while (std::getline(file, line)){
        std::size_t commaPosition = line.find(',');
        std::string date = line.substr(0, commaPosition);
        std::string rate = line.substr(commaPosition + 1);
        _rates[date] = std::strtod(rate.c_str(), NULL);
    }
    file.close();
}

std::string BitcoinExchange::trimWhitespace(const std::string& text) const
{
	std::size_t first = text.find_first_not_of(" \t\r\n");
	if (first == std::string::npos)
	{
		return "";
	}
	std::size_t last = text.find_last_not_of(" \t\r\n");
	return text.substr(first, last - first + 1);
}

void BitcoinExchange::parseInput(char * argv) {
    std::ifstream file(argv);
    if (!file)
        throw std::runtime_error("Error: could not open the input file.");

    std::string line;
    std::string date;
    std::string amount;
    bool firstLine = true;
    
    while (std::getline(file, line)){
		if (firstLine)
		{
			firstLine = false;
			std::string trimmedLine = trimWhitespace(line);
			if (trimmedLine == "date | value" || trimmedLine.empty())
				continue;
		}
        if(line.empty())
            continue;
        
        try {parseLine(line);}
        catch( std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    file.close();
}

void BitcoinExchange::parseLine(std::string line) {
        std::size_t pipePosition = line.find('|');
        if (pipePosition == std::string::npos)
            throw std::runtime_error("Error: bad input => " + line);

        std::string date = trimWhitespace(line.substr(0, pipePosition));
        std::string amount = trimWhitespace(line.substr(pipePosition + 1));
        if(!validateDate(date))
            throw std::runtime_error("Error: invalid date => " + line);
        if (amount.empty())
            throw std::runtime_error("Error: bad input => " + line);
        
        char* end = NULL;
        double amountDouble = std::strtod(amount.c_str(), &end);
        // No numeric content at all (e.g. "", "abc")
        if (end == amount.c_str())
            throw std::runtime_error("Error: bad input => " + line);
        // Extra junk after number (e.g. "12abc", "3.2.1")
        if (*end != '\0')
            throw std::runtime_error("Error: bad input => " + line);
        if (amountDouble < 0)
            throw std::runtime_error("Error: not a positive number.");
        if (amountDouble > 1000)
            throw std::runtime_error("Error: too large a number. Choose between 1 and 1000");
        
        double exchangeRate = getRateForDate(date);
        double result = amountDouble * exchangeRate;

        std::cout << date << " => " << amount << " = " << result << std::endl;
}



double BitcoinExchange::getRateForDate(const std::string& date) const
{
    if (_rates.empty())
        throw std::runtime_error("Error: database is empty.");

    // lower_bound returns iterator to first key that is >= than 'date'
    std::map<std::string, double>::const_iterator it = _rates.lower_bound(date);

    // Case 1: exact match. lower_bound found a match and key == date.
    // it->second is the value of thhe key in the map container.
    if (it != _rates.end() && it->first == date)
        return it->second;

    // Case 2: date is earlier than the first date in the DB
    // lower_bound(date) == begin() means there is no key < date
    if (it == _rates.begin())
        throw std::runtime_error("Error: no rate available for this date.");

    // Case 3: lower_bound points to first key > date (or end)
    // the closest lower date is the previous element
    --it;
    return it->second;
}

bool BitcoinExchange::validateDate(std::string date){
    if(date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;
    
    std::string year = date.substr(0, 4);
    std::string month = date.substr(5, 2);
    std::string day = date.substr(8, 2);
    
    if(year.find_first_not_of("0123456789") != std::string::npos)
        return false;
    if(month.find_first_not_of("0123456789") != std::string::npos)
        return false;
    if(day.find_first_not_of("0123456789") != std::string::npos)
        return false;

    long monthLong = std::strtol(month.c_str(), NULL, 10);
    long dayLong = std::strtol(day.c_str(), NULL, 10);
    if(monthLong < 1 || monthLong > 12)
        return false;
    
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (dayLong > daysInMonth[monthLong] || dayLong < 1)
        return false;
    
    return true;
}