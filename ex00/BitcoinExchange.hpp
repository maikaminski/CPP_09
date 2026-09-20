#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <string>
# include <iostream>
# include <map>

class BitcoinExchange {
    private:
        std::map<std::string, double> _rates;
        
        void parseLine(std::string line);
        std::string trimWhitespace(const std::string& text) const;
        bool validateDate(std::string date);
        double getRateForDate(const std::string& date) const;
    
    public:
        BitcoinExchange();
        BitcoinExchange(const BitcoinExchange& other);
        BitcoinExchange& operator=(const BitcoinExchange& other);
	    ~BitcoinExchange();
        
        
        void parseInput(char *argv);
        void parseRates();
        
};

#endif